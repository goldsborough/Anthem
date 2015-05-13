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
    
    
    anthem.envelopes[Anthem::A].setSegmentLength(Envelope::Segments::ATTACK, 1000);
    
    anthem.envelopes[Anthem::A].setSegmentRate(Envelope::Segments::ATTACK, 2);
    
    anthem.envelopes[Anthem::A].setSegmentStartLevel(Envelope::Segments::ATTACK, 0);
    
    anthem.envelopes[Anthem::A].setSegmentEndLevel(Envelope::Segments::ATTACK, 1);
    
    
    anthem.envelopes[Anthem::A].setSegmentLength(Envelope::Segments::A, 1000);
    
    anthem.envelopes[Anthem::A].setSegmentBothLevels(Envelope::Segments::A, 1);
    
    
    anthem.envelopes[Anthem::A].setSegmentLength(Envelope::Segments::B, 1000);

    anthem.envelopes[Anthem::A].setSegmentRate(Envelope::Segments::B, 2);
    
    anthem.envelopes[Anthem::A].setSegmentStartLevel(Envelope::Segments::B, 1);
    
    anthem.envelopes[Anthem::A].setSegmentEndLevel(Envelope::Segments::B, 0);
    
    
    anthem.operators[Anthem::D].attachMod(Operator::LEVEL, &anthem.envelopes[Anthem::A]);
    
     
    anthem.setNote(69, true);
    

    anthem.audio.start();
    
    anthem.mixer.startRecording();
    
    const unsigned long len = Global::samplerate * 3;
    
    while (anthem.getSampleCount() < len);
    
    anthem.mixer.saveRecording();
    
}
