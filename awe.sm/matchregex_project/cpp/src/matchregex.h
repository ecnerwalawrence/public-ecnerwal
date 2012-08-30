#ifndef ___MATCH_REG_EX_H__
#define ___MATCH_REG_EX_H__

#include <string>
#include <list>

/**
 * matchRegExp (Awe.sm)
 *
 * @param input - String to determine whether we find a match.
 * @return -  if not match, returns empty string
 *            if match, returns regular expression
 */
const std::string matchRegEx(const std::string input);

/**
 * matchRegExpInit
 *  - Helper function to initialize the regular expression list  
 *
 * @param regexList - additional regular expression to add.
 * @param reset - clear previous loaded regular expressions.
 */
void matchRegExInit(const std::list<std::string> regexList, bool reset = true);
void matchRegExDefaultInit();

#endif //___MATCH_REG_EX_H__
