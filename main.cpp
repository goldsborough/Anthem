#include "Anthem.hpp"

#include <iostream>
#include <vector>

int main(int argc, const char * argv[])
{
  Global::init();
    
  Anthem anthem;
    
  anthem.operators[Anthem::D].setWavetable(WavetableDatabase::SINE);
    
  anthem.operators[Anthem::D].setActive(true);
    
  anthem.operators[Anthem::D].setLevel(1);
    
  anthem.audio.start();
    
  anthem.mixer.startRecording();
    
  const unsigned long len = Global::samplerate * 10;
    
  while (anthem.getSampleCount() < len);
    
  anthem.mixer.saveRecording();
}
