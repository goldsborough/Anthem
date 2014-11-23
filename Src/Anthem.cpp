#include "Anthem.hpp"

Anthem::Anthem()
: fm(&operators[A],
     &operators[B],
     &operators[C],
     &operators[D])
{
    midi.init(this);
    
    audio.init(this);
}

Sample Anthem::tick()
{
    double tick;
    Sample sample;
    
    tick = operators[A].tick();
    
    sample = mixer.process(tick);
    
    return sample;
}

void Anthem::increment()
{
    for(unsigned short unit = A; unit < D; ++unit)
    {
        operators[unit].increment();
        lfos[unit].increment();
        //envelopes[unit].increment();
    }
}