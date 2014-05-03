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
    
    Mixer(bool directOut = true,
          bool waveOut = false);
    
    ~Mixer();
    
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
    
    SampleQueue  *  _sampleDataBuffer;
    
    DirectOutput * _directOut;
    
    Wavefile     * _waveOut;
};

#endif /* defined(__Synth__Mixer__) */
