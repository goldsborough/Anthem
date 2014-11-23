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

class Midi
{
public:
    
    typedef unsigned char byte_t;
    
    enum Status
    {
        NOTE_ON = 0b1001,
        NOTE_OFF = 0b1001
    };
    
    struct Message
    {
        byte_t status;
        byte_t note;
        byte_t velocity;
    };
    
    Midi();
    
    void init(Anthem* anthem);
    
    void openPort(byte_t portID);
    
    void closePort();
    
    bool hasOpenPort() const;
    
    byte_t getCurrentPortID() const;
    
    std::string getCurrentPortName() const;
    
    std::string getAnyPortName(byte_t id);
    
    byte_t getNumberOfPorts();
    
private:
    
    struct
    {
        byte_t id;
        
        std::string name;
        
    } port_;
    
    static void callback_(double timestamp,
                          std::vector<byte_t>*,
                          void* userData);
    
    static Anthem* anthem_;
    
    RtMidiIn midi_;
};

#endif /* defined(__Anthem__Midi__) */
