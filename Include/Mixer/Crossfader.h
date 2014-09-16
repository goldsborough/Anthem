/*********************************************************************************************//*!
*
*  @file        Units.h
*
*  @author      Peter Goldsborough
*
*  @date        15/09/2014
*
*  @brief       CrossfadeUnits and Crossfader.
*
*************************************************************************************************/

#ifndef __Anthem__Crossfade__
#define __Anthem__Crossfade__

#include "Units.h"
#include "Sample.h"

namespace CrossfadeTypes
{ enum Types { LINEAR, SINE, SQRT }; }

/*********************************************************************************************//*!
*
*  @brief       Basic crossfading unit.
*
*  @details     This class stores retrievable crossfade values in lookup tables. There are three
*               different tables, one for linear crossfading (values add up to 1), one for
*               crossfading with the sine function and one using the sqrt function. The last two
*               don't add up to 1, so there is an option to scale them.
*
*************************************************************************************************/

class CrossfadeUnit
{
    
public:
    
    CrossfadeUnit(unsigned short type = 0,
                  bool scalingEnabled = false,
                  unsigned short offset = 0);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Enables or disables scaling to 1 for sine and sqrt crossfading.
    *
    *  @param       scaling Boolean whether or not to scale.
    *
    *************************************************************************************************/
    
    void setScalingEnabled(bool scalingEnabled);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the CrossfadeUnit's type.
    *
    *  @param       type The new crossfading type, usually a member of the CrossfadeTypes enum.
    *
    *************************************************************************************************/
    
    void setType(unsigned short type);

    /*********************************************************************************************//*!
    *
    *  @brief       Sets the crossfade value for the two sides.
    *
    *  @param       value The crossfade value, between -100 (full left) and +100 (full right).
    *
    *************************************************************************************************/
    
    void setValue(short value);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns left value.
    *
    *************************************************************************************************/
    
    double left() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns right value.
    *
    *************************************************************************************************/
    
    double right() const;
    
protected:
    
    /*! Checks if scaling is necessary and returns scaled value if appropriate. Scaling is appropriate
        if scalign is enabled and if the type is either SQRT or SINE. */
    double _scale(const double& value) const;
    
    /*! Current index in the lookup table */
    unsigned short _index;
    
    /*! The crossfading type */
    unsigned short _type;
    
    /*! Boolean whether or not scaling is enabled*/
    bool _scalingEnabled;
    
    /*! Array of LookupTables for crossfading values */
    Sample _tables [3][201];
};

class Crossfader : public GenUnit
{
    
public:
    
    enum Docks { VALUE };
    
    enum FadeType { LINEAR, SINE, SQRT };
    
    Crossfader(unsigned short fadeType = 0,
               bool scale = false,
               GenUnit* left = 0,
               GenUnit* right = 0);
    
    ~Crossfader();
    
    double tick();
    
    void setValue(short value);
    
    void setFadeType(unsigned short fadeType, bool scale = false);
    
    void setLeftGenUnit(GenUnit* genUnit);
    
    void setRightGenUnit(GenUnit* genUnit);
    
private:
    
    void _initModDocks();
    
    unsigned short _fadeType;
    
    CrossfadeUnit* _xfade;
    
    GenUnit* _left;
    GenUnit* _right;
};

#endif /* defined(__Anthem__Crossfade__) */
