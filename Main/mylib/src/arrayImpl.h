#include <jsc/impl.h>
#include <vector>

class ArrayImpl : public IImpl
{
public:
    ArrayImpl(std::initializer_list<var> alist) : internalData(alist) {}

public:
    ArrayImpl *toArrayImpl() { return this; }
    inline static ArrayImpl *cast(IImpl *impl) { return impl ? impl->toArrayImpl() : nullptr; }
    template <class... _Types>
    static ArrayImpl *createImpl(_Types &&..._Args)
    {
        return new ArrayImpl(_Args...);
    }

    std::wstring toString() const { return L"[]"; }

private:
    typedef std::vector<var> InternalType;
    InternalType internalData;
};
