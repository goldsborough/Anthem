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

#include <memory>
#include <utility>
#include <string>
#include <vector>

template <typename T>
class LookupTable
{
    
public:
    
    using value_t = T;
    
    using pointer_t = T*;
    
    using const_pointer_t = const T*;
    
    using index_t = typename std::vector<T>::size_type;
    
    
    LookupTable(pointer_t data,
                index_t size,
                const std::string& id) noexcept
    : id_(std::make_shared<std::string>(id)),
      data_(std::make_shared<std::vector<value_t>>(data, size))
    { }
    
    LookupTable(const LookupTable& other) noexcept
    : id_(other.id_),
      data_(other.data_)
    { }
    
    LookupTable(LookupTable&& other) noexcept
    : id_(std::move(other.id_)),
      data_(std::move(other.data_))
    { }

    LookupTable& operator= (LookupTable other) noexcept
    {
        swap(other);
        
        return *this;
    }
    
    virtual ~LookupTable() = default;
    
    
    virtual inline void swap(const LookupTable& other)
    {
        // Enable ADL
        using std::swap;
        
        swap(*data_, *other.data_);
        
        swap(*id_, *other.id_);
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
    
    virtual value_t interpolate(double index) const
    {
        // The truncated integral part
        long integral = static_cast<long>(index);
        
        // The remaining fractional part
        double fractional = index - integral;
        
        // Grab the two items in-between which the actual value lies
        value_t lower = (*data_)[integral];
        value_t upper = (*data_)[integral+1];
    
        // Perform interpolation
        return lower + ((upper - lower) * fractional);
    }
    
    virtual inline const value_t& operator[] (index_t index) const
    {
        return (*data_)[index];
    }
    
    /*! Returns a const LookupTable's data pointer. */
    virtual inline const_pointer_t data() const
    {
        return data_->data();
    }
    
    /*! Returns the LookupTable's data pointer. */
    virtual inline pointer_t data()
    {
        return data_->data();
    }
    
    /*! Returns the LookupTable's size. */
    virtual inline index_t size() const
    {
        return data_->size();
    }
    
    /*! Returns the LookupTable's id. */
    virtual inline std::string id() const
    {
        return *id_;
    }
    
protected:
    
    LookupTable(index_t size, const std::string& id)
    : id_(std::make_shared<std::string>(id)),
      data_(std::make_shared<std::vector<value_t>>(size))
    { }
    
    /*! The LookupTable's data. */
    std::shared_ptr<std::vector<value_t>> data_;
    
    /*! A descriptive ID for the LookupTable. */
    std::shared_ptr<std::string> id_;
};

#endif /* LOOKUP_TABLE_HPP */
