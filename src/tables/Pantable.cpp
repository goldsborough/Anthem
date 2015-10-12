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

PantableDatabase::PantableDatabase()
{
    // The pantable configuration file
    TextParsley textParser("/Users/petergoldsborough/Documents/Anthem/rsc/pantables/pantables.md");
    
    std::vector<std::string> config = textParser.getAllWords();
    
    // First item is the table length
    std::size_t tableLength = std::stoi(config.front());
    
    // So we can start from 0 when looping
    config.erase(config.begin());
    
    std::ifstream file;
    
    for (auto& name : config)
    {
        file.open("/Users/petergoldsborough/Documents/Anthem/rsc/pantables/" + name + ".table");
        
        if (! file)
        { throw FileOpenError("Error opening Pantable!"); }
        
        Sample* data = new Sample [tableLength];
        
        for (std::size_t i = 0; i < tableLength; ++i)
        {
            file >> data[i].left >> data[i].right;
        }
        
        _tables.push_back(std::make_shared<Pantable>(data, tableLength, name));
        
        file.close();
    }
}

std::shared_ptr<Pantable>& PantableDatabase::operator[] (index_t type)
{
    return _tables[type];
}

const std::shared_ptr<Pantable>& PantableDatabase::operator[] (index_t type) const
{
    return _tables[type];
}

PantableDatabase::index_t PantableDatabase::size() const
{
    return _tables.size();
}