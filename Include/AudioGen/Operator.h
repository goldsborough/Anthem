//
//  Operator.h
//  Anthem
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__Operator__
#define __Anthem__Operator__

#include "Units.h"

class Oscillator;

class Operator : GenUnit
{
    
public:
    
    enum DOCKS
    {
        AMP,
        FREQ_SEMI,
        FREQ_CENT
    };
    
    Operator(const short& wt = 0, const double& amp = 1);
    
    ~Operator();
    
    double tick();
    
    void setSemis(const double& semis);
    void setCents(const double& cents);
    
    void setWavetable(const short& wt);
    
    void addNote(const double& frq);
    
    void relNote(const unsigned short& ind);
    
private:
    
    typedef std::vector<Oscillator*> oscVec;
    
    oscVec _oscs;
    
    unsigned short _semis;
    unsigned short _cents;
    
    unsigned int _wavetableId;
};

#endif /* defined(__Anthem__Operator__) */
