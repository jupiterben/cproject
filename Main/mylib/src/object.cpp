#include <jsc/object.h>
#include "objectImpl.h"

Object::Object(const InitialMapType &keyValues) : var(createInternal<ObjectImpl>(keyValues)) {}
Object::Object(InitialListType initList) : var(createInternal<ObjectImpl>(initList)) {}