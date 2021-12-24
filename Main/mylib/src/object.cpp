#include <jsc/object.h>
#include "objectImpl.h"

TValueImplPool<ObjectImpl> ObjectImplPool;

Object::Object(const InitialMapType &keyValues) 
: var(ObjectImplPool.GetOrCreate(ObjectImpl::InternalType(keyValues.begin(), keyValues.end()))) {}

Object::Object(InitialListType initList) 
: var(ObjectImplPool.GetOrCreate(ObjectImpl::InternalType(initList))){}