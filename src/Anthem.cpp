/********************************************************************************************//*!
*
*  @file        Anthem.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Anthem.hpp"

Anthem::Anthem()
: fm(&operators[A],
     &operators[B],
     &operators[C],
     &operators[D]),
  active_(false),
  count_(0)
{
    operators[A].setActive(true);
    
    midi.init(this);
    
    audio.init(this);
}


void Anthem::setNote(note_t note, bool on)
{
    if (on)
    {
        for (unsigned short i = A; i <= D; ++i)
        {
            operators[i].setNote(note);
        }
        
        active_ = true;
    }
    
    else
    {
        for (unsigned short i = A; i <= D; ++i)
        {
            operators[i].setSilent();
        }
        
        for (unsigned short i = A; i <= D; ++i)
        {
            if (envelopes[i].isActive())
            {
                envelopes[i].reset();
            }
        }
        
        active_ = false;
    }
}

Sample Anthem::tick_()
{
    ++count_;
    
    double tick = 0;
    
    if (operators[A].isActive())
    {
        tick = operators[A].tick();
    }
    
    if (noise.isActive())
    {
        tick += noise.tick();
    }
    
    for (unsigned short i = A; i <= B; ++i)
    {
        if (filters[i].isActive())
        {
            tick = filters[i].process(tick);
        }
        
        if (effects[i].isActive())
        {
            tick = effects[i].process(tick);
        }
    }
    
    return mixer.process(tick);
}

void Anthem::update_()
{
    for(unsigned short unit = A; unit < D; ++unit)
    {
        operators[unit].update();
        lfos[unit].update();
        envelopes[unit].update();
    }
    
    noise.update();
}