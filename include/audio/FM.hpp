/*********************************************************************************************//*!
*
*  @file        FM.hpp
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Frequency modulation class declaration.
*
*************************************************************************************************/

#ifndef __Anthem__FM__
#define __Anthem__FM__

class Operator;
enum class Mode;

/*************************************************************************************************//*!
*
*  @brief       Frequency modulation class.
*
*  @details     The FM class implements frequency modulation as well as additive synthesis
*               with 4 operators and 11 different algorithms, ranging from full linear
*               operator-to-operator frequency modulation to full parallel additive synthesis.
*
*****************************************************************************************************/

class FM
{
public:
    
    typedef unsigned short index_t;
    
    /*! The 4 operators, enumerated for convenience. */
    enum Operators { A, B, C, D };
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs an FM object.
    *
    *  @details     Note that the FM class does not memory-manage the pointer-to-Operators in any
    *               way, they are just used for synthesis.
    *
    *  @param       a Pointer to Operator A.
    *
    *  @param       b Pointer to Operator B.
    *
    *  @param       c Pointer to Operator C.
    *
    *  @param       d Pointer to Operator D.
    *
    *  @param       alg The algorithm used initially, defaults to 0 (full linear FM modulation).
    *
    *****************************************************************************************************/
    
    FM(Operator* a,
       Operator* b,
       Operator* c,
       Operator* d,
       index_t alg = 0);
    
    /*! Returns a synthesized sample. */
    double tick();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the currently used FM algorithm.
    *
    *  @details     Sets the FM algorithm by setting the appropriate modes for the individual operators.
    *
    *  @param       alg The algorithm number (0-11).
    *
    *  @throws      std::invalid_argument if the algorithm number is invalid.
    *
    *****************************************************************************************************/
    
    void setAlgorithm(index_t alg);
    
    /*! Returns the currently used algorithm. */
    unsigned short getAlgorithm() const;
    
private:
    
    /*! Returns an Operator's tick if active, else 0. */
    double tickIfActive_(index_t index);
    
    /*! The four Operators used for synthesis. */
    Operator* operators_ [4];
    
    /*! Frequency modulates an Operator with a value. */
    double modulate_(index_t carrier, double value);
    
    /*! Performs additive synthesis for a carrier Operator and a value. */
    double add_(index_t carrier, double value);
    
    /*! The current algorithm in use.  */
    index_t alg_;
    
};

#endif /* defined(__Anthem__FM__) */
