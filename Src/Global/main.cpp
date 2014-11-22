#include "Anthem.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
    clock_t t = clock();
    
    const unsigned long end = t + 5 * CLOCKS_PER_SEC;
    
    Global::init();
    
    Anthem anthem;
    
    anthem.mixer.setMasterAmp(0.2);
    
    anthem.mixer.startRecording();
    
    while (clock() != end);
    
    anthem.mixer.saveRecording();
    
    std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}