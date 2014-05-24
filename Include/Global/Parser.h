//
//  TextParser.h
//  Vibe
//
//  Created by Peter Goldsborough on 19/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Vibe__mParser__
#define __Vibe__mParser__

#include "Wavetable.h"

#include <string>
#include <vector>
#include <map>

typedef std::string::iterator str_Itr;
typedef std::string::const_iterator str_cItr;

typedef std::vector<std::string>::const_iterator vec_cItr;

std::string condense(str_cItr begin, str_cItr end);

std::string strip(str_cItr begin, str_cItr end);

std::vector<std::string> split(str_cItr begin, str_cItr end);

std::string join(vec_cItr begin, vec_cItr end);

std::string splitOne(str_cItr begin, str_cItr end);

class TextParser
{
    
public:
    
    TextParser() { };
    TextParser(const std::string& fname) { open(fname); }
    
    ~TextParser() { close(); }
    
    void open(const std::string& fname);
    void close(){}
    
    bool readWord(std::string& str);
    bool readLine(std::string& str);
    
    bool appendWord(const std::string& str);
    bool appendLine(const std::string& str);
    
    bool insertWord(const std::string& str);
    bool insertLine(const std::string& str);
    
    bool eraseWord();
    bool eraseLine();
    
    bool replaceWord(const std::string& str);
    bool replaceLine(const std::string& str);
    
    void toLineStart()
    { _currWord = _currLine->begin(); }
    
    void toFileStart()
    { _currLine = _file.begin(); }
    
    void toLineEnd()
    { _currWord = _currLine->end(); }
    
    void toFileEnd()
    { _currLine = _file.end(); }
    
    void moveWord(int count)
    { std::advance(_currWord, count); }
    
    void moveLine(int count)
    { std::advance(_currLine, count); }
    
    std::vector<std::string> readAllItems();
    std::vector<std::string> readAllLines();
    
private:
    
    typedef std::vector<std::string> wordVec;
    typedef std::vector<std::vector<std::string>> lineVec;
    
    typedef wordVec::iterator wordItr;
    typedef lineVec::iterator lineItr;
    
    lineVec _file;

    wordItr _currWord;
    lineItr _currLine;
    
    bool _open;
};

struct VibeWTParser
{
    Wavetable readWT(const std::string& fname);
    
    void writeWT(const std::string& fname, const Wavetable& wt);
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
    
    ~XMLParser() { close(); }
    
    XMLNode * open(const std::string& fname);
    
    void close();
    
    void saveToDiffFile(XMLNode * root, const std::string& fname);
    
    void saveToSameFile(XMLNode * root) { saveToDiffFile(root, _fname); }
    
private:

    typedef std::vector<std::string> StrVec;
    
    typedef StrVec::const_iterator StrVec_cItr;
    
    XMLNode * _makeNode(str_cItr begin, str_cItr end, bool docHead = false);
    
    StrVec_cItr _makeNodeTree(StrVec_cItr itr, StrVec_cItr end, XMLNode * parent);
    
    std::vector<std::string> _parse(str_cItr begin, str_cItr end);

    bool isSelfClosing(str_cItr begin, str_cItr end) const;
    
    XMLNode::AttrMap getAttrs(str_cItr begin, str_cItr end) const;
    
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
