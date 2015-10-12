/*********************************************************************************************//*!
*
*  @file        AudioOutput.hpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
*  @brief       Holds the AudioOutput class for real-time audio output.
*
*************************************************************************************************/

#ifndef __Anthem__AudioOutput__
#define __Anthem__AudioOutput__

#include <RtAudio.h>

#include <deque>
#include <memory>

class Anthem;
class Sample;

/*********************************************************************************************//*!
*
*  @brief       Direct, real-time audio output class.
*
*  @details     The AudioOutput class uses the RtAudio library to direct samples computed by
*               Anthem to the OS' audio output (DAC) to output sound in real-time.
*                                                                                                
*************************************************************************************************/

class AudioOutput
{
    
public:
    
    typedef unsigned int id_t;
    
    /*! Struct storing information about a DAC. */
    typedef RtAudio::DeviceInfo Device;
    
    /*! Constructs an AudioOutput object and attempts to open the default device. */
    AudioOutput();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Initializes the AudioOutput object with a pointer to an Anthem object.
    *
    *  @details     The AudioOutput class will call Anthem's tick() method whenever it needs
    *               samples to output to the sound card.
    *
    *  @param       anthem A pointer to an Anthem object.
    *
    *************************************************************************************************/
    
    static void init(Anthem* anthem);
    
    /****************************************************************************************************************************//*!
    *
    *  @brief       Opens a device for audio output.
    *
    *  @details     Note that at initialization the default device is opened so you can output
    *               sound without calling the open() method explicitly. Use this method if you
    *               want to change the output device to something other than the default.
    *
    *  @param       id The device ID.
    *
    *  @param       channels The number of channels to output to (1 = mono, 2 = stereo). Defaults to and should probably stay 2.
    *
    *  @param       frames The number of frames/samples to fetch at each callback.
    *
    *  @see         getNumberOfDevices()
    *
    *  @see         getDevice()
    *
    ********************************************************************************************************************************/
    
    void open(id_t id, id_t channels = 2, id_t frames = 256);
    
    /*! Closes the current device. Close before re-opening. */
    void close();
    
    /*! Starts the output stream. */
    void start();
    
    /*! Stops the output stream. */
     void stop();
    
    /*! Whether or not the stream is currently started. */
     bool isStreaming();
    
    /*! Whether or not there is currently an open device. */
     bool isOpen();
    
    /*! Returns the id of the current device. */
     id_t getCurrentID() const;
    
    /*! Returns a Device struct for the current in-use device. */
     Device getCurrentDevice() const;
    
    /*! Returns the number of available devices to output to. */
     id_t getNumberOfDevices();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns a Device struct for a device, given an ID.
    *
    *  @param       id The device ID.
    *
    *  @return      A Device struct holding information for the given ID of the device.
    *
    *************************************************************************************************/
    
     Device getDevice(id_t id);
    
    /*! Returns the current API/interface used for audio output. */
     std::string getApi() const;
    
private:
    
    /*! Callback function that fetches samples from Anthem. */
    static int _callback(void* output,
                     void* input,
                     unsigned int numberOfFrames,
                     double streamTime,
                     RtAudioStreamStatus status,
                     void* userData);

    /*! Returns the name of an audio output API as string, given the enum member. */
    std::string getApiName(const RtAudio::Api& api);

    /*! Pointer to the Anthem object to retrieve samples from. */
    static Anthem* _anthem;

    /*! The Device struct for the current device in use. */
    Device _device;

    /*! The id of the current device. */
    id_t _id;

    /*! The audio output api currently in use. */
    RtAudio::Api _api;

    /*! The name of the current api as string. */
    std::string _apiName;

    /*! The wrapped around audio api object from RtAudio. */
    RtAudio _audio;
};

#endif /* defined(__Anthem__AudioOutput__) */
