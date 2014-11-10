//
//  FM.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 08/09/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "FM.h"
#include "Operator.h"

#include <stdexcept>

FM::FM(index_t alg,
       Operator* a,
       Operator* b,
       Operator* c,
       Operator* d)
: alg_(alg)
{
    ops_[A] = a;
    ops_[B] = b;
    ops_[C] = c;
    ops_[D] = d;
}

void FM::setAlgorithm(unsigned short alg)
{
    if (alg > 11)
    { throw std::invalid_argument("Algorithm number must be between 0 and 11!"); }
    
    alg_ = alg;
}

unsigned short FM::getAlgorithm() const
{
    return alg_;
}

void FM::setOperator(index_t index, Operator* op)
{
    if (index > 3)
    { throw std::invalid_argument("Invalid index! Must be between 0 and 3!"); }
    
    ops_[index] = op;
}

Operator* FM::getOperator(index_t index) const
{
    if (index > 3)
    { throw std::invalid_argument("Invalid index! Must be between 0 and 3!"); }
    
    return ops_[index];
}

double FM::modulate_(index_t carrier, double value)
{
    if (! ops_[carrier]) return value;
    
    ops_[carrier]->modulateFrequency(value);
    
    return ops_[carrier]->tick();
}

double FM::add_(index_t left, index_t right)
{
    double leftTick = (ops_[left]) ? ops_[left]->tick() : 0;
    
    double rightTick = (ops_[right]) ? ops_[right]->tick() : 0;
    
    return leftTick + rightTick;
}

double FM::tick()
{
    const double aTick = (ops_[A]) ? ops_[A]->tick() : 0;
    
    double temp;
    
    switch (alg_)
    {
        case 0:
            return modulate_(D, modulate_(C, modulate_(B, aTick)));
            
        case 1:
            return modulate_(D, modulate_(C, add_(A, B)));
            
        case 2:
            return modulate_(D, add_(C, modulate_(B, aTick)));
            
        case 3:
            return modulate_(D, add_(modulate_(B, aTick), modulate_(C, aTick)));
            
        case 4:
        {
            temp = modulate_(B, aTick);
            
            return add_(modulate_(D, temp), modulate_(C, temp));
        }
            
        case 5:
            return add_(D, modulate_(C, modulate_(B, aTick)));
            
        case 6:
            return modulate_(D, add_(C, add_(B, A)));
            
        case 7:
        {
            temp = add_(A, B);
            
            return add_(modulate_(C, temp), modulate_(D, temp));
        }
            
        case 8:
            return add_(modulate_(D, aTick), add_(modulate_(C, aTick), modulate_(B, aTick)));
            
        case 9:
            return add_(D, add_(C, modulate_(B, aTick)));
            
        case 10:
            return add_(D, add_(modulate_(C, aTick), modulate_(B, aTick)));
            
        default:
            return add_(D, add_(C, add_(B, A)));
    }
}
