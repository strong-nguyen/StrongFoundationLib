#include "StringHelper.h"

#include <iostream>

void sf::string::trimLeft(std::string& str)
{
  if (str.empty())
  {
    return;
  }

  std::size_t i = 0;
  while (str[i] == ' ')
  {
    ++i;
  }

  if (i > 0)
  {
    str.erase(0, i);
  }
}

void sf::string::trimRight(std::string& str)
{
  if (str.empty())
  {
    return;
  }

  std::size_t i = str.size() - 1;
  while (str[i] == ' ')
  {
    if (i == 0)
    {
      break;
    }
    --i;
  }

  if (i == 0)
  {
    str.erase(0);  // Erase all
  }
  else if (i < str.size() - 1)
  {
    str.erase(i + 1);
  }
}

void sf::string::trim(std::string& str)
{
  trimLeft(str);
  trimRight(str);
}

std::vector<std::string> sf::string::splitStr(const std::string& str, const std::string& delimeter)
{
  if (str.empty() || delimeter.empty())
  {
    return {};
  }

  std::vector<std::string> tokens;
  std::size_t start = 0;
  std::size_t found = 0;
  do
  {
    found = str.find(delimeter, start);
    if (found != std::string::npos)
    {
      std::string token = str.substr(start, found - start);
      if (!token.empty())
      {
        tokens.push_back(token);
      }
    }
    else
    {
      break;
    }

    start = found + delimeter.length();
  } while (true);

  if (start < str.size())
  {
    std::string token = str.substr(start);
    if (!token.empty())
    {
      tokens.push_back(token);
    }
  }

  return tokens;
}
