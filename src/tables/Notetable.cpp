/********************************************************************************************//*!
*
*  @file        Notetable.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Notetable.hpp"

#include <fstream>

void Notetable::init()
{
    size_ = 128;
    
    data_ = new double [size_];
    
    std::ifstream file("/Users/petergoldsborough/Documents/Anthem/rsc/notes.table");
    
    for (index_t note = 0; note < size_; ++note)
    {
        file >> data_[note];
    }
}
