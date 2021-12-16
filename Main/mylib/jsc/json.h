
#include "var.h"

class JSON
{
public:
    String stringify(var obj);
    var parse(const String& str);
};
