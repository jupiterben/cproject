#include <jsc/array.h>
#include "arrayImpl.h"
//////////////////////////////////////////////////////////////////////////
Array::Array(const var& other) :var(other, other.getImpl<ArrayImpl>())
{

}
