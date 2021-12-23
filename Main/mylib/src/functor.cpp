
#include <jsc/functor.h>
#include "functorImpl.h"
#include "valueImplPool.h"

TValueImplPool<FunctorImpl> FunctorImplPool;

Functor::Functor(const var &v) : var(v, v.getImpl<IFunctorImpl>())
{
}
