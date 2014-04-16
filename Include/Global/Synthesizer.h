//
//  Synthesizer.h
//  Synth
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__Synthesizer__
#define __Synth__Synthesizer__

class Synthesizer
{
    
public:
    
    Synthesizer(unsigned int smplr = 44100, unsigned short wtLen = 4095);
    
    void process();
    
};

#endif /* defined(__Synth__Synthesizer__) */
