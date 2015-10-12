/*********************************************************************************************//*!
*
*  @file        Parsley.hpp
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Parsley, the XML Parser.
*
*************************************************************************************************/

#ifndef __Parsley__
#define __Parsley__

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

struct FileOpenError : public std::runtime_error
{
    FileOpenError(std::string msg = "Error opening file!")
    : std::runtime_error(msg) {}
};

struct FileReadError : public std::runtime_error
{
    FileReadError(std::string msg = "Error reading from file!")
    : std::runtime_error(msg) {}
};

struct FileWriteError : public std::runtime_error
{
    FileWriteError(std::string msg = "Error writing to file!")
    : std::runtime_error(msg) {}
};

struct FileNotOpenError : public std::runtime_error
{
    FileNotOpenError(std::string msg = "No file has been opened!")
    : std::runtime_error(msg) {}
};

struct ParseError : public std::runtime_error
{
    ParseError(std::string msg = "Error parsing file!")
    : std::runtime_error(msg) {}
};

typedef std::string::const_iterator Str_cItr;
typedef std::vector<std::string>::iterator Vec_Itr;

/*************************************************************************//*!
*
*   @brief Class to open, navigate and edit a file.
*
*   @details TextParsley enables the user to open or create a new file
*            and navigate/iterate over the lines and the words of its
*            text. New text can be added to the file or to the individual
*            lines. Essentially, it is a more useful alternative to seekp,
*            seekg, tellp and tellg.
*
****************************************************************************/

class TextParsley
{
    
public:
    
    TextParsley()
    : _closed(0)
    { }
    
    /*************************************************************************//*!
    *
    *   @brief Constructs the text parser and reads/creates a file.
    *
    *   @details If the file "fname" is found and exists, it is read and
    *            parsed, else a new file called "fname" is created.
    *
    *   @param fname The name of the file to read or create.
    *
    ****************************************************************************/
    
    TextParsley(const std::string& fname)
    { open(fname); }
    
    ~TextParsley()
    { close(); }
    
    /*************************************************************************//*!
    *
    *   @brief Opens a file if it exists, else creates a new file, named fname.
    *
    *   @param fname The filename to open or to create.
    *
    ****************************************************************************/
    
    void open(const std::string& fname);
    
    
    /*************************************************************************//*!
    *
    *   @brief Closes and saves the file.
    *
    ****************************************************************************/
    
    void close();
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns the current word pointed to by the currWord pointer.
    *
    *   @return An std::string containing the current word.
    *
    ****************************************************************************/
    
    std::string currWord()
    { return *_currWord; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns the current line pointed to by the currLine pointer.
    *
    *   @return An std::string containing the current line.
    *
    ****************************************************************************/
    
    std::string currLine();
    
    /*************************************************************************//*!
    *
    *   @brief Appends an std::string to the current line.
    *
    *   @param str The std::string to append.
    *
    ****************************************************************************/
    
    void appendToLine(const std::string& str);
    
    /*************************************************************************//*!
    *
    *   @brief Appends an std::string to the end of the file.
    *
    *   @param str The std::string to append.
    *
    ****************************************************************************/
    
    void appendToFile(const std::string& str);
    
    /*************************************************************************//*!
    *
    *   @brief Inserts an std::string at the current position in the line.
    *
    *   @param str The std::string to insert.
    *
    ****************************************************************************/
    
    void insertInLine(const std::string& str);
    
    /*************************************************************************//*!
    *
    *   @brief Inserts a new line at the current position in the file.
    *
    *   @param str The std::string to insert.
    *
    ****************************************************************************/
    
    void insertInFile(const std::string& str);
    
    /*************************************************************************//*!
    *
    *   @brief Erases the current word.
    *
    ****************************************************************************/
    
    void eraseWord();
    
    
    /*************************************************************************//*!
    *
    *   @brief Erases the current line.
    *
    ****************************************************************************/
    
    void eraseLine();
    
    
    /*************************************************************************//*!
    *
    *   @brief Replaces the current word with a new one.
    *
    *   @param str The std::string with which to replace the current word.
    *
    ****************************************************************************/
    
    void replaceWord(const std::string& str);
    
    
    /*************************************************************************//*!
    *
    *   @brief Replaces the current line with a new one.
    *
    *   @param str The std::string with which to replace the current line.
    *
    ****************************************************************************/
    
    void replaceLine(const std::string& str);
    
    
    /*************************************************************************//*!
    *
    *   @brief Moves the word pointer to the beginning of the current line.
    *
    ****************************************************************************/
    
    void toLineStart()
    { _currWord = _currLine->begin(); }
    
    
    /*************************************************************************//*!
    *
    *   @brief Moves the line pointer to the beginning of the file.
    *
    ****************************************************************************/
    
    void toFileStart()
    {
        _currLine = _file.begin();
        _currWord = _currLine->begin();
    }
    
    
    /*************************************************************************//*!
    *
    *   @brief Moves the word pointer to the end of the current line.
    *
    ****************************************************************************/
    
    void toLineEnd()
    { _currWord = _currLine->end() - 1; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Moves the line pointer to the end of the file.
    *
    ****************************************************************************/
    
    void toFileEnd()
    {
        _currLine = _file.end() - 1;
        _currWord = _currLine->begin();
    }
    
    
    /*************************************************************************//*!
    *
    *   @brief Moves the word pointer __count__ places within the current line.
    *
    *   @param count The amount of places to move the pointer. Can be negative.
    *
    ****************************************************************************/
    
    void moveWord(int count)
    { std::advance(_currWord, count); }
    
    
    /*************************************************************************//*!
    *
    *   @brief Moves the line pointer within the file.
    *
    *   @param count The amount of places to move the pointer. Can be negative.
    *
    ****************************************************************************/
    
    void moveLine(int count)
    { std::advance(_currLine, count); }
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns all text of the current line.
    *
    *   @return All text of the current line.
    *
    ****************************************************************************/
    
    std::vector<std::string> getAllWords();
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns all text of the file.
    *
    *   @return All text of the file.
    *
    ****************************************************************************/
    std::vector<std::string> getAllLines();
    
private:
    
    typedef std::vector<std::string> wordVec;
    typedef std::vector<std::vector<std::string>> lineVec;
    
    typedef wordVec::iterator wordItr;
    typedef lineVec::iterator lineItr;
    
    lineVec _file;
    
    wordItr _currWord;
    lineItr _currLine;
    
    std::string _fname;
    
    bool _closed;
};

/*************************************************************************//*!
*
*   @brief An ParsleyNode is a single node in an XML document.
*
*   @details The ParsleyNode class provides methods and interfaces to traverse
*            and edit an entire XML document. Every node except for the
*            top-level node has a parent, children and sibling and has
*            many methods to work with and navigate these.
*
****************************************************************************/

class ParsleyNode
{
    
public:
    
    typedef std::vector<ParsleyNode*> NodeVec;
    typedef NodeVec::const_iterator NodeVec_cItr;
    
    ParsleyNode()
    { }
    
    /*************************************************************************//*!
    *
    *   @brief Constructor, takes the tag name of the XML node as parameter.
    *
    *   @param tagName The tag name of the XML Node.
    *
    ****************************************************************************/
    
    ParsleyNode(const std::string& tagName)
    : tag(tagName)
    { }
    
    ~ParsleyNode();
    
    /*************************************************************************//*!
    *
    *   @brief Returns the value of the attribute with the key attrKey.
    *
    *   @param attrKey The key of the attribute.
    *
    *   @throws ParseError if the key is not found.
    *
    *   @return The corresponding value of the key, if existent.
    *
    ****************************************************************************/
    
    std::string getAttr(const std::string& attrKey);
    
    
    /*************************************************************************//*!
    *
    *   @brief Searches for an attribute in the XML node.
    *
    *   @param key The key of the attribute to find.
    *
    *   @return True if the attribute was found, else false.
    *
    ****************************************************************************/
    
    bool findAttr(const std::string& key)
    { return attrs.find(key) != attrs.end(); }
    
    
    /*************************************************************************//*!
    *
    *   @brief Adds a new attribute to the node.
    *
    *   @details Use this method to add a new attribute, setAttr() will not
    *            automatically add new attribute if the one passed is
    *            non-existent.
    *
    *   @param key The key of the new attribute.
    *
    *   @param value The value of the new attribute.
    *
    *   @see setAttr()
    *
    ****************************************************************************/
    
    void addAttr(const std::string& key, const std::string& val)
    { attrs[key] = val; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Sets a new value for an attribute.
    *
    *   @details This method does not add new attributes if the passed key is
    *            not found. Use addAttr() to add a new attribute.
    *
    *   @param value The new value of the attribute.
    *
    *   @param key The key of the attribute to change.
    *
    *   @see addAttr()
    *
    ****************************************************************************/
    
    void setAttr(const std::string& key, const std::string& val)
    { if (findAttr(key)) addAttr(key, val); }
    
    
    /*************************************************************************//*!
    *
    *   @brief Removes an attribute.
    *
    *   @param key The key of the attribute to remove.
    *
    *   @throws ParseError if the key is not found.
    *
    ****************************************************************************/
    
    void removeAttr(const std::string& key);
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns the node's tag name.
    *
    *   @return The node's tag name.
    *
    ****************************************************************************/
    
    std::string getTag() { return tag; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns the node's tag name.
    *
    *   @return The node's tag name.
    *
    ****************************************************************************/
    
    void setTag(const std::string& name) { tag = name; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns a vector of nodes with the tag name tagName.
    *
    *   @param tagName The tag name to search for.
    *
    *   @return A vector of nodes. Might be empty.
    *
    ****************************************************************************/
    
    NodeVec getElementsByTagName(const std::string& tagName);
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns a vector of nodes with the attribute key attrName.
    *
    *   @param attrName The attribute key to search for.
    *
    *   @return A vector of nodes. Might be empty.
    *
    ****************************************************************************/
    
    NodeVec getElementsByAttrName(const std::string& attrName);
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns the data of the node (the text between the tags).
    *
    *   @return The data of the node.
    *
    ****************************************************************************/
    
    std::string getData() const { return data; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns the length of the node's data (the text between the tags).
    *
    *   @return The node's data's length.
    *
    ****************************************************************************/
    
    std::string::size_type getDataLength() const { return data.size(); }
    
    
    /*************************************************************************//*!
    *
    *   @brief Whether the node has data (text between the tags).
    *
    *   @return True if the node has data, else fals.
    *
    ****************************************************************************/
    
    bool hasData() const { return ! data.empty(); }
    
    
    /*************************************************************************//*!
    *
    *   @brief Sets the node's data (the text between the tags).
    *
    *   @param newData The std::string that becomes the new data.
    *
    ****************************************************************************/
    
    void setData(const std::string& newData) { data = newData; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Appends to the node's data (the text between the tags).
    *
    *   @param newData The std::string to append to the node's data.
    *
    ****************************************************************************/
    
    void appendData(const std::string& newData) {data += newData; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Inserts new data to the node's data (the text between the tags).
    *
    *   @param ind The position where to insert the new data.
    *
    *   @param newData The new data to insert.
    *
    *   @throws ParseError if the index is out of bounds.
    *
    ****************************************************************************/
    
    void insertData(const std::string::size_type ind, const std::string& newData);
    
    
    /*************************************************************************//*!
    *
    *   @brief Replaces any data matching oldData with newData.
    *
    *   @param oldData The data to replace.
    *
    *   @param newData The new data to insert instead of oldData.
    *
    ****************************************************************************/
    
    void replaceData(const std::string& oldData, const std::string& newData)
    { data.replace(oldData.begin(), oldData.end(), newData); }
    
    /*************************************************************************//*!
    *
    *   @brief Splits the data at ind and returns the data starting at that point.
    *
    *   @param ind The position where to split.
    *
    *   @return The split data, starting at ind.
    *
    ****************************************************************************/
    
    std::string splitData(const std::string::size_type ind)
    { return data.substr(ind); }
    
    /*************************************************************************//*!
    *
    *   @brief Returns a substring of the node's data.
    *
    *   @param ind The starting position of where to substring.
    *
    *   @param count The number of characters to substring starting at ind.
    *
    ****************************************************************************/
    
    std::string substringData(const std::string::size_type ind,
                              std::string::size_type count = std::string::npos)
    { return data.substr(ind,count); }
    
    
    /*************************************************************************//*!
    *
    *   @brief Deletes the node's data.
    *
    ****************************************************************************/
    
    void deleteData() { data.erase(); }
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns a pointer to the node's parent.
    *
    *   @return Returns an ParsleyNode* if the parent exists, else returns NULL.
    *
    ****************************************************************************/
    
    ParsleyNode* getParent() const { return parent; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns a pointer to the node's previous sibling.
    *
    *   @return Returns an ParsleyNode* if the previous sibling exists, else returns NULL.
    *
    ****************************************************************************/
    
    ParsleyNode* getPrevSibling() const { return prevSibling; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns a pointer to the node's next sibling.
    *
    *   @return Returns an ParsleyNode* if the next sibling exists, else returns NULL.
    *
    ****************************************************************************/
    
    ParsleyNode* getNextSibling() const { return nextSibling; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns a pointer to the node's first child, if existent.
    *
    *   @return Returns an ParsleyNode* if a first child exists, else returns NULL.
    *
    ****************************************************************************/
    
    ParsleyNode* getFirstChild() const { return firstChild; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns a pointer to the node's last child, if existent.
    *
    *   @return Returns an ParsleyNode* if a last child exists, else returns NULL.
    *
    ****************************************************************************/
    
    ParsleyNode* getLastChild() const { return lastChild; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Returns a pointer to the node's nth child, if existent.
    *
    *   @return Returns an ParsleyNode* if nth child exists, else returns NULL.
    *
    ****************************************************************************/
    
    ParsleyNode* getNthChild(unsigned int n) const;
    
    
    /*************************************************************************//*!
    *
    *   @brief Whether this node is the parent's last child.
    *
    *   @return Returns true if this node is the parent's last child, else false.
    *
    ****************************************************************************/
    
    bool isLastChild() const
    { return parent->lastChild != 0 && this == parent->lastChild; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Whether this node is the parent's first child.
    *
    *   @return Returns true if this node is the parent's first child, else false.
    *
    ****************************************************************************/
    
    bool isFirstChild() const
    { return parent->lastChild != 0 && this == parent->firstChild; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Whether this node has children.
    *
    *   @return Returns true if this node has children, else false.
    *
    ****************************************************************************/
    
    bool hasChildren() const { return firstChild != 0 && lastChild != 0; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Whether this node has a parent.
    *
    *   @return Returns true if this node has a parent, else false.
    *
    ****************************************************************************/
    
    bool hasParent() const { return parent; }
    
    
    /*************************************************************************//*!
    *
    *   @brief Insert a new child at position 0.
    *
    *   @param node The new node which will become this node's first child.
    *
    ****************************************************************************/
    
    void prependChild(ParsleyNode * node);
    
    
    /*************************************************************************//*!
    *
    *   @brief Append a new child.
    *
    *   @param node The new node which will become this node's last child.
    *
    ****************************************************************************/
    
    void appendChild(ParsleyNode* node);
    
    
    /*************************************************************************//*!
    *
    *   @brief Insert a new child at the position of childOfThisNode.
    *
    *   @param childOfThisNode A child of this node, at the position of
    *          which to insert the new node.
    *
    *   @param node The new node to insert.
    *
    *   @return Returns true if successful, else false (e.g. child wasn't of this node)
    *
    ****************************************************************************/
    
    bool insertChild(ParsleyNode* childOfThisNode, ParsleyNode * node);
    
    
    /*************************************************************************//*!
    *
    *   @brief Removes a child.
    *
    *   @param childOfThisNode. The child of this node which is to be removed.
    *
    ****************************************************************************/
    
    bool removeChild(ParsleyNode* childOfThisNode);
    
    
    /*************************************************************************//*!
    *
    *   @brief Removes the node's first child.
    *
    ****************************************************************************/
    
    void removeFirstChild() { removeChild(firstChild); }
    
    
    /*************************************************************************//*!
    *
    *   @brief Removes the node's last child.
    *
    ****************************************************************************/
    
    void removeLastChild() { removeChild(lastChild); }
    
private:
    
    friend class Parsley;
    
    typedef std::map<std::string,std::string> AttrMap;
    
    AttrMap attrs;
    
    std::string tag;
    
    std::string data;
    
    ParsleyNode* parent       = 0;
    
    ParsleyNode* prevSibling  = 0;
    ParsleyNode* nextSibling  = 0;
    
    ParsleyNode* firstChild   = 0;
    ParsleyNode* lastChild    = 0;
    
    bool isClosed = false;
    bool selfClosed = false;
};

/*************************************************************************//*!
*
*   @brief The Parsley class parses and manages XML documents and nodes.
*
****************************************************************************/

class Parsley
{
    
public:
    
    /*************************************************************************//*!
    *
    *   @brief Method to manually open and parse an existing XML document.
    *
    ****************************************************************************/
    
    ParsleyNode * parse(const std::string& fname);
    
    /*************************************************************************//*!
    *
    *   @brief Closes and saves the XML file.
    *
    ****************************************************************************/
    
    void save(ParsleyNode* node,
              const std::string& fname,
              bool deleteTree = true,
              bool addHeader = true);
    
private:
    
    typedef std::vector<std::string> StrVec;
    
    typedef StrVec::const_iterator StrVec_cItr;
    
    ParsleyNode * _makeNode(Str_cItr begin, Str_cItr end, bool docHead = false);
    
    StrVec_cItr _makeNodeTree(StrVec_cItr itr, StrVec_cItr end, ParsleyNode * parent);
    
    std::vector<std::string> _parse(Str_cItr begin, Str_cItr end);
    
    bool _isSelfClosing(Str_cItr begin, Str_cItr end) const;
    
    bool _isTag(Str_cItr begin, Str_cItr end) const;
    
    bool _isComment(Str_cItr begin, Str_cItr end) const;
    
    bool _isHeader(Str_cItr begin, Str_cItr end);
    
    ParsleyNode::AttrMap _getAttrs(Str_cItr begin, Str_cItr end) const;
    
    template <class T>
    T _lastNonSpace(T begin, T end);
    
    std::string _nodeToString(const ParsleyNode * node,
                              std::string indent = "",
                              bool docHead = false) const;
    
    std::string _treeToString(const ParsleyNode * root,
                              std::string& str,
                              std::string indent = "") const;
};


#endif /* defined(__Parsley__) */
