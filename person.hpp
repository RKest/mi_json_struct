#include <mi_json_struct.hpp>

MI_STRUCT_WITH_FROM_AND_TO_JSON(
    foo,
    Person,
    (std::string, name),
    (int, age),
    (std::string, address)
)
