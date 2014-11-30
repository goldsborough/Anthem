/********************************************************************************************//*!
*
*  @file        Pantable.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Pantable.hpp"
#include "Parsley.hpp"

#include <string>
#include <fstream>

void PantableDatabase::init()
{
    // The pantable configuration file
    TextParsley textParser("/Users/petergoldsborough/Documents/Anthem/rsc/pantables/pantables.md");
    
    std::vector<std::string> names = textParser.getAllWords();
    
    // First item is the table size
    index_t tableSize = std::stoi(names.front());
    
    // So we can start from 0 when looping
    names.erase(names.begin());
    
    std::ifstream file;
    
    for (index_t i = 0; i < size_; ++i)
    {
        file.open("/Users/petergoldsborough/Documents/Anthem/rsc/pantables/" + names[i] + ".table");
        
        if (! file)
        { throw FileOpenError("Error opening Pantable!"); }
        
        Sample* data = new Sample [tableSize];
        
        for (index_t j = 0; j < tableSize; ++j)
        {
            file >> data[j].left >> data[j].right;
        }
        
        tables_[i] = Pantable(data, tableSize, i, names[i]);
        
        file.close();
    }
}

Pantable& PantableDatabase::operator[] (index_t index)
{
    return tables_[index];
}

const Pantable& PantableDatabase::operator[] (index_t index) const
{
    return tables_[index];
}

PantableDatabase::index_t PantableDatabase::size() const
{
    return size_;
}