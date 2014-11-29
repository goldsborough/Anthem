#include "Anthem.hpp"

Anthem::Anthem()
: fm(&operators[A],
     &operators[B],
     &operators[C],
     &operators[D])
{
    notes_.reserve(8);
    
    midi.init(this);
    
    audio.init(this);
}

void Anthem::setNote(note_t note, bool on)
{
    if (on)
    {
        notes_.push_back(note);
    }
    
    else if (! notes_.empty())
    {
       notes_.erase(std::find(notes_.begin(), notes_.end(), note));
    }
}

Sample Anthem::tick_()
{
    double tick = 0;
    Sample sample;
    
    for (std::vector<note_t>::const_iterator itr = notes_.begin(), end = notes_.end();
         itr != end;
         ++itr)
    {
        operators[A].setNote(*itr);
        
        double temp = operators[A].tick();
        
        tick += temp;
    }
    
    sample = mixer.process(tick);
    
    return sample;
}

void Anthem::increment_()
{
    for(unsigned short unit = A; unit < D; ++unit)
    {
        operators[unit].increment();
        lfos[unit].increment();
        envelopes[unit].increment();
    }
}