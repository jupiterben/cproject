#pragma once
#include <memory>
#include "impl.h"

template <typename T>
class VarT
{
protected:
    typedef std::shared_ptr<T> InternalType;
    InternalType internalPtr;
    VarT() {}
    VarT(InternalType internal) : internalPtr(internal) {}

public:
    inline bool isUndefined() const { return internalPtr.get() == nullptr; }
};

class var : public VarT<IImpl>
{
public:
    const static var undefined;

public:
    var() {}
    var(const double& x);
    var(const InternalType internal) : VarT(internal) {}
    inline bool operator==(var other) const
    {
        return internalPtr == other.internalPtr;
    }
    var operator()(void);
    var operator+(var other) const;
    var operator-(var other) const;
    var operator*(var other) const;
    var operator/(var other) const;

    template <class _TyImpl, class... _Types>
    inline static var createInternal(_Types &&..._Args)
    { // make a shared_ptr
        return var(std::make_shared<_TyImpl>(_Args...));
    }

protected:
    inline NumberImpl *toNumber() const
    {
        return isUndefined() ? nullptr : internalPtr->toNumber();
    }
    inline IFunctorImpl *toFunctor() const
    {
        return isUndefined() ? nullptr : internalPtr->toFunctor();
    }
};
