
#include "foomaxplugin.h"

FooMaxPlugin::FooMaxPlugin()
{

}

FooMaxPlugin::~FooMaxPlugin()
{

}

void FooMaxPlugin::DeleteThis()
{
	delete this;
}

// Activate and Stay Resident
DWORD FooMaxPlugin::Start()
{
	#pragma message(TODO("Do plugin initialization here"))
	#pragma message(TODO("Return if you want remain loaded or not"))
	return GUPRESULT_KEEP;
}

void FooMaxPlugin::Stop()
{
	#pragma message(TODO("Do plugin un-initialization here"))
}

DWORD_PTR FooMaxPlugin::Control( DWORD /*parameter*/ )
{
	return 0;
}

IOResult FooMaxPlugin::Save(ISave* /*isave*/)
{
	return IO_OK;
}

IOResult FooMaxPlugin::Load(ILoad* /*iload*/)
{
	return IO_OK;
}

FooMaxPluginClassDesc & FooMaxPluginClassDesc::getInstance()
{
	static FooMaxPluginClassDesc desc;
	return desc;
}
