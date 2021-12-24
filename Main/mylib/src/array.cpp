#include <jsc/array.h>
#include "arrayImpl.h"

auto& ArrayImplPool = TValueImplPool<ArrayImpl>::GetInstance();
//////////////////////////////////////////////////////////////////////////
Array::Array(const var& other) :var(other, other.getImpl<ArrayImpl>())
{

}

Array::Array(std::initializer_list<var> alist)
    : var(ArrayImplPool.GetOrCreate(ArrayImpl::InternalType(alist)))
{
}

String Array::join(String sep)
{
    if(isUndefined()) return String::UndefinedStr;
    return getImpl<ArrayImpl>()->join(sep);
}