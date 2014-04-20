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

std::string XMLNode::getAttr(std::string attrKey)
{
    if (attrs.find(attrKey) == attrs.end())
        throw std::invalid_argument("Invalid attribute!");
    
    return attrs[attrKey];
}

bool XMLParser::_isComment(const std::string &str)
{
    return (condense(str.begin(), str.end())).substr(0,4) == "<!--";
}

XMLNode *  XMLParser::open(const std::string& fname)
{
    std::ifstream file(fname);
    
    _open = false;

    if (! file.good() || ! file.is_open())
        throw FileOpenError();
    
    _open = true;
    
    StrVec vec;
    std::string s,str;

    getline(file, str);
    
    _root = _makeNode(str.begin(), str.end(),true);
    
    _root->isClosed = true;
    
    str = "";
    
    while (getline(file,s)) str += s;
    
    vec = _parse(str.begin(), str.end());
    
     _makeNodeTree(vec.begin(), vec.end(),_root);
    
    return _root;
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

void XMLNode::deleteFirstChild()
{
    
    XMLNode * node = 0;
    
    if (firstChild->nextSibling != 0)
        node = firstChild->nextSibling;
    
    delete firstChild;
    
    firstChild = node;
}

void XMLNode::deleteLastChild()
{
    XMLNode * node = lastChild->prevSibling;
    
    delete lastChild;
    
    lastChild = node;
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

XMLNode::AttrMap XMLParser::getAttrs(str_cItr begin, str_cItr end)
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

XMLParser::StrVec_cItr XMLParser::findClosingTag(str_cItr tagBegin, str_cItr tagEnd,
                                                 StrVec_cItr vecBegin, StrVec_cItr vecEnd) const
{
    while (vecBegin != vecEnd &&
           ! (tagEnd - tagBegin > 2 && *tagBegin == '/'))
          ++vecBegin;
    
    return vecBegin;
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
    
    str_cItr tBegin = node->tag.begin();
    str_cItr tEnd = node->tag.end();
    
    node->isClosing = (tEnd - tBegin > 2 && *tBegin == '/');
    
    return node;
}

XMLParser::StrVec_cItr XMLParser::_makeNodeTree(StrVec_cItr itr, StrVec_cItr end, XMLNode * parent)
{
    if (std::find(itr->begin(), itr->end(), '<') == itr->end())
        parent->data += *itr;
    
    else
    {
        XMLNode * node;
        bool isClosingOfPar;
        
        node = _makeNode(itr->begin(), itr->end());
        
        isClosingOfPar = node->isClosing &&
                         std::equal(node->tag.begin() + 1, node->tag.end() , parent->tag.begin());
        
        if (node->selfClosed)
            parent->selfClosed = true;
        
        else if (node->isClosing)
        {
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
            parent->appendChild(node);
            
            while (! node->isClosed)
            {
                itr = _makeNodeTree(++itr, end, node);
                
                if (itr == end)
                    throw ParseError("Could not find matchin closing parantheses");
            }
        }
    }
    
    return itr;
}

void XMLParser::_deleteTree(XMLNode * root)
{
    while (root->firstChild != 0)
        _deleteTree(root->firstChild);
    
    if (root->parent != 0)
        root->parent->deleteFirstChild();
    
    else delete root;
}

XMLParser::~XMLParser()
{
    _deleteTree(_root);
}
