#pragma once

#include <string>
#include <vector>


namespace sf
{
  namespace string
  {
    /*
    * Trim trailing whitespace on the left of input string
    */
    void trimLeft(std::string& str);

    /*
    * Trim trailing whitespace on the right of input string
    */
    void trimRight(std::string& str);

    /*
    * Trim trailing whitespace both left and right
    */
    void trim(std::string& str);

    std::vector<std::string> splitStr(const std::string& str, const std::string& delimeter);
  }
}