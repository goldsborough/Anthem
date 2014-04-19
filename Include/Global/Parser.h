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
#include <fstream>

class TextParser
{
    
public:
    
    TextParser() { };
    TextParser(const std::string& fname) { open(fname); }
    
    void open(const std::string& fname);
    
    bool readItem(std::string&);
    
    std::vector<std::string> readAll();
    
    void close();
    
private:
    
    typedef std::vector<std::string> strVec;
    
    strVec _split(const std::string& str);
    
    strVec _lines;
    
    strVec::const_iterator _curr;
    
    std::ifstream _file;
};

#endif /* defined(__Vibe__mParser__) */
