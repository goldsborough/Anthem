//
//  Errors.h
//  Vibe
//
//  Created by Peter Goldsborough on 20/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef Vibe_Errors_h
#define Vibe_Errors_h

#include <stdexcept>

struct FileOpenError : public std::runtime_error
{
    FileOpenError() : std::runtime_error("Error opening / reading file!") {}
    FileOpenError(std::string msg) : std::runtime_error(msg) {}
};

struct FileNotOpenError : public std::runtime_error
{
    FileNotOpenError() : std::runtime_error("No file has been opened!") {}
    FileNotOpenError(std::string msg) : std::runtime_error(msg) {}
};

struct ParseError : public std::runtime_error
{
    ParseError() : std::runtime_error("Error parsing file!") {}
    ParseError(std::string msg) : std::runtime_error(msg) {}
};

#endif
