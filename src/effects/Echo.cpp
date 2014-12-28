#include "Echo.hpp"

Echo::Echo(double delayLen,
           double decayTime,
           double decayRate,
           double feedbackLevel,
           double capacity)
: Delay(delayLen,decayTime,decayRate,feedbackLevel,capacity)
{ }

Echo::~Echo()
{
    
}

double Echo::process(double sample)
{
    double output = sample + Delay::process(sample);
    
    return dryWet_(sample, output);
}
