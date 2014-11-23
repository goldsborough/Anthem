#ifndef Anthem_Notes_hpp
#define Anthem_Notes_hpp

#include "LookupTable.hpp"

struct Notetable : public LookupTable<double>
{
    void init();
};

extern Notetable notetable;

#endif
