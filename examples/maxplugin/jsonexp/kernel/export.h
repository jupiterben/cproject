#include "../maxHeaders.h"
#include "./json.h"

class ExportKernel
{
public:
    int GetStaticFrame() { return 0; }
    static json export2JSON(Interface *ip);
};
