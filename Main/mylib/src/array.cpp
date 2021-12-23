#include <jsc/array.h>
#include "arrayImpl.h"
#include "valueImplPool.h"

TValueImplPool<ArrayImpl> ArrayImplPool;
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