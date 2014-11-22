#ifndef Anthem_WavetableDB_hpp
#define Anthem_WavetableDB_hpp

#include <vector>
#include <string>
#include <memory>

class Wavetable;

/*********************************************************************************************//*!
*
*  @brief       Stores Anthem's wavetables.
*
*  @details     The WavetableDB class manages all of Anthem's wavetables and is responsible for
*               providing Oscillators with Wavetables.
*
*************************************************************************************************/

class WavetableDB
{
    
public:
    
    typedef long index_t;
    
    /*! Standard wavetables */
    enum Wavetables
    {
        NONE = -1,
        
        SINE,
        SINE_2,
        SINE_4, // The number is the number of bits, not partials
        SINE_8,
        
        SQUARE, // with sigma
        SQUARE_2,
        SQUARE_4,
        SQUARE_8,
        SQUARE_16,
        SQUARE_32,
        SQUARE_64,
        
        SAW, // with sigma
        SAW_2,
        SAW_4,
        SAW_8,
        SAW_16,
        SAW_32,
        SAW_64,
        
        TRIANGLE,
        RAMP,
        
        DIRECT_TRIANGLE,
        DIRECT_SQUARE,
        DIRECT_SAW,
        
        SMOOTH_SQUARE,
        SMOOTH_SAW
    };
    
    /*********************************************************************************************//*!
    *
    *  @brief       Initialzes the WavetableDB.
    *
    *  @details     The WavetableDB is initialized by reading all available wavetables from the
    *               wavetable folder.
    *
    *************************************************************************************************/
    
    void init();
    
    Wavetable& operator[] (short wt);
    
    const Wavetable& operator[] (short wt) const;
    
    /*************************************************************************//*!
    *
    *   @brief Writes Wavetable object to file.
    *
    *   @param fname The name of the file to write to.
    *
    *   @param wt The wavetable object to write to file. 
    *
    ****************************************************************************/
    
    void writeWavetable(const std::string& fname, const Wavetable& wt) const;
    
private:
    
    /*************************************************************************//*!
    *
    *   @brief Reads a wavetable and returns a double pointer.
    *
    *   @param fname The name of the file to read from.
    *
    ****************************************************************************/
    
    double* readWavetable(const std::string& fname) const;
    
    /*! Vector of Wavetable objects */
    std::vector<std::unique_ptr<Wavetable>> tables_;
};

#endif
