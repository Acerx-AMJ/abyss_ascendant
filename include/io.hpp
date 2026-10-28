#ifndef AA_IO_HPP
#define AA_IO_HPP

#include <termios.h>
#include <iostream>
#include <functional>
#include <sstream>

// Colors used in the terminal, to make elements stand out.
constexpr const char * PINK    = "\033[38;2;255;128;255m";
constexpr const char * RED     = "\033[38;2;255;0;0m";
constexpr const char * ORANGE  = "\033[38;2;255;128;0m";
constexpr const char * YELLOW  = "\033[38;2;255;255;0m";
constexpr const char * L_GREEN = "\033[38;2;128;255;0m";
constexpr const char * GREEN   = "\033[38;2;0;255;0m";
constexpr const char * CYAN    = "\033[38;2;0;255;255m";
constexpr const char * BLUE    = "\033[38;2;0;0;255m";
constexpr const char * PURPLE  = "\033[38;2;128;0;255m";
constexpr const char * BROWN   = "\033[38;2;139;69;19m";
constexpr const char * D_BROWN = "\033[38;2;84;57;44m";
constexpr const char * BLACK   = "\033[38;2;0;0;0m";
constexpr const char * D_GRAY  = "\033[38;2;64;64;64m";
constexpr const char * GRAY    = "\033[38;2;128;128;128m";
constexpr const char * L_GRAY  = "\033[38;2;192;192;192m";
constexpr const char * WHITE   = "\033[38;2;255;255;255m";
constexpr const char * RES     = "\033[0m";

// The original terminal settings.
inline struct termios originalTerm;

// Convert an object to string.
template<typename T>
std::string tostr(T object)
{
   std::ostringstream stream;
   stream << object;
   return stream.str();
}

// Wrap a string in color.
std::string wrap(const std::string& object, const std::string& color);

// Print the given object to the terminal.
template<typename T>
void print(const T& object, std::string_view color = "")
{
   std::cout << color << object << RES;
}

// Print the given object to the terminal and add a new line.
template<typename T>
void println(const T& object, std::string_view color = "")
{
   std::cout << color << object << RES << std::endl;
}

void println();

// Flush a line to the terminal.
template<typename T>
void flush(const T& object, std::string_view color = "")
{
   std::cout << color << object << RES << std::flush;
}

// Set the terminal to raw mode or reset it back
void setRawMode();
void resetTerminal();

// Clear multiple lines from the terminal.
void clearln(int times);

// Get a single character input from the user.
char getKey(std::string_view prompt, std::string_view color = "");
char getKey();

// Get input from the user
std::string getInput(std::string_view prompt, std::string_view color = "", int min = 1, int max = 48, bool filter = true);
std::string getHiddenInputLoud(std::string_view prompt, std::string_view color = "");
void getHiddenInput(std::string_view prompt, std::string_view color = "");

// Get number input from the user.
int getNumber(std::string_view prompt, std::function<bool(int)> condition);

// Get consent from the user.
bool getConsent(std::string_view prompt, std::string_view color = "");

// Make the user select a single option.
std::string getOption(const std::vector<std::string>& options, const std::vector<std::string>& values);

// Return the size of a string as UTF-32.
int utf32Size(const std::string& prompt);

#endif // AA_IO_HPP
