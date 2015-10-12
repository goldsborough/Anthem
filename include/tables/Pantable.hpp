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
#include <memory>

/*! A Pantable is just a normal LookupTable
 with two-channel Samples as value_t. */
using Pantable = LookupTable<Sample>;

/*********************************************************************************************//*!
*
*  @brief       Database for pan/crossfading value LookupTables a.k.a. Pantables. 
*
*  @details     The PantableDatabase holds five different Pantables which can be accessed
*               from the CrossfadeUnit/Crossfader classes (or elsewhere )to lookup or
*               or interpolate between Sample values. The tables provided are:
*
*               + __Linear__ panning:       left,right
*               + __Sine__ panning:         sin(left * pi/2), sin(right * pi/2)
*               + __Sqrt panning:           sqrt(left), sqrt(right)
*               + ___Scaled sine__ panning: like sine, but multiplied with sqrt(2)/2
*               + ___Scaled sqrt__ panning: like sqrt, but multiplied with sqrt(2)/2
*
*               Scaled tables have left and right at 0.5 in the middle.
*
*************************************************************************************************/

class PantableDatabase
{
    
public:
    
    /*! Different types of crossfading */
    enum Type
    {
        LINEAR,
        SINE,
        SQRT,
        SCALED_SINE,
        SCALED_SQRT
    };
    
    using index_t = unsigned short;
    
    /*! Reads the pantable data from disk and initializes the Pantables. */
    PantableDatabase();
    
    /*! Returns a Pantable. */
    std::shared_ptr<Pantable>& operator[] (index_t type);
    
    /*! Returns a const Pantable. */
    const std::shared_ptr<Pantable>& operator[] (index_t type) const;
    
    /*! Returns the number of Pantables in the database. */
    index_t size() const;
    
private:
    
    /*! Array of pantables. */
    std::vector<std::shared_ptr<Pantable>> _tables;
};

extern PantableDatabase pantableDatabase;

#endif
