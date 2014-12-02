#ifndef Anthem_EffectBlock_hpp
#define Anthem_EffectBlock_hpp

#include <memory>

class EffectUnit;
class Delay;
class Echo;
class Reverb;

class EffectBlock
{
    
public:
    
    enum EffectTypes
    {
        NONE,
        DELAY,
        ECHO,
        REVERB
    };
    
    EffectBlock(unsigned short effectType = NONE);
    
    ~EffectBlock();
    
    double process(double sample);
    
    void setEffectType(unsigned short effectType);
    
    unsigned short getEffectType() const;
    
    void setActive(bool state);
    
    bool isActive() const;
    
    Delay& delay() const;
    
    Echo& echo() const;
    
    Reverb& reverb() const;
    
private:
    
    bool active_;
    
    EffectUnit* curr_;
    
    unsigned short effectType_;
    
    std::unique_ptr<Delay> delay_;
    
    std::unique_ptr<Echo> echo_;
    
    std::unique_ptr<Reverb> reverb_;
};

#endif
