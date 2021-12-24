
#include <jsc/functor.h>

Functor::Functor(const var &v) : var(v, v.getImpl<IFunctorImpl>())
{
}
