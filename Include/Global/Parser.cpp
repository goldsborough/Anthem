//
//  TextParser.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 19/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Parser.h"
#include <stdexcept>

void TextParser::open(const std::string& fname)
{
    _file.open(fname);
    
    if (! _file.good() || ! _file.is_open())
        throw std::runtime_error("Error opening file!");
    
    std::string s;
    
    while (getline(_file, s))
        if (! s.empty() && ! isspace(s[0]) && s[0] != '#')
            _lines.push_back(s);
    
    _curr = _lines.begin();

}

std::vector<std::string> _split(const std::string& str)
{
    std::vector<std::string> vec;
    
    std::string::const_iterator i = str.begin();
    std::string::const_iterator j = i;
    
    while (i != str.end())
    {
        while (i != str.end() && isspace(*i)) ++i;
        
        j = i;
        
        while (j != str.end() && ! isspace(*j)) ++j;
        
        vec.push_back( std::string(i,j) );
        
        i = j;
    }
    
    return vec;
}

bool TextParser::readItem(std::string& str)
{
    if (! _file.is_open())
        throw std::runtime_error("No file has been opened!");
    
    if (_curr++ != _lines.end())
    {
        str = *_curr;
        return true;
    }
    
    return false;

}

std::vector<std::string> TextParser::readAll()
{
    if (! _file.is_open())
        throw std::runtime_error("No file has been opened!");
    
    return _lines;
}


void TextParser::close()
{
    if (! _file.is_open())
        throw std::runtime_error("No file has been opened!");
    
    _file.close();
}