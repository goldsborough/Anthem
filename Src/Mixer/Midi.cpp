#include "Midi.h"

#include <chrono>
#include <thread>

Midi::Midi()
{
    RtMidiIn* temp;
    
    // Allocate midi object
    try
    {
        temp = new RtMidiIn;
    }
    
    catch(RtMidiError& error)
    {
        error.printMessage();
    }
    
    // midi_ now owns the allocated RtMidiIn object
    midi_.reset(temp);
    
    // Try to open default midi port if any
    if (midi_->getPortCount())
    {
        try
        {
            midi_->openPort();
        }
        
        catch(RtMidiError& error)
        {
            error.printMessage();
        }
    }
}

void Midi::openPort(byte_t portID)
{
    // Not sure if necessary but
    // better safe than sorry
    closePort();
    
    try
    {
        midi_->openPort(portID);
    }
    
    catch(RtMidiError& error)
    {
        error.printMessage();
    }
    
    port_.id = portID;
    
    try
    {
        port_.name = midi_->getPortName();
    }
    
    catch(RtMidiError& error)
    {
        error.printMessage();
    }
}

void Midi::closePort()
{
    try
    {
        midi_->closePort();
    }
    
    catch(RtMidiError& error)
    {
        error.printMessage();
    }
}

bool Midi::hasOpenPort() const
{
    bool isOpen;
    
    try
    {
        isOpen = midi_->isPortOpen();
    }
    
    catch(RtMidiError& error)
    {
        error.printMessage();
    }
    
    return isOpen;
}

Midi::byte_t Midi::getPortID() const
{
    return port_.id;
}

std::string Midi::getPortName() const
{
    return port_.name;
}

bool Midi::hasMessage()
{
    // First get a message .. Format:
    // STATUS BYTE -- 4 status bits, 4 channel bits
    // DATA BYTE 1 -- 8 bits for note
    // DATA BYTE 2 -- 8 bits for velocity
    try
    {
        midi_->getMessage(&rawMessage_);
    }
    
    catch(RtMidiError& error)
    {
        error.printMessage();
    }
    
    // We get an empty vector if no message is
    // in the queue
    if (rawMessage_.empty()) return false;
    
    // If we did receive a message
    
    // Set to lastMessage_ member
    lastMessage_.status = rawMessage_[0];
    lastMessage_.note = rawMessage_[1];
    lastMessage_.velocity = rawMessage_[2];
    
    // Necessary sleep for 10 milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    // Return true -- data is now available
    return true;
}

Midi::Message Midi::getLastMessage() const
{
    return lastMessage_;
}