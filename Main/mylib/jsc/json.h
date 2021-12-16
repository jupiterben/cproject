
#include "var.h"
#include "str.h"

class JSON
{
public:
    String stringify(var obj);
    var parse(const String& str);
};
