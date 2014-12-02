#ifndef Anthem_Flanger_hpp
#define Anthem_Flanger_hpp

#include "Units.hpp"

class Delay;
class LFO;

/*! Doesn't work yet. */
class Flanger : public EffectUnit
{
public:
    
    Flanger(const double& center = 0.01,
            const double& depth = 0.01,
            const double& rate = 0.15,
            const double& feedback = 0);
    
    Flanger(const Flanger& other);
    
    Flanger& operator= (const Flanger& other);
    
    double process(double sample);
    
    void setFeedback(const double& feedback);
    
    void setCenter(const double& center);
    
    void setDepth(const double& depth);
    
    void setRate(const double& rate);
    
private:
    
    double center_;
    double depth_;
    double feedback_;
    
    std::unique_ptr<LFO> lfo_;
    std::unique_ptr<Delay> delay_;
};

#endif
