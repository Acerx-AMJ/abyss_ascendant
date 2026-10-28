#include "io.hpp"
#include "unistd.h"
#include <codecvt>
#include <locale>

// Wrap a string in color.
std::string wrap(const std::string& object, const std::string& color)
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
void clearln(int times)
{
   for (int i {}; i < times; i++)
   {
      std::cout << "\r\033[K";
      if (i < times - 1)
         std::cout << "\033[F";
   }
   std::cout << std::flush;
}

// Get a single character input from the user.
char getKey(std::string_view prompt, std::string_view color) 
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
std::string getHiddenInputLoud(std::string_view prompt, std::string_view color)
{
   print(prompt, color);
   std::string input {};

   setRawMode();
   getline(std::cin, input);
   resetTerminal();

   println();
   return input;
}

void getHiddenInput(std::string_view prompt, std::string_view color) 
{
   print(prompt, color);
   std::string input {};

   setRawMode();
   getline(std::cin, input);
   resetTerminal();

   println();
}

// Get string input from the user.
std::string getInput(std::string_view prompt, std::string_view color, int min, int max, bool filter) 
{
   std::string input {};
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
         flush("\b \b");
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
int getNumber(std::string_view prompt, std::function<bool(int)> condition) 
{
   int number {};
   std::string input {};
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
         else if (key == '!' && input.size() > 0) 
         {
            input.pop_back();
            flush("\b \b");
         }
         else if (isdigit(key) || (key == '-' && input.size() == 0)) 
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
            throw std::out_of_range("");

         // Everything went successfully.
         break;
      }
      catch (...) 
      {
         input.clear();
         clearln(2);
         println();
         flush("Invalid input, try again > ", RED);
      }
   }
   println();
   return number;
}

// Get consent from the user.
bool getConsent(std::string_view prompt, std::string_view color) 
{
   return getKey(prompt, color) == 'y';
}

// Make the user select an option, there's no color customization for this function.
std::string getOption(const std::vector<std::string>& options, const std::vector<std::string>& values)
{
   int size = options.size();
   println("\nOption:", YELLOW);

   for (int i {}; i < size; i++)
      println(" > [" + std::string(i + 1 < 10 && i + 1 >= 0 ? " " : "") + tostr(i + 1) + "] " + options.at(i));
   
   return values.at(getNumber("\nYour choice > ", 
   [&size](int num) -> bool 
   {
      return num >= 1 && num <= size;
   }) - 1);
}

// Return the size of a string as UTF-32.
int utf32Size(const std::string& prompt) 
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
