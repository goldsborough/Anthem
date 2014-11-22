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
    
    operators[B].tick();
    operators[C].tick();
    operators[D].tick();
    
    lfos[A].lfos(A).tick();
    lfos[B].lfos(A).tick();
    lfos[C].lfos(A).tick();
    lfos[D].lfos(A).tick();
    
    tick = filters[A].process(tick);
    
    tick = filters[B].process(tick);
    
    tick = reverbs[A].process(tick);
    
    tick = reverbs[B].process(tick);
    
    tick = delays[A].process(tick);
    
    tick = delays[B].process(tick);
    
    tick = echos[A].process(tick);
    
    tick = echos[B].process(tick);
    
    sample = mixer.process(tick);
    
    return sample;
}

void Anthem::increment()
{
    for(unsigned short unit = A; unit < D; ++unit)
    {
        operators[unit].increment();
        lfos[unit].increment();
        envelopes[unit].increment();
    }
}