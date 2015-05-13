#include "Anthem.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
    Global::init();
    
    Anthem anthem;
    
    anthem.operators[Anthem::D].setWavetable(WavetableDatabase::SINE);
    
    anthem.operators[Anthem::D].setActive(true);
    
    anthem.operators[Anthem::D].setLevel(0);
    
    
    anthem.envelopes[Anthem::A].setActive(true);
    
    anthem.envelopes[Anthem::A].setSegmentLength(Envelope::Segments::ATTACK, 100);
    
    anthem.operators[Anthem::D].attachMod(Operator::LEVEL, &anthem.envelopes[Anthem::A]);
    
     
    anthem.setNote(69, true);
    

    anthem.audio.start();
    
    anthem.mixer.startRecording();
    
    const unsigned long len = Global::samplerate * 1;
    
    while (anthem.getSampleCount() < len);
    
    anthem.mixer.saveRecording();
    
}
