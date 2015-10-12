/*********************************************************************************************//*!
*
*  @file        Midi.hpp
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Midi interfacing.
*
*************************************************************************************************/

#ifndef __Anthem__Midi__
#define __Anthem__Midi__

#include <RtMidi.h>
#include <memory>
#include <vector>
#include <string>

class Anthem;

/*********************************************************************************************//*!
*
*  @brief       Midi interaction class.
*
*  @details     The Midi class enables the reception and handling of MIDI signals in
*               real-time. The class works via callback methods that send note-on and
*               and note-off signals to an Anthem object when received.
*                                                                                                
*************************************************************************************************/

class Midi
{
public:
    
    /*! Datatype to hold one byte. */
    typedef unsigned char byte_t;
    
    /*! Constructs a Midi object. */
    Midi();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Initializes the Midi object with a pointer to an Anthem object.
    *
    *  @details     The Midi class will update Anthem's currently played notes whenever the
    *               user sends a note-on or note-off MIDI signal.
    *
    *  @param       anthem A pointer to an Anthem object.
    *
    *************************************************************************************************/
    
    void init(Anthem* anthem);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Opens a MIDI port for input.
    *
    *  @param       portID The ID of the port to open.
    *
    *  @see         getNumberOfPorts()
    *
    *************************************************************************************************/
    
    void openPort(byte_t portID);
    
    /*! Closes the current port. */
    void closePort();
    
    /*! Whether or not there is currently a port open. */
    bool hasOpenPort() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the current port ID.
    *
    *  @throws      std::runtime_error if no port currently open.
    *
    *  @see         getCurrentPortName()
    *
    *************************************************************************************************/
    
    byte_t getCurrentPortID() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the current port name/identification string.
    *
    *  @throws      std::runtime_error if no port currently open.
    *
    *  @see         getCurrentPortId()
    *
    *************************************************************************************************/
    
    std::string getCurrentPortName() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the port name string of any port, given its ID.
    *
    *  @param       id The ID to return the name string for.
    *
    *************************************************************************************************/
    
    std::string getAnyPortName(byte_t id);
    
    /*! Returns the number of ports available. */
    byte_t getNumberOfPorts();
    
private:
    
    /*! Struct with information for the current port. */
    struct
    {
        byte_t id;
        
        std::string name;
        
    } _port;
    
    /*! Callback function for MIDI input. */
    static void _callback(double timestamp,
                          std::vector<byte_t>*,
                          void* userData);
    
    /*! The pointer to an Anthem object to send note-on/off signals to. */
    static Anthem* _anthem;
    
    /*! The wrapped around midi api object from RtMidi. */
    RtMidiIn _midi;
};

#endif /* defined(__Anthem__Midi__) */
