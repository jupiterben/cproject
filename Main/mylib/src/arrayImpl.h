#include <jsc/impl.h>
#include <vector>
#include "strImpl.h"

class ArrayImpl : public IValue
{
    friend class Array;
public:
    typedef std::vector<var> InternalType;
public:
    ArrayImpl(std::initializer_list<var> alist) : internalData(alist) {}
    ArrayImpl(const InternalType& d):internalData(d){}
    bool equal(const InternalType& d)const
    {
        return d == internalData;
    }
    static size_t hashCode(const InternalType& d)
    {
        size_t h = 0;
		size_t step = d.size() / 9 + 1;
		for (size_t i = 0; i < d.size(); i += step)
		{
			h = h ^ (size_t)(d[i].getHash());
		}
		return h;
    }

public:
	virtual String toString() const override
    { 
        StringStream ss;
        ss << _U("[") << join() << _U("]");
        return ss.str();
    }
    virtual size_t getHash()const override
    {
        return hashCode(internalData);
    }

    String join(const String &sep = _U(", ")) const
    {
        if(internalData.empty()) return String::EmptyStr;
        StringStream ss;
        for(auto itr = internalData.begin();itr!=internalData.end();++itr)
        {
            if(itr!=internalData.begin()) ss << sep;
            ss << itr->toString();
        }
        return ss.str();
    }
private:
    InternalType internalData;
};
