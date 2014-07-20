//
//  Filter.h
//  Vibe
//
//  Created by Peter Goldsborough on 13/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Vibe__Filter__
#define __Vibe__Filter__

#include <vector>

// Direct Form II - Bi-quad filter http://goo.gl/XiA8jo
class Filter
{

public:
    
    enum Mode
    {
        LOW_PASS,
        HIGH_PASS,
        BAND_PASS,
        BAND_REJECT,
        ALL_PASS,
        PEAK,
        LOW_SHELF,
        HIGH_SHELF
    };
    
    Filter(const unsigned short& mode,
           const double& cutoff = 0.5,
           const double& bw = 0.5,
           const double& gain = 1);
    
    void process(double& sample);
    
    void setMode(const unsigned short& mode);
    
    void setCutoff(const double& cutoff);
    
    // Sets Q factor, but bandwidth is easier to understand
    // for the user
    void setBandWidth(const double& bw);
    
    void setGain(const short& gain);
    
private:
    
    void _calcCoefs();
    
    unsigned short _mode;
    
    double _cutoff;
    
    // The bandwidth or the Q factor. http://goo.gl/upz4ud
    double _q;
    
    double _gain;
    
    double _delayA;
    double _delayB;
    
    double _coefA1;
    double _coefA2;
    
    double _coefB0;
    double _coefB1;
    double _coefB2;
};

#endif /* defined(__Vibe__Filter__) */
