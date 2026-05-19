#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>


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

    /*
    * Split a string by delimeter, then apply a transformation on each token
    */
    template <typename Function>
    auto splitStr(const std::string& str, const std::string& delimeter, Function transform) -> std::vector<std::invoke_result_t<Function, const std::string&>>
    {
      std::vector<std::string> tokens = splitStr(str, delimeter);
      if (tokens.empty())
      {
        return {};
      }

      using ReturnType = std::invoke_result_t<Function, const std::string&>;
      std::vector<ReturnType> results;
      results.reserve(tokens.size());

      std::transform(tokens.begin(), tokens.end(), std::back_inserter(results), transform);

      return results;
    }
  }
}