/*********************************************************************************************//*!
*
*  @file        Pantable.hpp
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       LookupTable for pan/crossfading values.
*
*************************************************************************************************/

#ifndef Anthem_Pantable_hpp
#define Anthem_Pantable_hpp

#include "LookupTable.hpp"
#include "Sample.hpp"

#include <vector>

/*! A Pantable is just a normal LookupTable
 with two-channel Samples as value_t. */
typedef LookupTable<Sample> Pantable;

/*********************************************************************************************//*!
*
*  @brief       Database for pan/crossfading value LookupTables a.k.a. Pantables. 
*
*  @details     The PantableDatabase holds five different Pantables which can be accessed
*               from the CrossfadeUnit/Crossfader classes (or elsewhere )to lookup or
*               or interpolate between Sample values. The tables provided are:
*
*               + Linear panning:       left,right
*               + Sine panning:         sin(left * pi/2), sin(right * pi/2)
*               + Sqrt panning:         sqrt(left), sqrt(right)
*               + Scaled sine panning:  like sine, but multiplied with sqrt(2)/2
*               + Scaled sqrt panning:  like sqrt, but multiplied with sqrt(2)/2
*
*               Scaled tables have left and right at 0.5 in the middle.
*
*************************************************************************************************/

class PantableDatabase
{
    
public:
    
    /*! Different types of crossfading */
    enum Types
    {
        LINEAR,
        SINE,
        SQRT,
        SCALED_SINE,
        SCALED_SQRT
    };
    
    typedef unsigned short index_t;
    
    /*! Reads the pantable data from disk and initializes the Pantables. */
    void init();
    
    /*! Returns a Pantable. */
    Pantable& operator[] (index_t index);
    
    /*! Returns a const Pantable. */
    const Pantable& operator[] (index_t index) const;
    
    /*! Returns the number of Pantables in the database. */
    index_t size() const;
    
private:
    
    /*! Right now there are 5 Pantables. */
    index_t size_ = 5;
    
    /*! Array of pantables. */
    Pantable tables_ [5];
};

extern PantableDatabase pantableDatabase;

#endif
