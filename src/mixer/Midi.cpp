/********************************************************************************************//*!
*
*  @file        Midi.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Midi.hpp"
#include "Anthem.hpp"

#include <stdexcept>

Anthem* Midi::_anthem = 0;

Midi::Midi()
{
    // Try to open default midi port if any
    if (_midi.getPortCount())
    {
        try
        {
            _midi.openPort();
        }
        
        catch(RtMidiError& error)
        {
            error.printMessage();
        }
    }
}

void Midi::init(Anthem *anthem)
{
    _anthem = anthem;
    
    _midi.setCallback(&_callback);
}

void Midi::_callback(double timestamp,
                     std::vector<byte_t>* message,
                     void* userData)
{
    _anthem->setNote((*message)[1], (*message)[2]);
}

void Midi::openPort(byte_t portID)
{
    // Not sure if necessary but
    // better safe than sorry
    closePort();
    
    try
    {
        _midi.openPort(portID);
        
        _port.id = portID;
        
        _port.name = _midi.getPortName();
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
        _midi.closePort();
    }
    
    catch(RtMidiError& error)
    {
        error.printMessage();
    }
}

bool Midi::hasOpenPort() const
{
    bool isOpen = false;
    
    try
    {
        isOpen = _midi.isPortOpen();
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
        return _midi.getPortCount();
    }
    
    catch(RtMidiError& error)
    {
        error.printMessage();
    }
    
    return 0;
}

Midi::byte_t Midi::getCurrentPortID() const
{
    if (! hasOpenPort())
    { throw std::runtime_error("No port currently open!"); }
    
    return _port.id;
}

std::string Midi::getCurrentPortName() const
{
    if (! hasOpenPort())
    { throw std::runtime_error("No port currently open!"); }
    
    // Faster than calling getPortName() again
    return _port.name;
}

std::string Midi::getAnyPortName(byte_t id)
{
    try
    {
        return _midi.getPortName(id);
    }
    
    catch(RtMidiError& error)
    {
        error.printMessage();
    }
    
    return std::string();
}