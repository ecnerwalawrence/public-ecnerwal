#include "matchregex.h"
#include <ostream>
#include <sstream>
#include <iostream>
#include <set>
#include <list>

using namespace std;

/**
 *  Implementation
 * 
 *  SUMMARY:
 *
 *  TODO: Handle "blah*blah*" case
 *
 */
static const string emptyString;
static set<string> exactMatch;
static list<string> prefixMatch;
static int shortestprefix = 0;

/**
 *  Overloads default list
 */
void matchRegExInit(const list<std::string> regexList, bool reset)
{
  if(reset) 
  {
    exactMatch.clear();
    prefixMatch.clear();
  }

  list<std::string>::const_iterator regexListItr = regexList.begin();
  for( ; regexListItr != regexList.end(); regexListItr++ )
  {
    string regex = *regexListItr;
    if(regex[regex.length() - 1] == '*')
    {
      string prefix = regex.substr(0,(regex.length()-1));
      prefixMatch.push_back(prefix);
      if(prefix.length() < shortestprefix ||
         shortestprefix <= 0)
        shortestprefix = prefix.length();
    }
    else
    {
      exactMatch.insert(regex);
    }
  }
  set<string>::iterator eitr = exactMatch.begin();
  for( ; eitr != exactMatch.end(); eitr++ )
  {
    std::cerr << "exact=" << *eitr << std::endl;
  }
  list<string>::iterator pitr = prefixMatch.begin();
  for( ; pitr != prefixMatch.end(); pitr++ )
  {
    std::cerr << "prefix=" << *pitr << std::endl;
  }
}

void matchRegExDefaultInit()
{
  list<string> defaultRegEx;
  defaultRegEx.push_back("Ask");
  defaultRegEx.push_back("Ask*");
  defaultRegEx.push_back("Mozilla/1.0 (compatible; Ask Jeeves/Teoma*");
  defaultRegEx.push_back("Mozilla/2.0 (compatible; Ask Jeeves/Teoma*");
  defaultRegEx.push_back("Mozilla/2.0 (compatible; Ask Jeeves)");
  defaultRegEx.push_back("Baiduspider-image*");
  defaultRegEx.push_back("Baiduspider-ads*");
  defaultRegEx.push_back("Baiduspider-cpro*");
  defaultRegEx.push_back("Baiduspider-favo*");
  matchRegExInit(defaultRegEx);
}

const std::string matchRegEx(const std::string input)
{
  // Initialize once
  if(exactMatch.empty())
  {
    // TODO: I should make this configurable
    //       Default list
    matchRegExDefaultInit();
  }

  // 1. Check for exact match
  if(exactMatch.find(input) != exactMatch.end())
    return input;

  // 2. Check for prefix match

  // 2a. If input is shorter than prefix, we know it is not a match
  if(input.length() <= shortestprefix)
    return emptyString;

  // 2b. Iterate each prefix to find a match
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
