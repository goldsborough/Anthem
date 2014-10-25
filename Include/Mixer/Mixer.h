/*********************************************************************************************//*!
*
*  @file        Mixer.h
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

#include "Units.h"

class CrossfadeUnit;
class DirectOutput;
class Wavefile;
class SampleBuffer;
class Sample;

/*********************************************************************************************//*!
*
*  @brief       Mixing unit for final audio/wavefile output.
*
*  @details     The Mixer class is the last station for any sample before direct audio output
*               or wavefile storage.
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
    *  @param       directOut Boolean, whether or not to enable direct audio output (to sound card).
    *
    *  @param       waveOut Boolean, whether or not to enable wavefile storage of samples when stop() called.
    *
    *  @param        amp An initial amplitude value between 0 and 1.
    *
    ************************************************************************************************************/
    
    Mixer(bool directOut = true,
          bool waveOut = false,
          double amp = 0.5);
    
    Mixer(const Mixer& other);
    
    Mixer& operator= (const Mixer& other);
    
    ~Mixer();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Processes a sample.
    *
    *  @param       sample A Sample object with left and right values.
    *
    *************************************************************************************************/
    
    void process(Sample sample);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Starts the audio output and begins sample storage for wavefile output.
    *
    *************************************************************************************************/
    
    void play();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Stops the audio output and stores samples to wave file, if enabled.
    *
    *************************************************************************************************/
    
    void stop();
    
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
    
    /*********************************************************************************************//*!
    *
    *  @brief       Method for enabling/disabling wavefile output.
    *
    *  @param       state Boolean, whether or not to enable wavefile output.
    *
    *************************************************************************************************/
    
    void setWavefileEnabled(bool state);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns whether or not wavefile output mode is enabled.
    *
    *  @return      Boolean, whether or not wavfile output enabled.
    *
    *************************************************************************************************/
    
    bool wavefileEnabled() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Enables/disables direct audio output.
    *
    *  @param       state The new state (enabled/disabled), boolean.
    *
    *************************************************************************************************/
    
    void setDirectOutputEnabled(bool state);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns whether or not directOutput is enabled.
    *
    *  @return      Boolean, true if enabled, false if not.
    *
    *************************************************************************************************/
    
    bool directOutputEnabled() const;
    
private:
    
    /*! The current master amplitude value */
    double masterAmp_;

    /*! CrossfadeUnit for panning */
    std::unique_ptr<CrossfadeUnit> pan_;
    
    /*! Whether or not audio output is stopped */
    bool stopped_;
    
    /*! Whether or not to send samples to wavefile after stop() ist called */
    bool wavefileEnabled_;
    
    /*! Whether or not to send samples to direct output */
    bool directOutputEnabled_;
    
    /*! Stores samples for wavefile output */
    std::unique_ptr<SampleBuffer>  sampleDataBuffer_;
    
    /*! Direct audio output object */
    std::unique_ptr<DirectOutput> directOut_;
    
    /*! Wavfile object */
    std::unique_ptr<Wavefile> waveOut_;
};

#endif /* defined(__Anthem__Mixer__) */
