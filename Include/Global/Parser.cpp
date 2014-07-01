//
//  TextParser.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 19/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Parser.h"
#include "Errors.h"
#include "Global.h"
#include <fstream>




#include <iostream>




std::string strip(str_cItr begin, str_cItr end)
{
    while (begin != end && ::isspace(*begin)) ++begin;
    
    while (end != begin && ( ::isspace(*end) || *end == '\0' )) --end;
    
    return std::string(begin,++end);
}

std::vector<std::string> split(str_cItr begin, str_cItr end)
{
    std::vector<std::string> vec;
    
    str_cItr j = begin;
    
    while (j != end)
    {
        begin = std::find_if_not(j, end, ::isspace);
        
        j = std::find_if(begin,end,::isspace);
        
        vec.push_back(std::string(begin,j));
    }
    
    return vec;
}

std::string splitOne(str_cItr begin, str_cItr end)
{
    str_cItr i,j;
    
    i = std::find_if_not(begin, end, ::isspace);
    
    j = std::find_if(i, end, ::isspace);
    
    return std::string(i,j);
}

std::string condense(str_cItr begin, str_cItr end)
{
    std::string s(begin,end);
    
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace),s.end());
    
    return s;
}

std::string join(vec_cItr begin, vec_cItr end)
{
    std::string s;
    
    while (begin != end)
        s += *begin++;
    
    return s;
}

void TextParser::save()
{
    // save to file ...
    
    
    
    _file.clear();
}

void TextParser::eraseWord()
{
    _currWord = _currLine->erase(_currWord);
    
    if (_currWord == _currLine->end()) moveWord(-1);
}

void TextParser::eraseLine()
{
    _currLine = _file.erase(_currLine);
    
    if (_currLine == _file.end()) moveLine(-1);
}

void TextParser::replaceLine(const std::string &str)
{
    eraseLine();
    
    insertInFile(str);
}

void TextParser::replaceWord(const std::string& str)
{
    eraseWord();
    
    insertInLine(str);
}

void TextParser::insertInLine(const std::string& str)
{
    wordVec vec = split(str.begin(), str.end());

    // insert all words (last word first)
    for (std::vector<std::string>::const_reverse_iterator itr = vec.rbegin(), end = vec.rend();
         itr != end;
         ++itr)
    {
        _currWord = _currLine->insert(_currWord, *itr);
    }
}

void TextParser::appendToLine(const std::string& str)
{
    // split line into words
    wordVec vec = split(str.begin(), str.end());
    
    toLineEnd();
    
    // append all to line
    for (wordItr itr = vec.begin(), end = vec.end();
         itr != end;
         ++itr)
    {
        _currLine->push_back(*itr);
    }
    
    // first new word
    moveWord(1);
}

void TextParser::newFile(const std::string &fname)
{
    _fname = fname;
    _currLine = _file.begin();
}

void TextParser::openFile(const std::string& fname)
{
    std::fstream file(fname, std::ios::in | std::ios::out);
    
    if (! file.is_open() || ! file.good())
        throw FileOpenError();
        
    std::string s;
    
    _fname = fname;
    
    // grab all lines and process them
    while (getline(file, s))
        _file.push_back(split(s.begin(), s.end()));
    
    _currLine = _file.begin();
    
    _currWord = _currLine->begin();
}

std::vector<std::string> TextParser::readAllItems()
{
    wordVec vec;
    
    for (lineItr line = _file.begin(), fileEnd = _file.end();
         line != fileEnd;
         ++line)
    {
        if (! line->empty())
        {
            // check for comment-only lines by grabbing the first word
            std::string w = *(line->begin());
            
            // check if line is a comment, else make into string and store in vector
            if (! w.empty() && condense(w.begin(), w.end())[0] != '#')
            {
                for (wordItr word = line->begin(), lineEnd = line->end();
                     word != lineEnd;
                     ++word)
                {
                    vec.push_back(*word);
                }
            }
        }
    }
    
    return vec;
}

std::vector<std::string> TextParser::readAllLines()
{
    wordVec vec;
    
    for (lineItr line = _file.begin(), fileEnd = _file.end();
         line != fileEnd;
         ++line)
    {
        if (! line->empty())
        {
            // check for comment-only lines by grabbing the first word
            std::string w = *(line->begin());
            
            // check if line is a comment, else make into string and store in vector
            if (! w.empty() && condense(w.begin(), w.end())[0] != '#')
            {
                std::string lineStr;
                
                for (wordItr word = line->begin(), end = line->end();
                     word != end;
                     ++word)
                {
                    lineStr += (*word);
                }
                
                vec.push_back(lineStr);
            }
        }
    }
    
    return vec;
}

Wavetable VibeWTParser::readWT(const std::string &fname)
{
    std::ifstream file(fname);
    
    if (! file.is_open())
        throw FileNotOpenError("Could not find wavetable file: " + fname);
    
    if (! file.good())
        throw FileOpenError("Error opening wavetable: " + fname);
    
    char id[4];
    
    file.read(id, 4);
    
    if (strncmp(id, "VIBE", 4) != 0)
        throw ParseError("Invalid signature for Vibe file!");
    
    int len = global.wtLen + 1;
    int size = len * sizeof(double);
    
    double * wt = new double [len];
    
    file.read(reinterpret_cast<char*>(wt), size);
    
    return Wavetable(wt,global.wtLen);
}

void VibeWTParser::writeWT(const std::string &fname, const Wavetable& wt)
{
    std::ofstream file(fname);
    
    if (! file.is_open())
        throw FileNotOpenError();
    
    if (! file.good())
        throw FileOpenError();
    
    file.write("VIBE", 4);
    
    int size = (global.wtLen + 1) * sizeof(double);
    
    file.write(reinterpret_cast<char*>(wt._data), size);
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

XMLNode::AttrMap XMLParser::getAttrs(str_cItr begin, str_cItr end) const
{
    XMLNode::AttrMap attrs;
    
    str_cItr j;
    
    j = begin;
    
    while (begin != end)
    {
        // find first non-space char
        begin = std::find_if_not(j, end, ::isspace);
        
        // find '=', lambda expression
        j = std::find_if(begin, end, [&] (const char& c) { return c == '='; });
        
        // from the first non-space char to the '=' is the key
        std::string key(begin,j);
        
        key = condense(key.begin(), key.end());
        
        // find the pair of parantheses
        
        begin = std::find(j, end, '\"');
        
        // need to increment begin to get the content anyway
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
    str_cItr i,j;
    
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

void XMLParser::close()
{
    if (! _saved)
        saveToSameFile(_root);
    
    _deleteTree(_root);
}
