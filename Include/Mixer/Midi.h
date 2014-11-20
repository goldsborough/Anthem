/*********************************************************************************************//*!
*
*  @file        Midi.h
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

class Midi
{
public:
    
    typedef unsigned char byte_t;
    
    struct Message
    {
        byte_t status;
        byte_t note;
        byte_t velocity;
    };
    
    Midi();
    
    void openPort(byte_t portID);
    
    void closePort();
    
    bool hasOpenPort() const;
    
    byte_t getPortID() const;
    
    std::string getPortName() const;
    
    bool hasMessage();
    
    Message getLastMessage() const;
    
private:
    
    struct
    {
        byte_t id;
        
        std::string name;
        
    } port_;
    
    Message lastMessage_;
    
    std::vector<unsigned char> rawMessage_;
    
    std::unique_ptr<RtMidiIn> midi_;
};

#endif /* defined(__Anthem__Midi__) */
