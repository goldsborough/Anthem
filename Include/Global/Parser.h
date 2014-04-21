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
    
    typedef std::vector<XMLNode*> NodeVec;
    typedef NodeVec::const_iterator NodeVec_cItr;
    
    bool getAttr(std::string& str, const std::string& attrKey);
    
    bool findAttr(const std::string& key)
    { return attrs.find(key) != attrs.end(); }
    
    void addAttr(const std::string& key, const std::string& val)
    { attrs[key] = val; }
    
    void setAttr(const std::string& key, const std::string& val)
    { if (findAttr(key)) addAttr(key, val); }
    
    bool removeAttr(const std::string& key);
    
    std::string getTag() { return tag; }
    void setTag(const std::string& name) { tag = name; }
    
    NodeVec getElementsByTagName(const std::string& tagName);
    NodeVec getElementsByAttrName(const std::string& attrName);
    
    std::string getData() const { return data; }
    std::string::size_type getDataLength() const { return data.size(); }
    
    bool hasData() const { return ! data.empty(); }
    
    void setData(const std::string& newData) { data = newData; }
    void appendData(const std::string& newData) {data += newData; }
    
    void insertData(const std::string::size_type ind, const std::string& newData)
    { data.insert(ind, newData); }
    
    void replaceData(const std::string& oldData, const std::string& newData)
    { data.replace(oldData.begin(), oldData.end(), newData); }
    
    std::string splitData(const std::string::size_type ind)
    { return data.substr(ind); }
    
    std::string substringData(const std::string::size_type ind,
                              std::string::size_type count = std::string::npos)
    { return data.substr(ind,count); }
    
    void deleteData() { data.erase(); }
    
    XMLNode* getParent() const { return parent; }
    XMLNode* getPrevSibling() const { return prevSibling; }
    XMLNode* getNextSibling() const { return nextSibling; }
    XMLNode* getFirstChild() const { return firstChild; }
    XMLNode* getLastChild() const { return lastChild; }
    
    bool isLastChild() const
    { return parent->lastChild != 0 && this == parent->lastChild; }
    
    bool isFirstChild() const
    { return parent->lastChild != 0 && this == parent->firstChild; }
    
    bool hasChildren() const { return firstChild != 0 && lastChild != 0; }
    
    void prependChild(XMLNode * node);
    void appendChild(XMLNode* node);
    
    bool insertChild(XMLNode* childOfThisNode, XMLNode * node);
    bool removeChild(XMLNode* childOfThisNode);
    
    void removeFirstChild() { removeChild(firstChild); }
    void removeLastChild() { removeChild(lastChild); }
    
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
    bool selfClosed = false;
};



class XMLParser
{
    
public:
    
    XMLParser() { };
    XMLParser(const std::string& fname) { open(fname); }
    
    ~XMLParser();
    
    XMLNode * open(const std::string& fname);
    
    void saveToDiffFile(XMLNode * root, const std::string& fname);
    
    void saveToSameFile(XMLNode * root) { saveToDiffFile(root, _fname); }
    
private:

    typedef std::vector<std::string> StrVec;
    
    typedef StrVec::const_iterator StrVec_cItr;
    typedef std::string::const_iterator str_cItr;
    
    XMLNode * _makeNode(str_cItr begin, str_cItr end, bool docHead = false);
    
    StrVec_cItr _makeNodeTree(StrVec_cItr itr, StrVec_cItr end, XMLNode * parent);
    
    std::vector<std::string> _parse(str_cItr begin, str_cItr end);

    bool isSelfClosing(str_cItr begin, str_cItr end) const;
    
    std::string splitOne(str_cItr begin, str_cItr end) const;
    
    XMLNode::AttrMap getAttrs(str_cItr begin, str_cItr end) const;
    
    std::string condense(str_cItr begin, str_cItr end) const;
    
    std::string strip(str_cItr begin, str_cItr end) const;
    
    template <class T>
    T lastNonSpace(T begin, T end);
    
    bool isHeader(str_cItr begin, str_cItr end);
    
    void _deleteTree(XMLNode * node);
    
    std::string _nodeToString(const XMLNode * node, std::string indent = "", bool docHead = false) const;
    std::string _treeToString(const XMLNode * root, std::string& str, std::string indent = "") const;
    
    std::string _fname;
    
    bool _open;
    bool _saved;
    bool _hasHeader;
    
    XMLNode * _root;
    
};

#endif /* defined(__Vibe__mParser__) */
