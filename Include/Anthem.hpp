#ifndef Anthem_Anthem_hpp
#define Anthem_Anthem_hpp

#include "FM.hpp"
#include "Noise.hpp"
#include "Operator.hpp"
#include "Oscillator.hpp"

#include "Delay.hpp"
#include "Effects.hpp"
#include "Filter.hpp"

#include "Global.hpp"
#include "Sample.hpp"
#include "Util.hpp"
#include "Wavetable.hpp"
#include "WavetableDatabase.hpp"

#include "AudioOutput.hpp"
#include "Midi.hpp"
#include "Mixer.hpp"

#include "Crossfader.hpp"
#include "Envelope.hpp"
#include "LFO.hpp"
#include "Macro.hpp"

class Anthem
{
    
public:
    
    enum Units
    {
        A,
        B,
        C,
        D
    };
    
    Anthem();
    
    Operator operators [4];
    
    FM fm;
    
    Mixer mixer;
    
    Midi midi;
    
    AudioOutput audio;
    
private:
    
    friend class AudioOutput;
    
    Sample tick_();
    
    void increment_();
};

#endif
