#ifndef Anthem_Pantable_hpp
#define Anthem_Pantable_hpp

#include "LookupTable.hpp"
#include "Sample.hpp"

#include <vector>

typedef LookupTable<Sample> Pantable;

class PantableDatabase
{
    
public:
    
    /*! Different types of crossfading */
    enum Types
    {
        LINEAR,
        SINE,
        SQRT
    };
    
    typedef unsigned short index_t;
    
    void init();
    
    Pantable& operator[] (index_t index);
    
    const Pantable& operator[] (index_t index) const;
    
    index_t size() const;
    
private:
    
    double* readPantable(const std::string& fname) const;
    
    index_t size_ = 5;
    
    Pantable tables_ [5];
};

extern PantableDatabase pantableDatabase;

#endif
