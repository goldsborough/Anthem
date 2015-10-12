/*********************************************************************************************//*!
*
*  @file        EffectBlock.hpp
*
*  @author      Peter Goldsborough
*
*  @date        11/10/2014
*
*  @brief       The EffectBlock class.
*
*************************************************************************************************/

#ifndef Anthem_EffectBlock_hpp
#define Anthem_EffectBlock_hpp

#include <memory>

class EffectUnit;
class Delay;
class Echo;
class Reverb;
class Flanger;

/************************************************************************************************//*!
*
*  @brief       An interface class for effects.
*
*  @details     The EffectBlock class makes it easy to access all effects from one interface
*               rather than having to maintain an object for each effect. What is especially
*               useful is that the process() method processes a sample for the currently set
*               effect. The underlying effects can be accessed to modify their properties.
*
*************************************************************************************************/

class EffectBlock
{
    
public:
    
    /*! Currently available effects */
    enum EffectTypes
    {
        NONE,
        DELAY,
        ECHO,
        REVERB,
        FLANGER
    };
    
    /*************************************************************************//*!
    *
    *  @brief       Constructs an EffectBlock object.
    *
    *  @param       effectType The initial effectType, defaults to NONE.
    *
    ****************************************************************************/
    
    EffectBlock(unsigned short effectType = NONE);
    
    ~EffectBlock();
    
    /*************************************************************************//*!
    *
    *  @brief       Processes a sample for the currently selected effect.
    *
    *  @details     The call is forwarded to the process() method of the effect
    *               that the internal polymorphic pointer currently points to.
    *
    *  @param       sample The sample to process.
    *
    *  @throws      std::invalid_argument if current type is NONE.
    *
    ****************************************************************************/
    
    double process(double sample);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the current effect type.
    *
    *  @param       efffectType The new effectType.
    *
    *  @see         EffectTypes
    *
    ****************************************************************************/
    
    void setEffectType(unsigned short effectType);
    
    /*************************************************************************//*!
    *
    *  @brief       Returns the current effect type.
    *
    *  @see         EffectTypes
    *
    ****************************************************************************/
    
    unsigned short getEffectType() const;
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the active property of the EffectBlock.
    *
    *  @detail      An inactive EffectBlock will return the sample supplied to
    *               process() immediately, without any processing.
    *
    *  @param       state The new boolean state.
    *
    ****************************************************************************/
    
    void setActive(bool state);
    
    /*************************************************************************//*!
    *
    *  @brief       Whether or not the EffectBlock is currently active.
    *
    *  @return      Boolean, true if active, else false.
    *
    ****************************************************************************/
    
    bool isActive() const;
    
    /*! Returns a reference to the Delay object. */
    Delay& delay() const;
    
    /*! Returns a reference to the Echo object. */
    Echo& echo() const;
    
    /*! Returns a reference to the Reverb object. */
    Reverb& reverb() const;
    
    /*! Returns a reference to the Flanger object. */
    Flanger& flanger() const;
    
private:
    
    /*! Whether or not the EffectBlock is active. */
    bool _active;
    
    /*! The polymorphic pointer to the currently selected EffectUnit. */
    EffectUnit* _curr;
    
    /*! The currently selected EffectType. */
    unsigned short _effectType;
    
    /*! Smart pointer to Delay object. */
    std::unique_ptr<Delay> _delay;
    
    /*! Smart pointer to Echo object. */
    std::unique_ptr<Echo> _echo;
    
    /*! Smart pointer to Reverb object. */
    std::unique_ptr<Reverb> _reverb;
    
    /*! Smart pointer to Flanger object. */
    std::unique_ptr<Flanger> _flanger;
};

#endif
