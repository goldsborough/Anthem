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
    operators_[A] = a;
    operators_[B] = b;
    operators_[C] = c;
    operators_[D] = d;
    
    setAlgorithm(alg);
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
        {
            operators_[A]->setMode(Operator::Mode::FM);
            operators_[B]->setMode(Operator::Mode::FM);
            operators_[C]->setMode(Operator::Mode::FM);
            operators_[D]->setMode(Operator::Mode::ADDITIVE);
            
            break;
        }
            
        case 4:
        case 5:
        case 7:
        {
            operators_[A]->setMode(Operator::Mode::FM);
            operators_[B]->setMode(Operator::Mode::FM);
            operators_[C]->setMode(Operator::Mode::ADDITIVE);
            operators_[D]->setMode(Operator::Mode::ADDITIVE);
            
            break;
        }
            
        case 8:
        case 9:
        case 10:
        {
            operators_[A]->setMode(Operator::Mode::FM);
            operators_[B]->setMode(Operator::Mode::ADDITIVE);
            operators_[C]->setMode(Operator::Mode::ADDITIVE);
            operators_[D]->setMode(Operator::Mode::ADDITIVE);
            
            break;
        }
            
        case 11:
        {
            operators_[A]->setMode(Operator::Mode::ADDITIVE);
            operators_[B]->setMode(Operator::Mode::ADDITIVE);
            operators_[C]->setMode(Operator::Mode::ADDITIVE);
            operators_[D]->setMode(Operator::Mode::ADDITIVE);
            
            break;
        }
    }
}

unsigned short FM::getAlgorithm() const
{
    return alg_;
}

double FM::tickIfActive_(index_t index)
{
    return (operators_[index]->isActive()) ? operators_[index]->tick() : 0;
}

double FM::modulate_(index_t carrier, double value)
{
    if (! operators_[carrier]->isActive()) return 0;
    
    operators_[carrier]->modulateFrequency(value);
    
    return operators_[carrier]->tick();
}

double FM::add_(index_t carrier, double value)
{
    if (! operators_[carrier]->isActive()) return 0;
    
    return operators_[carrier]->tick() + value;
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
            
            return modulate_(D, add_(C, aTick + bTick));
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
            
        case 11:
        default:
            return add_(D, add_(C, add_(B, aTick)));
    }
}
