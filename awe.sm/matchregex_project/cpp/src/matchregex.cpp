#include "matchregex.h"
#include "matchregex_priv.h"
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
 *  - The prefix matching is stored in a 2 level trie tree data structure.
 *  - A stl::set<> is used to stored the exact matches, so we get a O(1) time. 
 *
 */
static const string emptyString;
static set<string> exactMatch;
static int shortestprefix = 0;

/**
 * 2-level trie tree
 * - I don't want to create too much depth 
 *   in the trie tree.  It may lose efficiency, if we 
 *   make this too complex.
 */
TrieTree::~TrieTree()
{
  clear();
}

void TrieTree::clear()
{
  data_.clear();
}

bool TrieTree::empty() const
{
  return data_.empty();
}

/**
 *  If prefix is "a", we store as "a ".
 */
void TrieTree::insert(const string prefix)
{
  if(prefix.length() <= 0)
  { 
    std::cerr << "[error] invalid prefix" << std::endl;
    return;
  }

  // 1. Determine keys for data_
  char firstchar = prefix[0];
  level1char.insert(firstchar);
  char secondchar = ' ';
  if(prefix.length() > 1)
    secondchar = prefix[1];

  // 2. Update data_
  TT_Data_Lvl2_Type level2 = data_[firstchar];
  (level2[secondchar]).push_back(prefix);
  data_[firstchar] = level2;
}

string TrieTree::findPrefix(const string phrase) const
{  
  // Parse the keys
  //  Note, TrieTree would never match a single char, unless we have '*'.
  if(phrase.length() <= 1) return "";
  char firstchar = phrase[0];
  if(level1char.find(firstchar) == level1char.end()) return "";
  char secondchar = ' ';
  if(phrase.length() >= 2)
    secondchar = phrase[1];
  
  // 1. Check level1 of trie tree
  TT_Data_Type::const_iterator level1Itr = data_.find(firstchar);
  if(level1Itr == data_.end()) 
    return "";
  
  // 2. Check level2 of trie tree
  TT_Data_Lvl2_Type level2 = level1Itr->second;
  TT_Data_Lvl2_Type::const_iterator level2Itr = level2.find(' ');
  if(level2Itr != level2.end()) 
  {
    // 2a. Checking single letter prefix, e.g. "a*"
    return string(1,firstchar) + "*"; 
  }
  else
  {
    // 2b. Checking single letter prefix, e.g. "ab*"
    level2Itr = level2.find(secondchar);
    if(level2Itr == level2.end()) 
      return "";
  }
  
  // 3. Check last level
  const list<string> level3 = level2Itr->second;
  list<string>::const_iterator level3Itr = level3.begin();
  for( ; level3Itr != level3.end(); level3Itr++)
  {
    string prefix = *level3Itr;
    if(phrase.length() <= prefix.length()) continue;
    if(phrase.find(prefix) == 0)
    {
      string tmp = prefix + "*";
      return tmp;
    }
  }
  return "";
}

ostream& operator<< (ostream &out, const TrieTree &tt)
{
  // 1.level1 of trie tree
  TT_Data_Type::const_iterator level1Itr = tt.data_.begin();
  for( ; level1Itr != tt.data_.end(); level1Itr++ )
  {
    out << "[" << level1Itr->first << "]" << std::endl;
    // 2. level2 of trie tree
    TT_Data_Lvl2_Type level2 = level1Itr->second;
    TT_Data_Lvl2_Type::const_iterator level2Itr = level2.begin();
    for( ; level2Itr != level2.end() ; level2Itr++) 
    {
      out << "  [" << level2Itr->first << "]" << std::endl;
      // 3. Check last level
      const list<string> level3 = level2Itr->second;
      list<string>::const_iterator level3Itr = level3.begin();
      for( ; level3Itr != level3.end(); level3Itr++)
      {
        out << "    " << *level3Itr << std::endl;
      }
    }
  }
  return out;
}

static TrieTree matchRegTrieTree;

/**
 *  Overloads default list
 */
void matchRegExInit(const list<std::string> regexList, bool reset)
{
  if(reset) 
  {
    exactMatch.clear();
    matchRegTrieTree.clear();
  }

  list<std::string>::const_iterator regexListItr = regexList.begin();
  for( ; regexListItr != regexList.end(); regexListItr++ )
  {
    string regex = *regexListItr;
    if(regex[0] == '*' && regex.length() == 1) continue;
    if(regex[regex.length() - 1] == '*')
    {
      string prefix = regex.substr(0,(regex.length()-1));

      // Optimization
      if(prefix.length() < shortestprefix ||
         shortestprefix <= 0)
        shortestprefix = prefix.length();

      // Trie Tree Initialization
      matchRegTrieTree.insert(prefix);
    }
    else
    {
      exactMatch.insert(regex);
    }
  }
}

/**
 * Initializes with Default Data Set
 */
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
  if(exactMatch.empty() && matchRegTrieTree.empty())
  {
    // Default list
    matchRegExDefaultInit();
    if(exactMatch.empty() && matchRegTrieTree.empty())
    {
      // This should never happen.  However, if there was a coding mistake 
      // in matchRegExDefaultInit(), this would display an error.
      std::cerr << "[error] no reg expressions initialized" << std::endl;
      return "";
    }
  }

  // 1. Check for exact match
  if(exactMatch.find(input) != exactMatch.end())
    return input;

  // 2. Check for prefix match

  // 2a. If input is shorter than prefix, we know it is not a match
  if(input.length() <= shortestprefix)
  {
    return emptyString;
  }

  // 2b. Utilize TrieTree
  return matchRegTrieTree.findPrefix(input);

}
