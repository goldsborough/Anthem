//
//  Mixer.h
//  Synth
//
//  Created by Peter Goldsborough on 28/03/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__Mixer__
#define __Synth__Mixer__

class XFadeUnit;
class DirectOutput;
class Wavefile;
class SampleQueue;

class Mixer
{
    
public:
    
    // Types of polyphony
    
    enum Priority
    {
        LAST,      // first voices are replaced by new ones
        FIRST,     // a key must be released to add a new one
        HIGHEST,   // lower pitches are released first
        LOWEST     // higher pitches are released first
    };
    
    Mixer(bool directOut = true,
          bool waveOut = false,
          const Priority& priort = LAST);
    
    ~Mixer();
    
    void setPriority(const Priority& prt) { _priority = prt; }
    
    void setMasterAmp(const double amp) { _masterAmp = amp; }
    
    void setPanning(const char pan);
    
    void processTick(const double smpl);
    
    void play();
    
    void stop();
    
private:
    
    double _sample;
    
    double _masterAmp;

    XFadeUnit * _pan;
    
    bool _stopped = true;
    
    bool _sendToWaveFile;
    bool _sendToDirectOutput;
    
    Priority _priority;
    
    SampleQueue  *  _sampleDataBuffer;
    
    DirectOutput * _directOut;
    
    Wavefile     * _waveOut;
};

#endif /* defined(__Synth__Mixer__) */
