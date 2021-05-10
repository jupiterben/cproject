
#include <iostream>
#include <vector>

namespace cutils
{
    namespace algo
    {
        std::string Base64Encode(uint8_t const *bytes_to_encode, int len);
        std::vector<uint8_t> Base64Decode(std::string const &s);
    }
}
