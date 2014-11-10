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

class Operator;

class FM
{
public:
    
    typedef unsigned short index_t;
    
    FM(index_t alg = 0,
       Operator* a = 0,
       Operator* b = 0,
       Operator* c = 0,
       Operator* d = 0);
    
    double tick();
    
    void setAlgorithm(index_t alg);
    
    unsigned short getAlgorithm() const;
    
    void setOperator(index_t index, Operator* op);
    
    Operator* getOperator(index_t index) const;
    
private:
    
    enum
    {
        A,
        B,
        C,
        D
    };
    
    double modulate_(index_t carrier, double value);
    
    double add_(index_t left, index_t right);
    
    index_t alg_;
    
    Operator* ops_ [4];
    
};

#endif /* defined(__Anthem__FM__) */
