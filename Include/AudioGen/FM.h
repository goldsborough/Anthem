/*********************************************************************************************//*!
*
*  @file        FM.h
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Frequency modulation classes.
*
*************************************************************************************************/

#ifndef __Anthem__FM__
#define __Anthem__FM__

class FM
{
public:
    
    enum Algorithms
    {
        
    };
    
    FM();
    
    double tick();
    
    void setAlgorithm(const unsigned short& alg);
    
private:
    
    unsigned short alg_;
    
};

#endif /* defined(__Anthem__FM__) */
