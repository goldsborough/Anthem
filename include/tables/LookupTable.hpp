/*********************************************************************************************//*!
*
*  @file        LookupTable.hpp
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       The LookupTable template class declaration and definition.
*
*************************************************************************************************/

#ifndef LOOKUP_TABLE_HPP
#define LOOKUP_TABLE_HPP

#include <utility>
#include <string>
#include <vector>

template <typename T>
class LookupTable
{
    
public:
    
    LookupTable(T* data,
                std::size_t size,
                const std::string& id) noexcept
    : _id(id),
      _data(data, data + size)
    { }
    
    LookupTable(const LookupTable& other) noexcept
    : _id(other._id),
      _data(other._data)
    { }
    
    LookupTable(LookupTable&& other) noexcept
    : _id(std::move(other._id)),
      _data(std::move(other._data))
    { }

    LookupTable& operator= (LookupTable other) noexcept
    {
        swap(other);
        
        return *this;
    }
    
    virtual ~LookupTable() = default;
    
    
    virtual inline void swap(LookupTable& other)
    {
        // Enable ADL
        using std::swap;
        
        swap(_data, other._data);
        
        swap(_id, other._id);
    }
    
    friend inline void swap(const LookupTable& left,
                            const LookupTable& right)
    {
        left.swap(right);
    }

    
    /************************************************************************//*!
    *
    *  @brief       Interpolates values from a fractional index.
    *
    *  @details     This function returns a proportionate value 
    *               from a fractional index. For example, passing
    *               it an index of 1.5 will return [1] + (([2] - [1]) * 0.5).
    *
    *  @param       index The fractional index.
    *
    ***************************************************************************/
    
    virtual T interpolate(double index) const
    {
        // The truncated integral part
        long integral = static_cast<long>(index);
        
        // The remaining fractional part
        double fractional = index - integral;
        
        // Grab the two items in-between which the actual value lies
        T lower = _data[integral];
        T upper = _data[integral+1];
    
        // Perform interpolation
        return lower + ((upper - lower) * fractional);
    }
    
    virtual inline const T& operator[] (std::size_t index) const
    {
        return _data[index];
    }
    
    /*! Returns a const LookupTable's data pointer. */
    virtual inline const T* data() const
    {
        return _data.data();
    }
    
    /*! Returns the LookupTable's data pointer. */
    virtual inline T* data()
    {
        return _data.data();
    }
    
    /*! Returns the LookupTable's size. */
    virtual inline std::size_t size() const
    {
        return _data.size();
    }
    
    /*! Returns the LookupTable's id. */
    virtual inline std::string id() const
    {
        return _id;
    }
    
protected:
    
    LookupTable(std::size_t size, const std::string& id)
    : _id(id), _data(size)
    { }
    
    /*! The LookupTable's data. */
    std::vector<T> _data;
    
    /*! A descriptive ID for the LookupTable. */
    std::string _id;
};

#endif /* LOOKUP_TABLE_HPP */
