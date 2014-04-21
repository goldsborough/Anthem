//
//  TextParser.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 19/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Parser.h"
#include "Errors.h"
#include <fstream>
#include <iostream>

void TextParser::open(const std::string& fname)
{
    std::ifstream file(fname);
    
    _open = false;
    
    if (! file.good() || ! file.is_open())
        throw FileOpenError();
    
    _open = true;
    
    std::string s;
    
    while (getline(file, s))
        if (! s.empty() && ! isspace(s[0]) && s[0] != '#')
            _lines.push_back(s);
    
    _curr = _lines.begin();

}

bool TextParser::readItem(std::string& str)
{
    if (! _open)
        throw FileNotOpenError();
    
    if (_curr++ != _lines.end())
    {
        str = *_curr;
        return true;
    }
    
    return false;

}

std::vector<std::string> TextParser::readAll()
{
    if (! _open)
        throw FileNotOpenError();
    
    return _lines;
}

XMLNode * XMLParser::open(const std::string& fname)
{
    std::ifstream file(fname);
    
    _fname = fname;
    
    _open = false;

    if (! file.good() || ! file.is_open())
        throw FileOpenError();
    
    _open = true;
    
    StrVec vec;
    std::string s,str;

    getline(file, s,'>');
    
    // go back in file
    file.unget();
    
    // get a char
    s += file.get();
    
    _hasHeader = isHeader(s.begin() + 1, s.end() - 1);
    
    _root = new XMLNode;
    
    if (_hasHeader)
    {
        _root = _makeNode(s.begin(), s.end(),true);
        
        _root->isClosed = true;
    }
    
    else str += s;
    
    while (getline(file,s)) str += s;
    
    vec = _parse(str.begin(), str.end());
    
     _makeNodeTree(vec.begin(), vec.end(),_root);
    
    return _root;
}

bool XMLParser::isHeader(str_cItr begin, str_cItr end)
{
    std::string s = condense(begin, end);
    
    return *(s.begin()) == '?' && *(s.end() - 1) == '?';
}

template <class T>
T XMLParser::lastNonSpace(T begin, T end)
{
    return (condense(begin, end)).end();
}

bool XMLNode::getAttr(std::string& str, const std::string& attrKey)
{
    if (! findAttr(attrKey))
        return false;
    
    str = attrs[attrKey];
    
    return true;
}

bool XMLNode::removeAttr(const std::string &key)
{
    if (! findAttr(key))
        return false;
    
    attrs.erase(key);
    
    return true;
}

XMLNode::NodeVec XMLNode::getElementsByTagName(const std::string& tagName)
{
    NodeVec vec;
    
    XMLNode * itr = firstChild;
    
    while (itr != 0)
    {
        if (itr->tag != tagName)
            continue;
        
        vec.push_back(itr);
        
        itr = itr->nextSibling;
    }
    
    return vec;
}

XMLNode::NodeVec XMLNode::getElementsByAttrName(const std::string& attrName)
{
    NodeVec vec;
    
    XMLNode * itr = firstChild;
    
    while (itr != 0)
    {
        if (! findAttr(attrName))
            continue;
        
        vec.push_back(itr);
        
        itr = itr->nextSibling;
    }
    
    return vec;
}

bool XMLNode::insertChild(XMLNode* childOfThisNode, XMLNode * node)
{
    if (childOfThisNode == 0 ||
        childOfThisNode->parent != this)
        return false;
    
    // connect the node with the child
    node->nextSibling = childOfThisNode;
    childOfThisNode->prevSibling = node;
    
    // if there was a previous sibling, connect it with the node
    if( childOfThisNode->prevSibling != 0)
    {
        node->prevSibling = childOfThisNode->prevSibling;
        
        childOfThisNode->prevSibling->nextSibling = node;
    }
    
    return true;
}

bool XMLNode::removeChild(XMLNode* childOfThisNode)
{
    // check if valid node
    if (childOfThisNode == 0 ||
        childOfThisNode->parent != this)
        return false;
    
    // if there is a previous sibling
    // and a next, connect those two
    if (childOfThisNode->prevSibling != 0 &&
        childOfThisNode->nextSibling != 0)
    {
        // connect the previous and the next
        childOfThisNode->prevSibling->nextSibling =
        childOfThisNode->nextSibling;
        
        childOfThisNode->nextSibling->prevSibling =
        childOfThisNode->prevSibling;
    }
    
    // if this is the last child of a node
    // connect it to the previous if there
    // is one, else make the parent's last
    // child pointer be 0
    if (childOfThisNode == lastChild)
    {
        if (childOfThisNode->prevSibling != 0)
        {
            lastChild = childOfThisNode->prevSibling;
            lastChild->nextSibling = 0;
        }
        
        else lastChild = 0;
    }
    
    // same as for last child, but for previous child
    if (childOfThisNode == firstChild)
    {
        if (childOfThisNode->nextSibling != 0)
        {
            firstChild = childOfThisNode->nextSibling;
            firstChild->prevSibling = 0;
        }
        
        else firstChild = 0;
    }
    
    delete childOfThisNode;
    
    return true;
}

XMLParser::StrVec XMLParser::_parse(str_cItr begin, str_cItr end)
{
    StrVec vec;
    
    std::string s;
    
    str_cItr i = begin;
    str_cItr j = i;
    
    while (j != end)
    {
        j = std::find(i, end, '<');
        
        s = std::string(i,j);
        
        if (! s.empty()
            && s.substr(0,4) != "<!--"
            && std::find_if_not(s.begin(), s.end(), ::isspace) != s.end())
            vec.push_back(s);
        
        i = std::find(j, end, '>');
        
        s = std::string(j,++i);
        
        if (! s.empty()
            && s.substr(0,4) != "<!--"
            && std::find_if_not(s.begin(), s.end(), ::isspace) != s.end())
            vec.push_back(s);
    }
    
    return vec;
}

void XMLNode::prependChild(XMLNode *node)
{
    if (node == this)
        throw ParseError("Prepending Node to self");
    
    node->parent = this;
    
    if (firstChild != 0)
    {
        firstChild->prevSibling = node;
        
        node->nextSibling = firstChild;
    }
    
    firstChild = node;
    
    if (lastChild == 0)
        lastChild = firstChild;
}

void XMLNode::appendChild(XMLNode *node)
{
    if (node == this)
        throw ParseError("Appending Node to self");
    
    node->parent = this;
    
    if (lastChild != 0)
    {
        lastChild->nextSibling = node;
        
        node->prevSibling = lastChild;
    }
    
    lastChild = node;
    
    if (firstChild == 0)
        firstChild = lastChild;
}

std::string XMLParser::splitOne(XMLParser::str_cItr begin, XMLParser::str_cItr end) const
{
    XMLParser::str_cItr i,j;
    
    i = std::find_if_not(begin, end, ::isspace);
    
    j = std::find_if(i, end, ::isspace);
    
    return std::string(i,j);
}

std::string XMLParser::condense(str_cItr begin, str_cItr end) const
{
    std::string s(begin,end);
    
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace),s.end());

    return s;
}

XMLNode::AttrMap XMLParser::getAttrs(str_cItr begin, str_cItr end) const
{
    XMLNode::AttrMap attrs;
    
    XMLParser::str_cItr j;
    
    j = begin;
    
    while (begin != end)
    {
        // find first non-space char
        begin = std::find_if_not(j, end, ::isspace);
        
        // find '='
        j = std::find_if(begin, end, [&] (const char& c) { return c == '='; });
        
        // from the first non-space char to the '=' is the key
        std::string key(begin,j);
        
        key = condense(key.begin(), key.end());
        
        // find the pair of parantheses
        
        begin = std::find(j, end, '\"');
        
        j = std::find(++begin, end, '\"');
        
        // the value is between the parantheses
        std::string val(begin, j);
        
        val = condense(val.begin(), val.end());
        
        // create new entry
        attrs[key] = val;
        
        begin = ++j;
    }
    
    return attrs;
}

bool XMLParser::isSelfClosing(str_cItr begin, str_cItr end) const
{
    while (end != begin && isspace(*end--));
    
    return *end == '/';
}

XMLNode * XMLParser::_makeNode(str_cItr begin, str_cItr end, bool docHead)
{
    XMLParser::str_cItr i,j;
    
    i = std::find(begin, end, '<');
    
    j = std::find(i, end, '>');
    
    if (j == end)
        throw ParseError("Could not find matching brackets '<' '>' !");
    
    
    if (docHead) ++i, --j;
    
    // remove brackets
    std::string curr(++i,j);
    
    if (curr.empty())
        throw ParseError("Empty tag found!");
    
    std::string tag = splitOne(curr.begin(), curr.end());

    curr.erase(0,tag.size());
    
    XMLNode::AttrMap attrs = getAttrs(curr.begin(), curr.end());
    
    XMLNode * node = new XMLNode;
    
    node->tag = tag;
    node->attrs = attrs;
    
    node->selfClosed = isSelfClosing(curr.begin(), curr.end());
    
    return node;
}

std::string XMLParser::strip(str_cItr begin, str_cItr end) const
{
    while (begin != end && ::isspace(*begin)) ++begin;
    
    while (end != begin && ( ::isspace(*end) || *end == '\0' )) --end;
    
    return std::string(begin,++end);
}

XMLParser::StrVec_cItr XMLParser::_makeNodeTree(StrVec_cItr itr, StrVec_cItr end, XMLNode * parent)
{
    if (std::find(itr->begin(), itr->end(), '<') == itr->end())
        parent->data += strip(itr->begin(), itr->end());
    
    else
    {
        XMLNode * node;
        
        node = _makeNode(itr->begin(), itr->end());
        
        bool isClosing = node->tag.end() - node->tag.begin() > 2 &&
                         *(node->tag.begin()) == '/';
        
        if (node->selfClosed)
            parent->selfClosed = true;
        
        else if (isClosing)
        {
            bool isClosingOfPar = std::equal(node->tag.begin() + 1,
                                             node->tag.end() , parent->tag.begin());
            
            if (isClosingOfPar)
            {
                if (parent->isClosed)
                    throw ParseError("Found duplicate closing tag for opening tag");
                
                parent->isClosed = true;
                
            } else
                throw ParseError("Found closing tag that does not close current node!");
        }
        
        else
        {
            // append node to current parent
            parent->appendChild(node);
            
            while (! node->isClosed)
            {
                itr = _makeNodeTree(++itr, end, node);
                
                if (itr == end)
                    throw ParseError("Could not find matching closing tag!");
            }
        }
    }
    
    return itr;
}

std::string XMLParser::_nodeToString(const XMLNode *node, std::string indent, bool docHead) const
{
    std::string str;
    
    if (node != 0)
    {
        str = "<";
        
        if (docHead) str += "?";
        
        str += node->tag;
        
        for (XMLNode::AttrMap::const_iterator itr = node->attrs.begin(), end = node->attrs.end();
             itr != end;
             ++itr)
        {
            str += " " + itr->first + "=\"" + itr->second + "\"";
        }
        
        if (docHead) str += "?";
        
        if (node->selfClosed) str += "/";
        
        str += ">";
        
        str = indent + str + "\n";
        
        if (node->hasData())
            str += indent + node->data + "\n";
    }
    
    return str;
}

std::string XMLParser::_treeToString(const XMLNode * root, std::string& str, std::string indent) const
{
    if (root != 0)
    {
        std::string nodeStr = _nodeToString(root,indent);
        
        str += nodeStr;
        
        if (root->hasChildren())
            _treeToString(root->firstChild, str,indent + "\t");
        
        if (! root->selfClosed)
        {
            if (! root->hasChildren() && nodeStr.size() <= 50)
            {
                std::string::const_iterator i = std::find(str.rbegin(), str.rend(), '>').base() - 1;
                
                while (i != str.end())
                    if (*i == '\n' || *i == '\t')
                        i = str.erase(i++);
                    else ++i;
                
            } else str += indent;
            
            str += "</" + root->tag + ">\n";
        }
        
        if (! root->isLastChild())
            _treeToString(root->nextSibling, str,indent);
    }
    
    return str;
}

void XMLParser::saveToDiffFile(XMLNode * root, const std::string& fname)
{
    std::string treeStr;
    
    if (_hasHeader)
        treeStr = _nodeToString(root,"",true);
    
    _treeToString(root->firstChild,treeStr);
    
    std::ofstream outFile(fname);
    
    outFile.write(treeStr.c_str(), treeStr.size());
    
    _saved = true;
}

void XMLParser::_deleteTree(XMLNode * node)
{
    while (node->hasChildren())
        _deleteTree(node->lastChild);
    
    if (node->parent != 0)
        node->parent->removeChild(node);
    
    else delete node;
}

XMLParser::~XMLParser()
{
    if (! _saved)
        saveToSameFile(_root);
    
    _deleteTree(_root);
}
