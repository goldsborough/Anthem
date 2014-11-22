#include "Anthem.hpp"

Anthem::Anthem()
: A(WavetableDB::SINE,440,1,1)
{
    midi.init(this);
    
    audio.init(this);
    
    audio.start();
}

Sample Anthem::tick_()
{
    double tick;
    Sample sample;
    
    tick = A.tick();
    
    sample = mixer.process(tick);
    
    return sample;
}

void Anthem::increment_()
{
    A.increment();
}