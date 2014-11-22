/*********************************************************************************************//*!
*
*  @file        Mixer.hpp
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Mixer class declaration.
*
*************************************************************************************************/

#ifndef __Anthem__Mixer__
#define __Anthem__Mixer__

#include "Units.hpp"
#include "Wavefile.hpp"

class CrossfadeUnit;
class Sample;

/*********************************************************************************************//*!
*
*  @brief       Mixing unit for final mastering and recording.
*
*  @details     The Mixer class is the last unit samples go to before audio output. Samples
*               are attenuated with a master amplitude as well as panning values. Additionally
*               the mixer can record samples for wavefile output.
*                                                                                                
*************************************************************************************************/

class Mixer : public Unit
{
    
public:
    
    /*! Available ModDocks */
    enum Docks
    {
        MASTER_AMP,
        PAN
    };

    /********************************************************************************************************//*!
    *
    *  @brief       Constructs a Mixer object.
    *
    *  @param        amp An initial amplitude value between 0 and 1.
    *
    ************************************************************************************************************/
    
    Mixer(double amp = 0.5);
    
    Mixer(const Mixer& other);
    
    Mixer& operator= (const Mixer& other);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Processes a sample.
    *
    *  @param       sample A sample of type double.
    *
    *  @return      A Sample object ready for audio output.
    *
    *************************************************************************************************/
    
    Sample process(double sample);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Begins audio sample storage for wavefile output.
    *
    *************************************************************************************************/
    
    void startRecording();
    
    /************************************************************************************************//*!
    *
    *  @brief       Stops recording and flushes the samples stored so far by the wavefile object.
    *
    ****************************************************************************************************/
    
    void stopRecording();
    
    /************************************************************************************************//*!
    *
    *  @brief       Stops recording and writes the audio samples stored in the Wavefile object to disk.
    *
    ****************************************************************************************************/
    
    void saveRecording();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Whether or not the mixer is currently recording samples.
    *
    *************************************************************************************************/
    
    bool isRecording() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the master amplitude, the final attenuation value before output.
    *
    *  @param       amp The new amplitude value, between 0 and 1.
    *
    *************************************************************************************************/
    
    void setMasterAmp(double amp);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the master amplitude.
    *
    *  @return      The master amplitude, between 0 and 1.
    *
    *************************************************************************************************/
    
    double getMasterAmp() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the panning value.
    *
    *  @param       pan The new panning value.
    *
    *************************************************************************************************/
    
    void setPanValue(short pan);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the current panning value.
    *
    *  @return      The current panning value, between -100 and +100.
    *
    *************************************************************************************************/
    
    short getPanValue() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the panning type.
    *
    *  @param       type The panning type, usually a member of CrossfadeTypes.
    *
    *************************************************************************************************/
    
    void setPanType(unsigned short type);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the current panning type.
    *
    *  @return      unsigned short value, compare with CrossfadeTypes.
    *
    *************************************************************************************************/
    
    unsigned short getPanType() const;
    
private:
    
    /*! The current master amplitude value */
    double masterAmp_;
    
    /*! Whether or not the mixer is currently recording */
    bool recording_;
    
    /*! CrossfadeUnit for panning */
    std::unique_ptr<CrossfadeUnit> pan_;
    
    /*! Wavfile object */
    Wavefile wavefile_;
};

#endif /* defined(__Anthem__Mixer__) */
