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
    ops_[A] = a;
    ops_[B] = b;
    ops_[C] = c;
    ops_[D] = d;
    
    setAlgorithm(alg);
}

void FM::setOperatorModes_(bool a, bool b, bool c, bool d)
{
    ops_[A]->setMode(a);
    ops_[B]->setMode(b);
    ops_[C]->setMode(c);
    ops_[D]->setMode(d);
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
            setOperatorModes_(0,0,0,1);
            break;
            
        case 4:
        case 5:
        case 7:
            setOperatorModes_(0, 0, 1,1);
            break;
            
        case 8:
        case 9:
        case 10:
            setOperatorModes_(0, 1, 1, 1);
            break;
            
        case 11:
            setOperatorModes_(1, 1, 1, 1);
    }
}

unsigned short FM::getAlgorithm() const
{
    return alg_;
}

double FM::tickIfActive_(index_t index)
{
    return (ops_[index]->isActive()) ? ops_[index]->tick() : 0;
}

double FM::modulate_(index_t carrier, double value)
{
    if (! ops_[carrier]->isActive()) return 0;
    
    ops_[carrier]->modulateFrequency(value);
    
    return ops_[carrier]->tick();
}

double FM::add_(index_t carrier, double value)
{
    if (! ops_[carrier]->isActive()) return 0;
    
    return ops_[carrier]->tick() + value;
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
