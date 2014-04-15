//
//  Operator.h
//  Synth
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__Oscillator__
#define __Synth__Oscillator__

#include "GenUnits.h"

class Envelope;
class Oscillator;
class Global;

class Operator : public AudioGenUnit
{
public:
    
    /*!
        @brief      Initializes mode
        @param      The waveform either as int or enum 'modes' member
        @throws     Error if out-of-range value
     */
    
    Operator(const Wavetable::Modes& md, const double& amp = 0.5);
    
    /*!
        @brief      Returns the current sample for each note
        @discussion Calls the tick() function on each voice
        @return     A pointer with _voice_number samples in them
    */
    
    int getVoiceNumber(){ return _voiceNumber; };
    
    void setAmp(const double& amp) { _Amp = amp; };
    
    void setFreqOffset(float offs);
    
    void addNote(const double frq);
    void remNote(int ind);
    
    double tick()
    {
        return 0;
    }
    
private:
    
    // vector?
    
    //std::vector<Oscillator> _voices;
    
    /*!
        @brief chicken soup
    */
    
    int _voiceNumber = 0;
    
    float _freqOffset = 0.0;
};

#endif /* defined(__Synth__Oscillator__) */
