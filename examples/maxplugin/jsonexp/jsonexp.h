#pragma once
#include "maxHeaders.h"
#include "resource.h"

extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;

class JsonExport : public SceneExport
{
public:
	JsonExport() {}
	~JsonExport() {}
	// SceneExport methods
	int ExtCount() {
		return 1;
	}                                                                                             // Number of extensions supported
	const TCHAR *Ext(int n) {
		switch (n) {
		case 0:
			// This cause a static string buffer overwrite
			return _T("json");
		}
		return _T("");
	}                                                                                    // Extension #n (i.e. "ASC")
	const TCHAR *LongDesc() {
		return GetString(IDS_LONGDESC);
	}
	const TCHAR *ShortDesc() {
		// Short ASCII description (i.e. "Ascii")
		return GetString(IDS_SHORTDESC);
	}
	const TCHAR *AuthorName() {
		// ASCII Author name
		return _T("");
	}
	const TCHAR *CopyrightMessage() {
		// ASCII Copyright message
		return _T("");
	}
	const TCHAR *OtherMessage1() {
		return _T("");// Other message #1
	}
	const TCHAR *OtherMessage2() {
		return _T("");// Other message #2
	}
	unsigned int Version() {
		return 100;
	}
	void ShowAbout(HWND hWnd) {
		// Show DLL's "About..." box
	}
	int DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0);
	BOOL SupportsOptions(int ext, DWORD options);
};

// Class ID. These must be unique and randomly generated!!
// If you use this as a sample project, this is the first thing
// you should change!
#define JSONEXP_CLASS_ID Class_ID(0x85548e0c, 0x4a26450c)
class JSONExpClassDesc : public ClassDesc
{
public:
    int IsPublic() { return 1; }
    void *Create(BOOL loading = FALSE) { return new JsonExport; }
    SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
    Class_ID ClassID() { return JSONEXP_CLASS_ID; }
	const TCHAR *ClassName() { return _T("JSONExp"); }
    const TCHAR *Category() { return GetString(IDS_CATEGORY); }
    static JSONExpClassDesc *getInstance();
};