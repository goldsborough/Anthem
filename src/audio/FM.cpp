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
    _operators[A] = a;
    _operators[B] = b;
    _operators[C] = c;
    _operators[D] = d;
    
    setAlgorithm(alg);
}

void FM::setAlgorithm(unsigned short alg)
{
    if (alg > 11)
    { throw std::invalid_argument("Algorithm number must be between 0 and 11!"); }
    
    _alg = alg;
    
    switch (_alg)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
        {
            _operators[A]->setMode(Operator::Mode::FM);
            _operators[B]->setMode(Operator::Mode::FM);
            _operators[C]->setMode(Operator::Mode::FM);
            _operators[D]->setMode(Operator::Mode::ADDITIVE);
            
            break;
        }
            
        case 4:
        case 5:
        case 7:
        {
            _operators[A]->setMode(Operator::Mode::FM);
            _operators[B]->setMode(Operator::Mode::FM);
            _operators[C]->setMode(Operator::Mode::ADDITIVE);
            _operators[D]->setMode(Operator::Mode::ADDITIVE);
            
            break;
        }
            
        case 8:
        case 9:
        case 10:
        {
            _operators[A]->setMode(Operator::Mode::FM);
            _operators[B]->setMode(Operator::Mode::ADDITIVE);
            _operators[C]->setMode(Operator::Mode::ADDITIVE);
            _operators[D]->setMode(Operator::Mode::ADDITIVE);
            
            break;
        }
            
        case 11:
        {
            _operators[A]->setMode(Operator::Mode::ADDITIVE);
            _operators[B]->setMode(Operator::Mode::ADDITIVE);
            _operators[C]->setMode(Operator::Mode::ADDITIVE);
            _operators[D]->setMode(Operator::Mode::ADDITIVE);
            
            break;
        }
    }
}

unsigned short FM::getAlgorithm() const
{
    return _alg;
}

double FM::_tickIfActive(index_t index)
{
    return (_operators[index]->isActive()) ? _operators[index]->tick() : 0;
}

double FM::_modulate(index_t carrier, double value)
{
    if (! _operators[carrier]->isActive()) return 0;
    
    _operators[carrier]->modulateFrequency(value);
    
    return _operators[carrier]->tick();
}

double FM::_add(index_t carrier, double value)
{
    if (! _operators[carrier]->isActive()) return 0;
    
    return _operators[carrier]->tick() + value;
}

double FM::tick()
{
    const double aTick = _tickIfActive(A);
    
    switch (_alg)
    {
        case 0:
            return _modulate(D, _modulate(C, _modulate(B, aTick)));
            
        case 1:
            return _modulate(D, _modulate(C, _add(B, aTick)));
            
        case 2:
            return _modulate(D, _add(C, _modulate(B, aTick)));
            
        case 3:
            return _modulate(D, _modulate(B, aTick) + _modulate(C, aTick));
            
        case 4:
        {
            double temp = _modulate(B, aTick);
            
            return _modulate(D, temp) + _modulate(C, temp);
        }
            
        case 5:
            return _add(D, _modulate(C, _modulate(B, aTick)));
            
        case 6:
        {
            double bTick = _tickIfActive(B);
            
            return _modulate(D, _add(C, aTick + bTick));
        }
            
        case 7:
        {
            double bTick = _tickIfActive(B);
            
            return _modulate(C, aTick) + _modulate(D, bTick);
        }
            
        case 8:
            return _modulate(D, aTick) + _modulate(C, aTick) + _modulate(B, aTick);
            
        case 9:
            return _add(D, _add(C, _modulate(B, aTick)));
            
        case 10:
            return _add(D, _modulate(C, aTick) + _modulate(B, aTick));
            
        case 11:
        default:
            return _add(D, _add(C, _add(B, aTick)));
    }
}
