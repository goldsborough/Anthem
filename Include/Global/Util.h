//
//  Util.h
//  Anthem
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__Util__
#define __Anthem__Util__

#include <string>

namespace Util
{
    
    extern std::string getDate();

    extern double noteToFreq(const unsigned short& n);

    extern unsigned short freqToNote(const double& freq);

    extern double centToFreq(const double& baseFreq, const int& centOffset);

    extern double semiToFreq(const double& baseFreq, const int& semiToneOffset);

    extern float getPassedTime(const unsigned long& start);
    
    extern double dbToAmp(const double& baseAmp, const double& dB);
    
    extern std::string checkFileName(std::string fname, const std::string& fileEnding);
}


#endif /* defined(__Anthem__Util__) */
