#include "matchregex.h"
#include <ostream>
#include <sstream>
#include <set>
#include <list>

using namespace std;

/**
 *  Implementation
 * 
 *  SUMMARY:
 *
 */
static const string emptyString;
static set<string> exactMatch;
static list<string> prefixMatch;

const std::string matchRegEx(const std::string input)
{
  // Initialize once
  if(exactMatch.empty())
  {
    // TODO: I should make this configurable
    exactMatch.insert("Ask");
    prefixMatch.push_back("Ask");
    prefixMatch.push_back("Mozilla/1.0 (compatible; Ask Jeeves/Teoma");
    prefixMatch.push_back("Mozilla/2.0 (compatible; Ask Jeeves/Teoma");
    exactMatch.insert("Mozilla/2.0 (compatible; Ask Jeeves)");
    prefixMatch.push_back("Baiduspider-image");
    prefixMatch.push_back("Baiduspider-ads");
    prefixMatch.push_back("Baiduspider-cpro");
    prefixMatch.push_back("Baiduspider-favo");
  }

  // 1. Check for exact match
  if(exactMatch.find(input) != exactMatch.end())
    return input;

  // 2. Check for prefix match
  list<string>::iterator prefixMatchItr = prefixMatch.begin();
  for( ; prefixMatchItr != prefixMatch.end(); prefixMatchItr++)
  {
    string prefix = *prefixMatchItr;
    if(input.length() <= prefix.length()) continue;
    if(input.find(prefix) == 0)
    {
      string tmp = prefix + "*";
      return tmp;
    }
  }
  return emptyString;
}
