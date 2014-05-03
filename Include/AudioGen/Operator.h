//
//  Operator.h
//  Synth
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__Operator__
#define __Synth__Operator__

#include "GenUnits.h"

class Oscillator;

class Operator : public AudioGenUnit
{
    
public:
    
    enum ModDocks
    {
        AMP,
        FREQ_SEMI,
        FREQ_CENT
    };
    
    Operator(const int md = 0, const double amp = 1);
    
    ~Operator();
    
    double tick();
    
    void setSemis(double semis);
    void setCents(double cents);
    
    void setWT (const int mode);
    
    void addNote(const double frq);
    
    void relNote(index_t ind);
    
private:
    
    typedef std::vector<Oscillator*> oscVec;
    
    oscVec _oscs;
    
    unsigned short _semis, _cents;
    
};

#endif /* defined(__Synth__Operator__) */
