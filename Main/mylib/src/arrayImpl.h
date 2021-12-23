#include <jsc/impl.h>
#include <vector>
#include "strImpl.h"

typedef std::vector<var> VarArray;
struct VarArrayHash
{
    std::size_t operator()(const VarArray &d) const noexcept
    {
        size_t h = 0;
        size_t step = d.size() / 9 + 1;
        for (size_t i = 0; i < d.size(); i += step)
        {
            h = h ^ (size_t)(d[i].getHash());
        }
        return h;
    }
};

class ArrayImpl : public TValueImpl<VarArray, VarArrayHash, std::equal_to<VarArray> >
{
    friend class Array;
public:
    ArrayImpl(std::initializer_list<var> alist) : TValueImpl(alist) {}
    ArrayImpl(const InternalType &d) : TValueImpl(d) {}
public:
	virtual String toString() const override
    { 
        StringStream ss;
        ss << _U("[") << join() << _U("]");
        return ss.str();
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
};
