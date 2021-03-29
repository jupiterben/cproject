#include "resources.h"
#include "DllEntry.h"


class FooMaxPluginClassDesc;
class FooMaxPlugin : public GUP
{
public:
    //Constructor/Destructor
    FooMaxPlugin();
    virtual ~FooMaxPlugin();

    // GUP Methods
    virtual DWORD Start();
    virtual void Stop();
    virtual DWORD_PTR Control(DWORD parameter);
    virtual void DeleteThis();

    virtual IOResult Save(ISave *isave);
    virtual IOResult Load(ILoad *iload);
};

#define FooMaxPlugin_CLASS_ID Class_ID(0x49f572e1, 0x7335a03c)
class FooMaxPluginClassDesc : public ClassDesc2
{
public:
    virtual int IsPublic() { return TRUE; }
    virtual void *Create(BOOL /*loading = FALSE*/) { return new FooMaxPlugin(); }
    virtual const TCHAR *ClassName() { return _T("FooMaxPluginClass"); }
    virtual SClass_ID SuperClassID() { return GUP_CLASS_ID; }
    virtual Class_ID ClassID() { return FooMaxPlugin_CLASS_ID; }
    virtual const TCHAR *Category() { return _T("FooCategory"); }

    virtual const TCHAR *InternalName() { return _T("FooMaxPlugin"); } // returns fixed parsable name (scripter-visible name)
    virtual HINSTANCE HInstance() { return hInstance; }                // returns owning module handle

    static FooMaxPluginClassDesc &getInstance();
};
