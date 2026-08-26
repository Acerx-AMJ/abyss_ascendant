#include "io.hpp"
#include "unistd.h"
#include <codecvt>
#include <locale>

// Wrap a string in color.
str wrap(const str& object, const str& color)
{
   return color + object + RES;
}

// Print a newline to the terminal.
void println() 
{
   std::cout << std::endl;
}

// Set terminal to raw mode, disabling some of the features.
void setRawMode() {
   // Save the original terminal.
   tcgetattr(STDIN_FILENO, &originalTerm);

   // Set to raw mode.
   struct termios tty;
   tcgetattr(STDIN_FILENO, &tty);
   tty.c_lflag &= ~(ICANON | ECHO);

   tty.c_cc[VMIN] = 1;
   tty.c_cc[VTIME] = 0;

   tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

// Reset the terminal back to the original settings.
void resetTerminal() 
{
   tcsetattr(STDIN_FILENO, TCSANOW, &originalTerm);
}

// Clears all of the previous lines.
void clearln(uint16 times)
{
   for (uint16 i {}; i < times; i++)
   {
      std::cout << "\r\033[K"s;
      if (i < times - 1)
         std::cout << "\033[F"s;
   }
   std::cout << std::flush;
}

// Get a single character input from the user.
char getKey(vstr prompt, vstr color) 
{
   print(prompt, color);
   setRawMode();

   // Get input from the user and restore the terminal.
   char input = getchar();
   resetTerminal();

   if (input == '\n' || input == '\r') 
      input = '?';
   else if (input == 8 || input == 127) 
      input = '!';

   println(input);
   return tolower(input);
}

char getKey() 
{
   setRawMode();

   // Get input from the user and restore the terminal.
   char input = getchar();
   resetTerminal();

   if (input == '\n' || input == '\r')
      input = '?';
   else if (input == 8 || input == 127) 
      input = '!';

   return tolower(input);
}

// Get hidden input from the user.
str getHiddenInputLoud(vstr prompt, vstr color)
{
   print(prompt, color);
   str input {};

   setRawMode();
   getline(std::cin, input);
   resetTerminal();

   println();
   return input;
}

void getHiddenInput(vstr prompt, vstr color) 
{
   print(prompt, color);
   str input {};

   setRawMode();
   getline(std::cin, input);
   resetTerminal();

   println();
}

// Get string input from the user.
str getInput(vstr prompt, vstr color, int16 min, int16 max, bool filter) 
{
   str input {};
   flush(prompt, color);

   while (true) 
   {
      const char key {getKey()};

      // Handle user input.
      if (key == '?' && input.size() >= min) 
      {
         break;
      }
      else if (key == '!' && input.size() > 0) 
      {
         input.pop_back();
         flush("\b \b"s);
      }
      else if (input.size() < max && ((filter && (isalnum(key) || key == '_')) || !filter)) 
      {
         if (key == '!' || key == '?') 
            continue;

         input.push_back(key);
         flush(key);
      }
   }
   println();
   return input;
}

// Get number input from the user.
int16 getNumber(vstr prompt, std::function<bool(int16)> condition) 
{
   int16 number {};
   str input {};
   flush(prompt);

   while (true) 
   {
      while (true) 
      {
         const char key {getKey()};

         // Handle user input.
         if (key == '?' && input.size() > 0) 
         {
            break;
         }
         else if (key == '!'  && input.size() > 0) 
         {
            input.pop_back();
            flush("\b \b"s);
         }
         else if (isdigit(key) || key == '-' && input.size() == 0) 
         {
            if (input.size() >= 9) 
               continue;

            input.push_back(key);
            flush(key);
         }
      }
      
      try 
      {
         number = stoi(input);
         if (!condition(number)) 
            throw std::out_of_range(""s);

         // Everything went successfully.
         break;
      }
      catch (...) 
      {
         input.clear();
         clearln(2);
         println();
         flush("Invalid input, try again > "s, RED);
      }
   }
   println();
   return number;
}

// Get consent from the user.
bool getConsent(vstr prompt, vstr color) 
{
   return getKey(prompt, color) == 'y';
}

// Make the user select an option, there's no color customization for this function.
str getOption(const v_str& options, const v_str& values)
{
   uint16 size = options.size();
   println("\nOption:"s, YELLOW);

   for (uint16 i {}; i < size; i++)
      println(" > ["s + str(i + 1 < 10 && i + 1 >= 0 ? " "s : ""s) + tostr(i + 1) + "] "s + options.at(i));
   
   return values.at(getNumber("\nYour choice > "s, 
   [&size](int num) -> bool 
   {
      return num >= 1 && num <= size;
   }) - 1);
}

// Return the size of a string as UTF-32.
uint16 utf32Size(const str& prompt) 
{
   std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

   try 
   {
      return converter.from_bytes(prompt).size();
   }
   catch (...)  
   {
      return -1;
   }
}
