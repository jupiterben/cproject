#pragma once

class StrImpl : public IImpl
{
    friend class String;

public:
    typedef std::shared_ptr<StrImpl> SharedPtr;
    typedef std::weak_ptr<StrImpl> WeakPtr;

public:
    virtual StrImpl *toStrImpl() { return this; }
    inline static StrImpl *cast(IImpl *impl) { return impl ? impl->toStrImpl() : nullptr; }
    std::wstring toString() const 
    { 
        return L"\"" + str + L"\""; 
    }

    inline bool equal(const std::wstring &str) const { return this->str == str; }
    StrImpl(const std::wstring &s) : str(s) {}

private:
    const std::wstring str;
};
