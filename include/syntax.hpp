#ifndef AA_SYNTAX_HPP
#define AA_SYNTAX_HPP

#include <string>
#include <cstdint>
#include <vector>

// Include string literals
using namespace std::string_literals;
using namespace std::string_view_literals;

// Unsigned integers
using uint8  = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

// Signed integers
using int8  = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

// Floating point numbers
using f32 = float;
using f64 = double;

// Strings
using str  = std::string;
using vstr = std::string_view;
using cstr = const char*;

// Other types
using boolean = bool;

// Vectors and matrixes
using v_str = std::vector<str>;
using m_str = std::vector<std::vector<str>>;

using v_uint = std::vector<uint16>;
using m_uint = std::vector<std::vector<uint16>>;

using v_boolean = std::vector<boolean>;
using m_boolean = std::vector<std::vector<boolean>>;

#endif // AA_SYNTAX_HPP
