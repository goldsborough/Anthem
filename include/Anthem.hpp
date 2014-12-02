#ifndef Anthem_Anthem_hpp
#define Anthem_Anthem_hpp

#include "Global.hpp"
#include "Util.hpp"

#include "FM.hpp"
#include "Noise.hpp"
#include "Operator.hpp"

#include "Reverb.hpp"
#include "Delay.hpp"
#include "EffectBlock.hpp"
#include "Delay.hpp"
#include "Filter.hpp"

#include "AudioOutput.hpp"
#include "Midi.hpp"
#include "Mixer.hpp"

#include "Crossfader.hpp"
#include "Envelope.hpp"
#include "LFO.hpp"
#include "Macro.hpp"

#include <vector>

struct Anthem
{
    enum Units { A, B, C, D };
    
    typedef unsigned char note_t;
    
    Anthem();
    
    void setNote(note_t note, bool on);
    
    Operator operators [4];
    
    LFOUnit lfos [4];
    
    Filter filters [2];
    
    EffectBlock effects [2];
    
    Noise noise;
    
    Envelope envelopes [4];
    
    Macro macros [4];
    
    Crossfader crossfaders [4];
    
    FM fm;
    
    Mixer mixer;
    
    Midi midi;
    
    AudioOutput audio;
    
private:
    
    friend class AudioOutput;
    
    Sample tick_();
    
    void update_();
    
    note_t note_;
    
    bool active_;
    
};

#endif
