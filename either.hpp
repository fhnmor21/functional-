#include <type_traits>
#include <cstdio>


// Sum Types
template <typename Left, typename Right>
class Either
{
private:
    template<unsigned int>
    struct TDisambiguator
    {
        TDisambiguator() {}
    };

public:
    explicit Either(Left& l, TDisambiguator<1> disambiguator = TDisambiguator<1>())
        : _tag(ETag::LEFT)
        {
            Set<Left>(l);
        }
    explicit Either(Left&& l, TDisambiguator<2> disambiguator = TDisambiguator<2>())
        : _tag(ETag::LEFT)
        {
            Set<Left>(l);
        }
    explicit Either(Left* l, TDisambiguator<3> disambiguator = TDisambiguator<3>())
        : _tag(ETag::LEFT)
        {
            Set<Left>(l);
        }
    
    explicit Either(Right& r, TDisambiguator<4> disambiguator = TDisambiguator<4>())
        : _tag(ETag::RIGHT)
        {
            Set<Right>(r);
        }
    explicit Either(Right&& r, TDisambiguator<5> disambiguator = TDisambiguator<5>())
        : _tag(ETag::RIGHT)
        {
            Set<Right>(r);
        }
    explicit Either(Right* r, TDisambiguator<6> disambiguator = TDisambiguator<6>())
        : _tag(ETag::RIGHT)
        {
            Set<Right>(r);
        }
    
    Either()
        : _tag(ETag::UNSET)
        {}

	Either(const Either& other)
	{
		Copy(other);
	}

	~Either()
	{
		Reset();
	}

    void Copy(const Either& other)
    {
		this->_values = other._values;
        this->_tag = other._tag;
    }

    bool operator==(const Either& rhs) const
    {
        bool is_equal = (_tag == rhs._tag);
        is_equal = is_equal && (_values == rhs._values);
        return is_equal;
    }

    bool operator!=(const Either& rhs) const
    {
        return !operator==(rhs);
    }

    Either& operator=(const Either& rhs)
    {
        Reset();
        Copy(rhs);
        return *this; 
    }

    void Reset()
    {
        if(IsLeft())
        { 
            Left* left = static_cast<Left*>(&_values._left);
            left->~Left(); 
        }
        if(IsRight())
        {
            Right* right = static_cast<Right*>(&_values._right);
            right->~Right();
        }
        _tag = ETag::UNSET;
    }

    template<typename SubType>
    void Set(SubType* value)
    {
        if(value)
        {
            Set<SubType>(*value);
        }
        else
        {
            Reset();
        }
        return;
    }

    template<typename SubType>
    void Set(SubType& value)
    {
        Set<SubType>(SubType(value));
    }

    template<typename SubType>
    void Set(SubType&& value)
    {
        if constexpr (std::is_same<Left, SubType>::value == true)
        {
            _tag = ETag::LEFT;
            _values._left = value;
            return;
        }
        if constexpr (std::is_same<Right, SubType>::value == true)
        {
            _tag = ETag::RIGHT;
            _values._right = value;
            return;
        }
    }

    template<typename SubType>
    const SubType* Get()
    {
        SubType* retval = nullptr;
        _getter(*this, retval);
        return retval;
    }

    template<typename SubType>
    bool IsOf() const
    {
        const SubType* val = nullptr;
        return _is(*this, val);
    }

    bool IsLeft() const
    {
        return _tag == ETag::LEFT;
    }

    bool IsRight() const
    {
        return _tag == ETag::RIGHT;
    }

    bool IsSet() const
    {
        return !(_tag == ETag::UNSET);
    }

protected:
    template<typename L, typename R, typename O>
    static void _getter(Either<L,R>& in, O*& inout)
    {
        inout =  nullptr;
    }

    template<typename L, typename R>
    static void _getter(Either<L,R>& in, L*& inout)
    {
        inout = static_cast<L*>(&in._values._left);
    };

    template<typename L, typename R>
    static void _getter(Either<L,R>& in, R*& inout)
    {
        inout = static_cast<R*>(&in._values._right);
    };

    template<typename L, typename R, typename O>
    bool _is(const Either<L,R>& in, const O* inout) const
    {
        return false;
    }
    
    template<typename L, typename R>
    bool _is(const Either<L,R>& in, const L* inout) const
    {
        return in.IsLeft();
    }

    template<typename L, typename R>
    bool _is(const Either<L,R>& in, const R* inout) const
    {
        return in.IsRight();
    }

    enum class ETag
    {
        UNSET,
        LEFT,
        RIGHT,
    };

    ETag _tag = ETag::UNSET;

    union {
        Left _left;
        Right _right;
    } _values;

};

/* TEST
int main()
{
    bool test =  false;
    int val = 42;
    
    Either<bool, int> BoolOrInt; //(val);

    BoolOrInt.Set(true);

    if(BoolOrInt.IsOf<bool>())
    {
        printf("Left\n");
        return 1;
    }
    else if(BoolOrInt.IsOf<int>())
    {
        const int* result = BoolOrInt.Get<int>();
        printf("Right value = %d \n", *result);
        return 2;
    } 
    printf("Test failed\n");
    return 0;
}
// END TEST */
