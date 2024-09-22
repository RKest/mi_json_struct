# Mi json struct
Better way of serializing and deserializing [nlohmann::json](https://github.com/nlohmann/json) objects into and from structs.
**Though use boost::pfr, if you don't mind slower build times honestly**

## Motivational examples

>#### No repetitions
```cpp
// type_definition.hpp                            // type_definition.hpp
--------------------------------------------------------------------------------------------
                                                | #include <mi_json_struct.hpp>
namespace ns {                                  |
    struct Type                                 | MI_STRUCT_WITH_FROM_AND_TO_JSON(
    {                                           |    ns, Type,
        int a;                                  |    (int, a),
        double b;                               |    (double, b)
    };                                          |  )
}                                               | 
                                                |
ns::Type fronPath(const char *path);            | ns::Type fromPath(const char *path);
                                                |
---------------------------------------------------------------------------------------------
// type_definition.cpp                          | // type_definition.cpp
---------------------------------------------------------------------------------------------
#include <nlohmann/json.hpp>                    | #define MI_JSON_STRUCT_IMPL
#include <fstream>                              | #include <fstream>
#include "type_definition.hpp"                  | #include "type_definition.hpp"
// Repetition, if the "Type" struct changes,    | 
// this needs to be updated also                |
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Type, a, b)  | 
                                                |
ns::Type fronPath(const char *path)             | ns::Type fromPath(const char* path)
{                                               | {
    std::ifstream file{path};                   |     std::ifstream file{path};
    auto j = nlohmann::json::parse(file);       |     auto j = nlohmann::json::parse(file);
    return j.get<Type>();                       |     return j.get<Type>();
}                                               | }
```
The `#include <mi_json_struct.hpp>` is lightweight, as opposed to `<nlohmann/json.hpp>`\
Therefore, usually it is beneficial, to only include the latter in a single source file with all the type definitions.

>#### More granularity
Given this struct 
```cpp
#include <nlohmann/json.hpp>
#include <thread>

struct ThreadsNumber
{
    int value;

    friend inline void from_json(const nlohmann::json& j, ThreadsNumber& number)
    {
        int new_value = j.get<int>();
        number.value = new_value > 0 ? new_value : std::thread::hardware_concurrency();
    }
};
```
```cpp
struct Type                             | MI_STRUCT_WITH_FROM_JSON(
{                                       |    ns, Type,
    ThreadsNubmer threads_number;       |    (ThreadsNumber, threads_number),
    std::string other;                  |    (std::string, other)
};                                      | )
                                        | // ^
// Will not work, `ThreadsNumber`       | // Works perfectly fine, because we
// needs `to_json` defined as well      | // do not need `to_json`
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(     | 
        Type, threads_number, other)    | 
```

## Overview
Single header, no dependencies apart from [nlohmann::json](https://github.com/nlohmann/json).\
Defines three simple self-explanatory macros.
- MI_STRUCT_WITH_FROM_JSON
- MI_STRUCT_WITH_TO_JSON
- MI_STRUCT_WITH_FROM_AND_TO_JSON

The `<nlohmann/json.hpp>` header is only included if the directive:
```cpp
#define MI_JSON_STRUCT_IMPL
```
is defined.
