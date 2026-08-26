#ifndef AA_IO_HPP
#define AA_IO_HPP

#include "syntax.hpp"
#include <termios.h>
#include <iostream>
#include <functional>
#include <sstream>

// Colors used in the terminal, to make elements stand out.
constexpr cstr PINK    = "\033[38;2;255;128;255m";
constexpr cstr RED     = "\033[38;2;255;0;0m";
constexpr cstr ORANGE  = "\033[38;2;255;128;0m";
constexpr cstr YELLOW  = "\033[38;2;255;255;0m";
constexpr cstr L_GREEN = "\033[38;2;128;255;0m";
constexpr cstr GREEN   = "\033[38;2;0;255;0m";
constexpr cstr CYAN    = "\033[38;2;0;255;255m";
constexpr cstr BLUE    = "\033[38;2;0;0;255m";
constexpr cstr PURPLE  = "\033[38;2;128;0;255m";
constexpr cstr BROWN   = "\033[38;2;139;69;19m";
constexpr cstr D_BROWN = "\033[38;2;84;57;44m";
constexpr cstr BLACK   = "\033[38;2;0;0;0m";
constexpr cstr D_GRAY  = "\033[38;2;64;64;64m";
constexpr cstr GRAY    = "\033[38;2;128;128;128m";
constexpr cstr L_GRAY  = "\033[38;2;192;192;192m";
constexpr cstr WHITE   = "\033[38;2;255;255;255m";
constexpr cstr RES     = "\033[0m";

// The original terminal settings.
inline struct termios originalTerm;

// Convert an object to string.
template<typename T>
str tostr(T object)
{
   std::ostringstream stream;
   stream << object;
   return stream.str();
}

// Wrap a string in color.
str wrap(const str& object, const str& color);

// Print the given object to the terminal.
template<typename T>
void print(const T& object, vstr color = ""sv)
{
   std::cout << color << object << RES;
}

// Print the given object to the terminal and add a new line.
template<typename T>
void println(const T& object, vstr color = ""sv)
{
   std::cout << color << object << RES << std::endl;
}

void println();

// Flush a line to the terminal.
template<typename T>
void flush(const T& object, vstr color = ""sv)
{
   std::cout << color << object << RES << std::flush;
}

// Set the terminal to raw mode or reset it back
void setRawMode();
void resetTerminal();

// Clear multiple lines from the terminal.
void clearln(uint16 times);

// Get a single character input from the user.
char getKey(vstr prompt, vstr color = ""sv);
char getKey();

// Get input from the user
str getInput(vstr prompt, vstr color = ""sv, int16 min = 1, int16 max = 48, bool filter = true);
str getHiddenInputLoud(vstr prompt, vstr color = ""sv);
void getHiddenInput(vstr prompt, vstr color = ""sv);

// Get number input from the user.
int16 getNumber(vstr prompt, std::function<bool(int16)> condition);

// Get consent from the user.
bool getConsent(vstr prompt, vstr color = ""sv);

// Make the user select a single option.
str getOption(const v_str& options, const v_str& values);

// Return the size of a string as UTF-32.
uint16 utf32Size(const str& prompt);

#endif // AA_IO_HPP
