#include "Midi.hpp"
#include "Anthem.hpp"

#include <iomanip>

Anthem* Midi::anthem_ = 0;

Midi::Midi()
{
    // Try to open default midi port if any
    if (midi_.getPortCount())
    {
        try
        {
            midi_.openPort();
        }
        
        catch(RtMidiError& error)
        {
            error.printMessage();
        }
    }
}

void Midi::init(Anthem *anthem)
{
    anthem_ = anthem;
    
    midi_.setCallback(&callback_);
}

void Midi::callback_(double timestamp,
                     std::vector<byte_t>* message,
                     void* userData)
{
    anthem_->setNote((*message)[1], (*message)[2]);
}

void Midi::openPort(byte_t portID)
{
    // Not sure if necessary but
    // better safe than sorry
    closePort();
    
    try
    {
        midi_.openPort(portID);
        
        port_.id = portID;
        
        port_.name = midi_.getPortName();
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
        midi_.closePort();
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
        isOpen = midi_.isPortOpen();
    }
    
    catch(RtMidiError& error)
    {
        error.printMessage();
    }
    
    return isOpen;
}

Midi::byte_t Midi::getNumberOfPorts()
{
    try
    {
        return midi_.getPortCount();
    }
    
    catch(RtMidiError& error)
    {
        error.printMessage();
    }
    
    return 0;
}

Midi::byte_t Midi::getCurrentPortID() const
{
    return port_.id;
}

std::string Midi::getCurrentPortName() const
{
    // Faster than calling getPortName() again
    return port_.name;
}

std::string Midi::getAnyPortName(byte_t id)
{
    try
    {
        return midi_.getPortName(id);
    }
    
    catch(RtMidiError& error)
    {
        error.printMessage();
    }
    
    return std::string();
}