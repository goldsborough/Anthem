//
//  TextParser.h
//  Vibe
//
//  Created by Peter Goldsborough on 19/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Vibe__mParser__
#define __Vibe__mParser__

#include <string>
#include <vector>
#include <map>

class TextParser
{
    
public:
    
    TextParser() { };
    TextParser(const std::string& fname) { open(fname); }
    
    void open(const std::string& fname);
    
    bool readItem(std::string&);
    
    std::vector<std::string> readAll();
    
private:
    
    typedef std::vector<std::string> strVec;
    
    strVec _lines;
    
    strVec::const_iterator _curr;
    
    bool _open;
};


class XMLNode
{
    
public:
    
    std::string getAttr(std::string attrKey);
    
    void setAttr(std::string key, std::string val);
    
    std::string getTag() { return tag; }
    void setTag(std::string name);
    
    XMLNode* getParent() { return parent; }
    XMLNode* getPrevSibling() { return prevSibling; }
    XMLNode* getNextSibling() { return nextSibling; }
    XMLNode* getFirstChild() { return firstChild; }
    XMLNode* getLastChild() { return lastChild; }
    
    void appendChild(XMLNode* node);
    
    void deleteFirstChild();
    void deleteLastChild();
    
private:
    
    friend class XMLParser;
    
    typedef std::map<std::string,std::string> AttrMap;
    
    AttrMap attrs;
    
    std::string tag;
    
    std::string data;
    
    XMLNode* parent       = 0;
    
    XMLNode* prevSibling  = 0;
    XMLNode* nextSibling  = 0;
    
    XMLNode* firstChild   = 0;
    XMLNode* lastChild    = 0;
    
    bool isClosed = false;
    bool isClosing = false;
    bool selfClosed = false;
};



class XMLParser
{
    
public:
    
    XMLParser() { };
    XMLParser(const std::string& fname) { open(fname); }
    
    ~XMLParser();
    
    XMLNode * open(const std::string& fname);
    
private:
    
    typedef std::vector<XMLNode*> NodeVec;
    typedef std::vector<std::string> StrVec;
    
    typedef NodeVec::const_iterator NodeVec_cItr;
    typedef StrVec::const_iterator StrVec_cItr;
    typedef std::string::const_iterator str_cItr;
    
    XMLNode * _makeNode(str_cItr begin, str_cItr end, bool docHead = false);
    
    StrVec_cItr _makeNodeTree(StrVec_cItr itr, StrVec_cItr end, XMLNode * parent);
    
    std::vector<std::string> _parse(str_cItr begin, str_cItr end);
    
    bool _isTag_Copy(str_cItr beign, str_cItr end);

    bool isSelfClosing(str_cItr begin, str_cItr end) const;
    
    StrVec_cItr findClosingTag(str_cItr tagBegin, str_cItr tagEnd,
                               StrVec_cItr vecBegin, StrVec_cItr vecEnd) const;
    
    std::string splitOne(str_cItr begin, str_cItr end) const;
    
    XMLNode::AttrMap getAttrs(str_cItr begin, str_cItr end);
    
    std::string condense(str_cItr begin, str_cItr end) const;
    
    void _deleteTree(XMLNode * root);
    
    bool _isComment(const std::string& str);
    
    NodeVec _nodes;
    
    bool _open;
    
    XMLNode * _root;
    
};

#endif /* defined(__Vibe__mParser__) */
