/*********************************************************************************************//*!
*
*  @file        Mixer.h
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Mixer class declaration.
*
*************************************************************************************************/

#ifndef __Anthem__Mixer__
#define __Anthem__Mixer__

class CrossfadeUnit;
class DirectOutput;
class Wavefile;
class SampleBuffer;
class Sample;

class Mixer
{
    
public:
    
    Mixer(bool directOut = true,
          bool waveOut = false);
    
    ~Mixer();
    
    void setMasterAmp(const double amp) { _masterAmp = amp; }
    
    void setPanning(const char pan);
    
    void process(Sample sample);
    
    void play();
    
    void stop();
    
private:
    
    double _masterAmp;

    CrossfadeUnit* _pan;
    
    bool _stopped = true;
    
    bool _sendToWaveFile;
    bool _sendToDirectOutput;
    
    SampleBuffer*  _sampleDataBuffer;
    
    DirectOutput* _directOut;
    
    Wavefile* _waveOut;
};

#endif /* defined(__Anthem__Mixer__) */
