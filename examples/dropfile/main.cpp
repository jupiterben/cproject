#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>
#include <tchar.h>
#include <winerror.h>

// … Insert the function GetUIObjectOfFile here …
int __cdecl wmain(int argc, WCHAR **argv)
{
    if (argc == 3 && SUCCEEDED(CoInitialize(NULL)))
    {
        IDataObject *pdto;
        if (SUCCEEDED(GetUIObjectOfFile(NULL, argv[1],
                                        IID_IDataObject, (void **)&pdto)))
        {
            IDropTarget *pdt;
            if (SUCCEEDED(GetUIObjectOfFile(NULL, argv[2],
                                            IID_IDropTarget, (void **)&pdt)))
            {
                POINTL pt = {0, 0};
                DWORD dwEffect = DROPEFFECT_COPY | DROPEFFECT_LINK;
                if (SUCCEEDED(pdt->DragEnter(pdto, MK_LBUTTON,
                                             pt, &dwEffect)))
                {
                    dwEffect &= DROPEFFECT_COPY | DROPEFFECT_LINK;
                    if (dwEffect)
                    {
                        pdt->Drop(pdto, MK_LBUTTON, pt, &dwEffect);
                    }
                    else
                    {
                        pdt->DragLeave();
                    }
                }
                pdt->Release();
            }
            pdto->Release();
        }
        CoUninitialize();
    }
    return 0;
}