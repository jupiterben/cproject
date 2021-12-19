#include <jsc/array.h>
#include "arrayImpl.h"
#include "c17.hpp"
//////////////////////////////////////////////////////////////////////////
Array::Array(const var& other) :var(other, other.getImpl<ArrayImpl>())
{

}
Array::Array(std::initializer_list<var> alist):var(createInternal<ArrayImpl>(alist))
{
}

String Array::join(String sep)
{
    if(isUndefined()) return String::UndefinedStr;
    return getImpl<ArrayImpl>()->join(sep);
}