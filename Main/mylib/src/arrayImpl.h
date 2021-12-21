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
        ss << _U("[") << join() << _U("]");
        return ss.str();
    }

	String join(const String& sep = String(_U(", ")))const
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
    typedef std::vector<var> InternalType;
    InternalType internalData;
};
