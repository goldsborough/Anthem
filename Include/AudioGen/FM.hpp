/*********************************************************************************************//*!
*
*  @file        FM.hpp
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

class Operator;

class FM
{
public:
    
    typedef unsigned short index_t;
    
    enum
    {
        A,
        B,
        C,
        D
    };
    
    FM(Operator* a,
       Operator* b,
       Operator* c,
       Operator* d,
       index_t alg = 0);
    
    double tick();
    
    void setAlgorithm(index_t alg);
    
    unsigned short getAlgorithm() const;
    
    void setActive(index_t index, bool state);
    
    bool isActive(index_t index) const;
    
private:
    
    double tickIfActive_(index_t index);
    
    void setModes_(bool a, bool b, bool c, bool d);
    
    struct
    {
        Operator* op = 0;
        
        bool active = true;
        
    } ops_ [4];
    
    double modulate_(index_t carrier, double value);
    
    double add_(index_t carrier, double value);
    
    index_t alg_;
    
};

#endif /* defined(__Anthem__FM__) */
