#include "Anthem.hpp"

Anthem::Anthem()
: fm(&operators[A],
     &operators[B],
     &operators[C],
     &operators[D])
{
    midi.init(this);
    
    audio.init(this);
    
    audio.start();
}

Sample Anthem::tick_()
{
    double tick;
    Sample sample;
    
    tick = operators[A].tick();
    
    sample = mixer.process(tick);
    
    return sample;
}

void Anthem::increment_()
{
    operators[A].increment();
}