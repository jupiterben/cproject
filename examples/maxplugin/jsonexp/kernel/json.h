
#include <nlohmann/json.hpp>
#include "fstring.h"
using json = nlohmann::basic_json<nlohmann::ordered_map, std::vector, FString>;
#define json_null json(json::value_t::null)
