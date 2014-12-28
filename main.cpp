#include "Anthem.hpp"

#include <iostream>
#include <vector>

int main(int argc, const char * argv[])
{
    Global::init();
    
    Anthem anthem;
    
    anthem.operators[Anthem::A].setWavetable(WavetableDatabase::SINE);
    
    anthem.operators[Anthem::A].setMode(1);
    
    anthem.operators[Anthem::A].setLevel(1);
    
    anthem.operators[Anthem::A].setNote(48);
    
    anthem.operators[Anthem::A].setActive(true);
    
    anthem.effects[Anthem::A].setActive(true);
    
    anthem.effects[Anthem::A].setEffectType(EffectBlock::ECHO);
    
    //anthem.effects[Anthem::A].delay().setFeedback(1);
    
    anthem.effects[Anthem::A].echo().setDelayTime(0.5);
    
    //anthem.effects[Anthem::A].delay().setDecayTime(5);
    
    anthem.effects[Anthem::A].echo().setDryWet(0.8);
    
    anthem.audio.start();
    
    anthem.mixer.startRecording();
    
    while (anthem.count_ < Global::samplerate * 4);
    
    anthem.mixer.saveRecording();
}