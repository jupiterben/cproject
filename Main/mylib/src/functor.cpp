
#include <jsc/functor.h>
#include "functorImpl.h"

Functor::Functor(const var &v) : var(v, v.getImpl<IFunctorImpl>())
{
}
