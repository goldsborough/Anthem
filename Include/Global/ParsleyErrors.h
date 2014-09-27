/*********************************************************************************************//*!
*
*  @file        ParsleyErrors.h
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Custom errors for Parsley.
*
*************************************************************************************************/

#ifndef __Parsley_Errors__
#define __Parsley_Errors__

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

#endif
