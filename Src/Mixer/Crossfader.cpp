//
//  Crossfade.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Crossfader.h"
#include "Global.h"

void XFadeUnit::setValue(char value)
{
    if (value < -100 || value > 100)
        throw std::out_of_range("Cross-fade / Pan value must be between -100 and 100");
    
    _data = &_table[value + 100];
}

const double XFadeUnit::sqrt2 = sqrt(2) * 2;

XFadeSine::XFadeSine(bool scale, char initVal)
{
    for (int i = 0; i < 201; i++)
    {
        float val = (i - 100) / 100.0;
        
        float leftVal = sin((1 -  val) / 2 * PI/2);
        
        // Do not equal up to 1
        float rightVal = sin((1 + val) / 2 * PI/2);
        
        if (scale)
        {
            leftVal *= sqrt2;
            rightVal *= sqrt2;
        }
        
        _table[i].left = leftVal;
        _table[i].right = rightVal;
    }
    
    _data = &_table[initVal + 100];
}

XFadeLinear::XFadeLinear(char initVal)
{
    for (int i = 0; i < 201; i++)
    {
        float val = (i - 100) / 100.0;
        
        _table[i].left = (1 -  val) / 2;
        
        // Do not equal up to 1
        _table[i].right = (1 + val) / 2 ;
    }
    
    _data = &_table[initVal + 100];
}

XFadeSqrt::XFadeSqrt(bool scale, char initVal)
{
    for (int i = 0; i < 201; i++)
    {
        float val = (i - 100) / 100.0;
        
        float leftVal = sqrt( (1 -  val) / 2 );
        
        // Do not equal up to 1
        float rightVal = sqrt( (1 + val) / 2 );
        
        if (scale)
        {
            leftVal *= sqrt2;
            rightVal *= sqrt2;
        }
        
        _table[i].left = leftVal;
        _table[i].right = rightVal;
    }
    
    _data = &_table[initVal + 100];
}