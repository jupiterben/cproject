#include <jsc/impl.h>
#include <vector>
#include "c17.hpp"
#include <string>

class ArrayImpl : public IImpl
{
    friend class Array;
public:
    ArrayImpl(std::initializer_list<var> alist) : internalData(alist) {}

public:
    ArrayImpl *toArrayImpl() { return this; }
    inline static ArrayImpl *cast(IImpl *impl) { return impl ? impl->toArrayImpl() : nullptr; }

    std::wstring toString() const 
    { 
        return L"[" + join() + L"]"; 
    }

    std::wstring join(const std::wstring& sep = L", ")const
    {
        return c17::join(internalData.begin(),internalData.end(), sep); 
    }
private:
    typedef std::vector<var> InternalType;
    InternalType internalData;
};
