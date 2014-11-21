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

class CrossfadeUnit;
class AudioOutput;
class Wavefile;
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
    *  @param       audioOut Boolean, whether or not to enable direct audio output (to sound card).
    *
    *  @param       waveOut Boolean, whether or not to enable wavefile storage of samples when stop() called.
    *
    *  @param        amp An initial amplitude value between 0 and 1.
    *
    ************************************************************************************************************/
    
    Mixer(bool audioOut = true,
          bool waveOut = false,
          double amp = 0.5);
    
    Mixer(const Mixer& other);
    
    Mixer& operator= (const Mixer& other);
    
    // Prevents some error .. ?
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
    
    void start();
    
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
    
    void setAudioOutputEnabled(bool state);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns whether or not audio output is enabled.
    *
    *  @return      Boolean, true if enabled, false if not.
    *
    *************************************************************************************************/
    
    bool isAudioOutputEnabled() const;
    
private:
    
    /*! The current master amplitude value */
    double masterAmp_;
    
    /*! Whether or not audio output is stopped */
    bool stopped_;
    
    /*! Whether or not to send samples to wavefile after stop() ist called */
    bool wavefileEnabled_;
    
    /*! Whether or not to send samples to direct output */
    bool audioOutputEnabled_;
    
    /*! CrossfadeUnit for panning */
    std::unique_ptr<CrossfadeUnit> pan_;
    
    /*! Direct audio output object */
    std::unique_ptr<AudioOutput> audioOutput_;
    
    /*! Wavfile object */
    std::unique_ptr<Wavefile> waveOutput_;
};

#endif /* defined(__Anthem__Mixer__) */
