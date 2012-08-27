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
 *  - Exposed for unit testing.
 *   
 */
void matchRegExInit(const std::list<std::string> regexList, bool reset = true);
void matchRegExtDefaultInit();


#endif //___MATCH_REG_EX_H__
