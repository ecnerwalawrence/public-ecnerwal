#include <list>
#include <boost/unordered_map.hpp>
#include <map>
#include <set>

#ifndef ___MATCH_REG_EX_PRIV_H__
#define ___MATCH_REG_EX_PRIV_H__

/**
 * TrieTree class 
 * - 2 level trie tree for prefix matching.
 * - Note, this is a private class, but exposed for unit testing.
 */

/**
 * Trying different data structures to see any performance improvements.
 */
typedef boost::unordered_map<char,std::list<std::string> > TT_Data_Lvl2_Type;
typedef boost::unordered_map<char, TT_Data_Lvl2_Type > TT_Data_Type;
//typedef std::map<char,std::list<std::string> > TT_Data_Lvl2_Type;
//typedef std::map<char, TT_Data_Lvl2_Type > TT_Data_Type;

class TrieTree {
 private:  
  TT_Data_Type data_;

  // Set seems to be faster than unordered_map.
  std::set<char> level1char;
  
 public:

  TrieTree() {}
  virtual ~TrieTree();

  /**
   *  Clears all the content in the tree.
   */
  void clear();

  /**
   *  For error checking
   */
  bool empty() const;

  /**
   *  insert
   *  - populates the tree.
   *
   *  @param word - word to add in the tree
   */
  void insert(const std::string word);

  /**
   *  findPrefix
   *  - searches in the tree for matching prefix.
   *
   *  @param phrase - needle
   *  @return prefix + '*'
   */
  std::string findPrefix(const std::string phrase) const;

  // Helper operator overload for debugging.
  friend std::ostream& operator<< (std::ostream &out, const TrieTree &tt);

};

#endif //___MATCH_REG_EX_PRIV_H__
