/********************************************************************************************//*!
*
*  @file        FM.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "FM.hpp"
#include "Operator.hpp"

#include <stdexcept>

FM::FM(Operator* a,
       Operator* b,
       Operator* c,
       Operator* d,
       index_t alg)
{
    ops_[A].op = a;
    ops_[B].op = b;
    ops_[C].op = c;
    ops_[D].op = d;
    
    setAlgorithm(alg);
}

void FM::setModes_(bool a, bool b, bool c, bool d)
{
    ops_[A].op->setMode(a);
    ops_[B].op->setMode(b);
    ops_[C].op->setMode(c);
    ops_[D].op->setMode(d);
}

void FM::setAlgorithm(unsigned short alg)
{
    if (alg > 11)
    { throw std::invalid_argument("Algorithm number must be between 0 and 11!"); }
    
    alg_ = alg;
    
    switch (alg_)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
            setModes_(0,0,0,1);
            break;
            
        case 4:
        case 5:
        case 7:
            setModes_(0, 0, 1,1);
            break;
            
        case 8:
        case 9:
        case 10:
            setModes_(0, 1, 1, 1);
            break;
            
        case 11:
            setModes_(1, 1, 1, 1);
    }
}

unsigned short FM::getAlgorithm() const
{
    return alg_;
}

void FM::setActive(index_t index, bool state)
{
    if (index > 3)
    { throw std::invalid_argument("Invalid index supplied! Must be between A (0) and B (3); "); }
    
    ops_[index].active = state;
}

bool  FM::isActive(index_t index) const
{
    if (index > 3)
    { throw std::invalid_argument("Invalid index supplied! Must be between A (0) and B (3); "); }
    
    return ops_[index].active;
}

double FM::tickIfActive_(index_t index)
{
    return (ops_[index].active) ? ops_[index].op->tick() : 0;
}

double FM::modulate_(index_t carrier, double value)
{
    if (! ops_[carrier].active) return 0;
    
    ops_[carrier].op->modulateFrequency(value);
    
    return ops_[carrier].op->tick();
}

double FM::add_(index_t carrier, double value)
{
    if (! ops_[carrier].active) return 0;
    
    return ops_[carrier].op->tick() + value;
}

double FM::tick()
{
    const double aTick = tickIfActive_(A);
    
    switch (alg_)
    {
        case 0:
            return modulate_(D, modulate_(C, modulate_(B, aTick)));
            
        case 1:
            return modulate_(D, modulate_(C, add_(B, aTick)));
            
        case 2:
            return modulate_(D, add_(C, modulate_(B, aTick)));
            
        case 3:
            return modulate_(D, modulate_(B, aTick) + modulate_(C, aTick));
            
        case 4:
        {
            double temp = modulate_(B, aTick);
            
            return modulate_(D, temp) + modulate_(C, temp);
        }
            
        case 5:
            return add_(D, modulate_(C, modulate_(B, aTick)));
            
        case 6:
        {
            double bTick = tickIfActive_(B);
            
            return modulate_(D, add_(B, aTick + bTick));
        }
            
        case 7:
        {
            double bTick = tickIfActive_(B);
            
            return modulate_(C, aTick) + modulate_(D, bTick);
        }
            
        case 8:
            return modulate_(D, aTick) + modulate_(C, aTick) + modulate_(B, aTick);
            
        case 9:
            return add_(D, add_(C, modulate_(B, aTick)));
            
        case 10:
            return add_(D, modulate_(C, aTick) + modulate_(B, aTick));
            
        default:
            return add_(D, add_(C, add_(B, aTick)));
    }
}
