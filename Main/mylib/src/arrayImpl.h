#include <jsc/impl.h>
#include <vector>
#include <jsc/str.h>

class ArrayImpl : public IImpl
{
    friend class Array;
public:
    ArrayImpl(std::initializer_list<var> alist) : internalData(alist) {}

public:
	virtual String toString() const
    { 
        StringStream ss;
        ss << "[" << join() << "]";
        return ss.str();
    }

	String join(const String& sep = String(", "))const
    {
        StringStream ss;
        return ss.str();
    }
private:
    typedef std::vector<var> InternalType;
    InternalType internalData;
};
