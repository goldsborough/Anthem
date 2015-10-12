#include "Echo.hpp"

Echo::Echo(double delayLength,
           double decayTime,
           double decayRate,
           double feedbackLevel,
           double capacity)
: Delay(delayLength,decayTime,decayRate,feedbackLevel,capacity)
{ }

Echo::~Echo()
{
    
}

double Echo::process(double sample)
{
    double output = sample + Delay::process(sample);
    
    return _dryWet(sample, output);
}
