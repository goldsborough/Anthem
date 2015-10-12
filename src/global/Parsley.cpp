/********************************************************************************************//*!
*
*  @file        Parsley.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Global.hpp"
#include "Parsley.hpp"

#include <fstream>

std::string strip(Str_cItr begin, Str_cItr end)
{
    while (begin != end && ::isspace(*begin)) ++begin;
    
    while (end != begin && ( ::isspace(*end) || *end == '\0' )) --end;
    
    return std::string(begin,++end);
}

std::vector<std::string> split(Str_cItr begin, Str_cItr end)
{
    std::vector<std::string> vec;
    
    Str_cItr j = begin;
    
    while (j != end)
    {
        begin = std::find_if_not(j, end, ::isspace);
        
        j = std::find_if(begin,end,::isspace);
        
        vec.push_back(std::string(begin,j));
    }
    
    return vec;
}

std::string splitOne(Str_cItr begin, Str_cItr end)
{
    Str_cItr i,j;
    
    i = std::find_if_not(begin, end, ::isspace);
    
    j = std::find_if(i, end, ::isspace);
    
    return std::string(i,j);
}

std::string condense(Str_cItr begin, Str_cItr end)
{
    std::string s(begin,end);
    
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace),s.end());
    
    return s;
}

std::string join(Vec_Itr begin, Vec_Itr end, const std::string& str = " ")
{
    std::string s;
    
    while (begin != end)
    { s += *begin++ + str; }
    
    return s;
}

void TextParsley::close()
{
    if(_closed) return;
    
    std::ofstream f(_fname);
    
    std::string output;
    
    for(lineVec::const_iterator line = _file.begin(), lineEnd = _file.end();
        line != lineEnd;
        ++line)
    {
        for(wordVec::const_iterator word = line->begin(), wordEnd = line->end();
            word != wordEnd;
            ++word)
        {
            output += *word + " ";
        }
        
        output += "\n";
    }
    
    f << output;
    
    _file.clear();
    
    _closed = true;
}

void TextParsley::insertInFile(const std::string& str)
{
    _currLine = _file.insert(_currLine, split(str.begin(),str.end()));
}

void TextParsley::appendToFile(const std::string& str)
{
    _file.push_back(split(str.begin(), str.end()));
    
    toFileEnd();
}

std::string TextParsley::currLine()
{
    return join(_currLine->begin(), _currLine->end());
}

void TextParsley::eraseWord()
{
    _currWord = _currLine->erase(_currWord);
    
    if (_currWord == _currLine->end()) moveWord(-1);
}

void TextParsley::eraseLine()
{
    _currLine = _file.erase(_currLine);
    
    if (_currLine == _file.end()) moveLine(-1);
}

void TextParsley::replaceLine(const std::string &str)
{
    eraseLine();
    
    insertInFile(str);
}

void TextParsley::replaceWord(const std::string& str)
{
    eraseWord();
    
    insertInLine(str);
}

void TextParsley::insertInLine(const std::string& str)
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

void TextParsley::appendToLine(const std::string& str)
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

void TextParsley::open(const std::string& fname)
{
    std::ifstream file(fname);
    
    if (! file.good())
        throw FileOpenError();
    
    _fname = fname;
    
    _currLine = _file.begin();
    
    if (file.is_open())
    {
        std::string s;
        
        while (getline(file, s))
        {
            _file.push_back(split(s.begin(), s.end()));
        }
        
        _currLine = _file.begin();
        _currWord = _currLine->begin();
    }
}

std::vector<std::string> TextParsley::getAllWords()
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
                    if (! word->empty())
                    { vec.push_back(*word); }
                }
            }
        }
    }
    
    return vec;
}

std::vector<std::string> TextParsley::getAllLines()
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
                    if (! word->empty())
                    { lineStr += (*word); }
                }
                
                vec.push_back(lineStr);
            }
        }
    }
    
    return vec;
}

ParsleyNode * Parsley::parse(const std::string& fname)
{
    std::ifstream file(fname);
    
    if (! file.good() || ! file.is_open())
        throw FileOpenError();
    
    StrVec vec;
    std::string s,str;
    
    // grab first line to see if it has a header
    getline(file, s,'>');
    
    // go back in file
    file.unget();
    
    // get a char
    s += file.get();
    
    // see if it is an XML header
    bool hasHeader = _isHeader(s.begin() + 1, s.end() - 1);
    
    if (!hasHeader) str += s;
    
    while (getline(file,s)) str += s;
    
    vec = _parse(str.begin(), str.end());
    
    // _makeNodeTree needs a parent to be passed
    // for the recursion to work, so pass this
    // "pseudo-parent"
    ParsleyNode* pseudo = new ParsleyNode;
    
    _makeNodeTree(vec.begin(), vec.end(),pseudo);
    
    // the root is then the first child of this pseudo-parent
    ParsleyNode* ret = pseudo->firstChild;
    
    // remove pseudo-parent
    pseudo->firstChild = pseudo->lastChild = 0;
    delete pseudo;
    
    return ret;
}

bool Parsley::_isHeader(Str_cItr begin, Str_cItr end)
{
    std::string s = condense(begin, end);
    
    return *(s.begin()) == '?' && *(s.end() - 1) == '?' && s.substr(1,3) == "xml";
}

template <class T>
T Parsley::_lastNonSpace(T begin, T end)
{
    return (condense(begin, end)).end();
}

std::string ParsleyNode::getAttr(const std::string& attrKey)
{
    if (! findAttr(attrKey))
    { throw ParseError("Could not find attribute key: " + attrKey); }
    
    return attrs[attrKey];
}

void ParsleyNode::removeAttr(const std::string &key)
{
    if (! findAttr(key))
    { throw ParseError("Could not find attribute key: " + key); }
    
    attrs.erase(key);
}

ParsleyNode* ParsleyNode::getNthChild(unsigned int n) const
{
    ParsleyNode* node = firstChild;
    
    while(n-- && node)
    { node = firstChild->nextSibling; }
    
    return node;
}

ParsleyNode::NodeVec ParsleyNode::getElementsByTagName(const std::string& tagName)
{
    NodeVec vec;
    
    ParsleyNode * itr = firstChild;
    
    while (itr != 0)
    {
        if (itr->tag == tagName)
        { vec.push_back(itr); }
        
        itr = itr->nextSibling;
    }
    
    return vec;
}

void ParsleyNode::insertData(const std::string::size_type ind, const std::string& newData)
{
    if (ind < data.size()) data.insert(ind, newData);
    
    else throw ParseError("Index out ouf bounds!");
}

ParsleyNode::NodeVec ParsleyNode::getElementsByAttrName(const std::string& attrName)
{
    NodeVec vec;
    
    ParsleyNode * itr = firstChild;
    
    while (itr != 0)
    {
        if (itr->findAttr(attrName))
        { vec.push_back(itr); }
        
        itr = itr->nextSibling;
    }
    
    return vec;
}

bool ParsleyNode::insertChild(ParsleyNode* childOfThisNode, ParsleyNode * node)
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

bool ParsleyNode::removeChild(ParsleyNode* childOfThisNode)
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

Parsley::StrVec Parsley::_parse(Str_cItr begin, Str_cItr end)
{
    StrVec vec;
    
    std::string s;
    
    Str_cItr i = begin;
    Str_cItr j = i;
    
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

void ParsleyNode::prependChild(ParsleyNode *node)
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

void ParsleyNode::appendChild(ParsleyNode *node)
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

ParsleyNode::AttrMap Parsley::_getAttrs(Str_cItr begin, Str_cItr end) const
{
    ParsleyNode::AttrMap attrs;
    
    Str_cItr j;
    
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

bool Parsley::_isSelfClosing(Str_cItr begin, Str_cItr end) const
{
    while (end != begin && isspace(*end--));
    
    return *end == '/';
}

ParsleyNode * Parsley::_makeNode(Str_cItr begin, Str_cItr end, bool docHead)
{
    Str_cItr i,j;
    
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
    
    ParsleyNode::AttrMap attrs = _getAttrs(curr.begin(), curr.end());
    
    ParsleyNode * node = new ParsleyNode;
    
    node->tag = tag;
    node->attrs = attrs;
    
    node->selfClosed = _isSelfClosing(curr.begin(), curr.end());
    
    return node;
}

Parsley::StrVec_cItr Parsley::_makeNodeTree(StrVec_cItr itr, StrVec_cItr end, ParsleyNode * parent)
{
    if (std::find(itr->begin(), itr->end(), '<') == itr->end())
        parent->data += strip(itr->begin(), itr->end());
    
    else
    {
        ParsleyNode* node;
        
        node = _makeNode(itr->begin(), itr->end());
        
        // check if the node is a closing tag
        bool isClosing = (node->tag.end() - node->tag.begin() > 2 &&
                          *(node->tag.begin()) == '/');
        
        // if this is a closing tag, close the parent
        if (node->selfClosed)
            parent->selfClosed = true;
        
        else if (isClosing)
        {
            bool isClosingOfPar = std::equal(node->tag.begin() + 1,
                                             node->tag.end() , parent->tag.begin());
            
            if (isClosingOfPar)
            {
                if (parent->isClosed)
                    throw ParseError("Found duplicate closing tag for opening tag: " + parent->tag);
                
                parent->isClosed = true;
                
            } else
                throw ParseError("Found closing tag: " + node->tag + "that does not close current node!");
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

std::string Parsley::_nodeToString(const ParsleyNode *node, std::string indent, bool docHead) const
{
    std::string str;
    
    if (node != 0)
    {
        str = "<";
        
        if (docHead) str += "?";
        
        str += node->tag;
        
        for (ParsleyNode::AttrMap::const_iterator itr = node->attrs.begin(), end = node->attrs.end();
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

std::string Parsley::_treeToString(const ParsleyNode * root, std::string& str, std::string indent) const
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
        
        if (root->parent && !root->isLastChild())
            _treeToString(root->nextSibling, str,indent);
    }
    
    return str;
}

void Parsley::save(ParsleyNode* node,
                   const std::string& fname,
                   bool deleteTree,
                   bool addHeader)
{
    static std::string headerStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    std::string treeStr;
    
    if (addHeader)
    { treeStr = headerStr; }
    
    _treeToString(node,treeStr);
    
    std::ofstream outFile(fname);
    
    outFile << treeStr;
    
    outFile.close();
    
    if (deleteTree) delete node;
}

ParsleyNode::~ParsleyNode()
{
    while(hasChildren())
    { removeFirstChild(); }
}
