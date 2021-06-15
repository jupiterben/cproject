

#include <windows.h>
#include <tchar.h>
// #include "./propertymgr.h"
struct PACKEDMETA
{
    WORD mm;
    WORD xExt;
    WORD yExt;
    WORD dummy;
};

void LoadThumbnail(LPSTORAGE pIStorage, HBITMAP *pHbitmap, BITMAPINFO **pOutBitmapInfo);

HRESULT OpenStorageFileEx(const WCHAR *storageName, IStorage **pIStorage, DWORD mode = (STGM_DIRECT | STGM_READ | STGM_SHARE_DENY_WRITE))
{
    *pIStorage = NULL;
    STGOPTIONS StgOptions = {0};

    StgOptions.usVersion = 1;       /* Win2k+ */
    StgOptions.ulSectorSize = 4096; /* 512 limits us to 2GB */

    HRESULT hr = StgOpenStorageEx(storageName, mode, STGFMT_DOCFILE, 0, &StgOptions, NULL, IID_IStorage, (void **)pIStorage);

    // Fallback to the old method of opening files.
    if (FAILED(hr))
        hr = StgOpenStorage(storageName, NULL, mode, NULL, 0, pIStorage);
    return hr;
}

void LoadMaxThumbnail(TCHAR *file, HBITMAP *phbmp)
{
    if (nullptr == file)
        return;

    HRESULT res = S_OK;
    LPSTORAGE pIStorage = NULL;

    res = OpenStorageFileEx(file, &pIStorage);

    // First check if the MAX file has the new SummaryInformation thumbnail
    // CCJ - 08/27/98

    if (S_OK == res && pIStorage)
    {
        LoadThumbnail(pIStorage, phbmp, NULL);

        if (pIStorage)
        {
            pIStorage->Release();
        }
    }
}

void LoadThumbnail(LPSTORAGE pIStorage, HBITMAP *pHbitmap, BITMAPINFO **pOutBitmapInfo)
{
    PROPSPEC propSpec;
    PROPVARIANT propVar;
    IPropertySetStorage *pPropertySetStorage = NULL;
    IPropertyStorage *pPropertyStorage = NULL;

    // Get the Storage interface
    if (S_OK == pIStorage->QueryInterface(IID_IPropertySetStorage,
                                          (void **)&pPropertySetStorage))
    {
        if (S_OK == pPropertySetStorage->Open(FMTID_SummaryInformation,
                                              STGM_READ | STGM_SHARE_EXCLUSIVE,
                                              &pPropertyStorage))
        {
            propSpec.ulKind = PRSPEC_PROPID;
            propSpec.propid = PIDSI_THUMBNAIL;

            if (S_OK == pPropertyStorage->ReadMultiple(1, &propSpec, &propVar))
            {
                if (propVar.vt == VT_CF)
                {
                    // Got clipboard format
                    CLIPDATA *pClip = propVar.pclipdata;
                    PACKEDMETA *pPackedMeta = NULL;
                    if (pClip && pClip->ulClipFmt == -1)
                    {
                        BYTE *pb = pClip->pClipData;
                        if (*reinterpret_cast<DWORD *>(pb) == CF_METAFILEPICT)
                        {
                            // Got metafile format
                            pb += sizeof(DWORD); // Skip the clip type
                            pPackedMeta = (PACKEDMETA *)pb;
                            pb += sizeof(PACKEDMETA); // Skip packed meta struct
                            int nMetaFileSize = pClip->cbSize -
                                                sizeof(pClip->ulClipFmt) -
                                                sizeof(DWORD) -
                                                sizeof(PACKEDMETA);

                            HMETAFILE hMetaFile = NULL;
                            HDC hdcdib;
                            HDC hdc = GetDC(NULL);
                            BITMAPINFO *lpbmi = NULL;
                            HBITMAP hbitmap = NULL;
                            int nSizeImage = 4 * pPackedMeta->xExt * pPackedMeta->yExt;

                            lpbmi = (BITMAPINFO *)malloc(sizeof(BITMAPINFO) + nSizeImage);
                            if (lpbmi)
                            {
                                hMetaFile = SetMetaFileBitsEx(nMetaFileSize, pb);
                                if (hMetaFile)
                                {
                                    memset(lpbmi, 0, sizeof(BITMAPINFOHEADER));

                                    lpbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                                    lpbmi->bmiHeader.biWidth = pPackedMeta->xExt;
                                    lpbmi->bmiHeader.biHeight = pPackedMeta->yExt;
                                    lpbmi->bmiHeader.biPlanes = 1;
                                    lpbmi->bmiHeader.biBitCount = 32;
                                    lpbmi->bmiHeader.biCompression = BI_RGB;
                                    lpbmi->bmiHeader.biSizeImage = 0;

                                    void *bits = NULL;
                                    hbitmap = CreateDIBSection(hdc, lpbmi, DIB_RGB_COLORS, &bits, NULL, 0);
                                    GdiFlush();
                                    if (hbitmap)
                                    {
                                        hdcdib = CreateCompatibleDC(hdc);
                                        if (hdcdib)
                                        {
                                            HBITMAP hOld = (HBITMAP)SelectObject(hdcdib, hbitmap);
                                            PlayMetaFile(hdcdib, hMetaFile);
                                            SelectObject(hdcdib, hOld);
                                            if (pOutBitmapInfo != NULL)
                                            {
                                                memcpy(lpbmi->bmiColors, bits, nSizeImage);
                                                (*pOutBitmapInfo) = lpbmi;
                                            }
                                            else
                                            {
                                                free(lpbmi);
                                            }
                                            DeleteDC(hdcdib);
                                        }
                                        if (pHbitmap != NULL)
                                        {
                                            (*pHbitmap) = hbitmap;
                                        }
                                        else
                                        {
                                            DeleteObject(hbitmap);
                                        }
                                    }
                                    DeleteMetaFile(hMetaFile);
                                }
                            }

                            if (hdc)
                            {
                                ReleaseDC(NULL, hdc);
                            }
                        }
                    }
                }
                PropVariantClear(&propVar);
            }
        }
    }

    if (pPropertyStorage)
        pPropertyStorage->Release();

    if (pPropertySetStorage)
        pPropertySetStorage->Release();
}

int main()
{
    LoadMaxThumbnail(_T("d:\\a.max"), 0);
    return 1;
}