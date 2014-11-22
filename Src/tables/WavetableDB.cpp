#include "WavetableDB.hpp"
#include "Wavetable.hpp"
#include "Parsley.hpp"
#include "Global.hpp"

#include <fstream>

void WavetableDB::init()
{
    // The wavetable configuration file
    TextParsley textParser("/Users/petergoldsborough/Documents/Anthem/rsc/wavetables/wavetables.txt");
    
    std::string fname;
    
    std::vector<std::string> names = textParser.getAllWords();
    
    tables_.resize(names.size() + 1);
    
    // Fetch all wavetable names and read their respective data files
    for (int i = 0; i < names.size(); ++i)
    {
        fname = "/Users/petergoldsborough/Documents/Anthem/rsc/wavetables/" + names[i] + ".wavetable";
        
        // Read wavetables with i as their id and push them into the tables_ vector.
        tables_[i].reset(new Wavetable(readWavetable(fname),i));
    }
}

double* WavetableDB::readWavetable(const std::string &fname) const
{
    std::ifstream file(fname);
    
    if (! file.is_open())
    { throw FileNotOpenError("Could not find wavetable file: " + fname); }
    
    if (! file.good())
    { throw FileOpenError("Error opening wavetable: " + fname); }
    
    char signature[6];
    
    file.read(signature, 6);
    
    if (strncmp(signature, "ANTHEM", 6))
    { throw ParseError("Invalid signature for Anthem file!"); }
    
    int len = Global::wtLen + 1;
    int size = len * sizeof(double);
    
    double * wt = new double [len];
    
    file.read(reinterpret_cast<char*>(wt), size);
    
    return wt;
}

void WavetableDB::writeWavetable(const std::string &fname, const Wavetable& wt) const
{
    std::ofstream file(fname);
    
    if (! file.is_open())
    { throw FileNotOpenError(); }
    
    if (! file.good())
    { throw FileOpenError(); }
    
    file.write("ANTHEM", 6);
    
    int size = (Global::wtLen + 1) * sizeof(double);
    
    file.write(reinterpret_cast<char*>(wt.get()), size);
}

Wavetable& WavetableDB::operator[](short wt)
{
    if (wt == NONE)
    { return tables_[tables_.size() - 1]; }
    
    if (wt < NONE || wt >= tables_.size())
    { throw std::invalid_argument("Wavetable ID out of range!"); }
    
    return tables_[wt];
}

const Wavetable& WavetableDB::operator[](short wt) const
{
    if (wt == NONE)
    { return tables_[tables_.size() - 1]; }
    
    if (wt < NONE || wt >= tables_.size())
    { throw std::invalid_argument("Wavetable ID out of range!"); }
    
    return tables_[wt];
}