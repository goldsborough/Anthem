#ifndef Anthem_LookupTable_hpp
#define Anthem_LookupTable_hpp

#include <string>

template <typename T>
class LookupTable
{
    
public:
    
    typedef unsigned long index_t;
    
    LookupTable(T* data = 0,
                index_t size = 0,
                index_t id = 0,
                const std::string& name = std::string())
    
    : data_(data), size_(size), name_(name),
      id_(id), refptr_(new index_t(1))
    
    { }
    
    ~LookupTable()
    {
        if (! --(*refptr_))
        {
            delete [] data_;
            delete refptr_;
        }
    }
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs a LookupTable object from another LookupTable.
    *
    *  @details     Because this class uses reference counting, the new Wavetable object will point to
    *               other's data. Call makeUnique() to create a copy of the data pointed to (after).
    *
    *  @param       other The other LookupTable object.
    *
    *****************************************************************************************************/
    
    LookupTable(const LookupTable& other)
    
    : id_(other.id_), size_(other.size_), name_(other.name_),
      data_(other.data_), refptr_(other.refptr_)
    
    {
        // now one more instance is pointing to
        // the same data
        ++(*refptr_);
    }
    
    /*************************************************************************************************//*!
    *
    *  @brief       Makes the internal pointer point to other's data.
    *
    *  @details     Copying a LookupTable makes it point to other's data and destructs its data if it
    *               is the last class pointing to it. Use makeUnique() (after) to create a copy of the
    *               data pointed to.
    *
    *  @param       other The other LookupTable object.
    *
    *****************************************************************************************************/
    
    LookupTable& operator= (const LookupTable& other)
    {
        if (this != &other)
        {
            // delete current data if this is the last
            // instance pointing to this data
            if (! --(*refptr_))
            {
                delete [] data_;
                delete refptr_;
            }
            
            // copy other's data
            this->data_ = other.data_;
            this->size_ = other.size_;
            this->id_ = other.id_;
            this->name_ = other.name_;
            
            this->refptr_ = other.refptr_;
            
            // one more pointing to this data now
            ++(*refptr_);
        }
        
        return *this;
    }
    
    T& operator[] (double index)
    {
        // Make this object's data unique as it might
        // have to be changed
        makeUnique();
        
        return data_[index];
    }
    
    const T& operator[] (index_t index) const
    {
        return data_[index];
    }
    
    /*************************************************************************************************//*!
    *
    *  @brief       Interpolates values from a fractional index.
    *
    *  @details     This function returns a proportionate value from a fractional index. For example,
    *               passing it an index of 1.5 will return [1] + (([2] - [1]) * 0.5).
    *
    *  @param       ind The fractional index.
    *
    *****************************************************************************************************/
    
    T interpolate(double index) const
    {
        int indexBase = static_cast<int>(index);  // The truncated integer part
        double indexFract = index - indexBase;    // The remaining fractional part
        
        // grab the two items in-between which the actual value lies
        T value1 = data_[indexBase];
        T value2 = data_[indexBase+1];
        
        // interpolate: integer part + (fractional part * difference between value2 and value1)
        T final = value1 + ((value2 - value1) * indexFract);
        
        return final;
    }
    
    /*! Returns the LookupTable's data pointer. */
    T* get() const 
    {
        return data_;
    }
    
    /*! Returns the LookupTable's size. */
    index_t size() const
    {
        return size_;
    }
    
    /*! Returns the LookupTable's id. */
    index_t id() const
    {
        return id_;
    }
    
    /*! Sets the LookupTable's id */
    void setId(index_t id)
    {
        id_ = id;
    }
    
    /******************************************************************************************************//*!
    *
    *  @brief       Makes a unique copy of the pointed-to data.
    *
    *  @details     The LookupTable class implements reference counting so constructing a new LookupTable
    *               object from an existing object will make both objects point to the same data. It
    *               may be required to make the data unique to an object at one point, for which this
    *               method exists. Note that calling the non-const operator[] also calls makeUnique().
    *
    **********************************************************************************************************/
    
    LookupTable& makeUnique()
    {
        if ((*refptr_) > 1)
        {
            // One Table less pointing
            // to the current shared data
            --(*refptr_);
            
            refptr_ = new index_t(1);
            
            T* temp = data_;
            
            data_ = new T [size_];
            
            for (index_t i = 0; i < size_; ++i)
            {
                data_[i] = temp[i];
            }
        }
        
        return *this;
    }
    
    /*! Returns the name of the table. */
    std::string getName() const
    {
        return name_;
    }
    
    /*! Sets the name of the table. */
    void setName(const std::string& name)
    {
        name_ = name;
    }
    
protected:
    
    /*! The LookupTable's data */
    T* data_;
    
    /*! The LookupTable's ID */
    index_t id_;
    
    /*! The LookupTable's table size */
    index_t size_;
    
    /*! The pointer pointing to the number of LookupTable objects
        pointing to data_ */
    index_t* refptr_;
    
    /*! A descriptive name of the waveform in the Wavetable */
    std::string name_;
};

#endif
