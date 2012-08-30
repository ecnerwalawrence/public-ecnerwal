#include <sys/types.h>
#include <sys/time.h>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <list>
#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "matchregex.h"
#include "matchregex_priv.h"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <cppunit/XmlOutputter.h>

using namespace std;
using namespace boost::posix_time;

/**
 * fx highlight_doc() Unit Test
 *
 *  - This unit test provides two checks
 *  - Base kind 
 *  - text input type unit test
 */

// #define DEBUG 
class testMatchRegEx : public CppUnit::TestFixture
{
 
  // Comment any test to remove a test case
  CPPUNIT_TEST_SUITE(testMatchRegEx) ;
  // Begin test suite 
  CPPUNIT_TEST(test_emptyInput);
  CPPUNIT_TEST(test_errors);
  CPPUNIT_TEST(test_weirdcases);
  CPPUNIT_TEST(test_basic);
  CPPUNIT_TEST(test_trietree);
  CPPUNIT_TEST(test_performanceLotsOfInput);
  CPPUNIT_TEST(test_performanceLargeRegex);
  // End test suite definitions
  CPPUNIT_TEST_SUITE_END() ;

public:
  
  // Clean-up and initializer
  void setUp() 
  {
    matchRegExDefaultInit();
  }

  void tearDown()
  {
  }


  void test_emptyInput()
  {
    string msg = "Result does not match with expected output";
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("") == ""));
  }

  void test_errors()
  {
    // Force empty reg expressions
    list<string> generatedRegExp;
    matchRegExInit(generatedRegExp,true);
    string msg = "Result does not match with expected output";
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("Ask") == "Ask"));

    // Test very long word 
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("Askkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk") == "Ask*"));

    // Test Ctrl characters
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("Ask") == "Ask*"));

  }

  void test_weirdcases()
  {
    list<string> generatedRegExp;

    // Should match with *.... 
    generatedRegExp.push_back("**");
    generatedRegExp.push_back("1*");
    generatedRegExp.push_back("***");
    generatedRegExp.push_back("22*");
    generatedRegExp.push_back("333*");
    matchRegExInit(generatedRegExp,false);
    string msg = "Result does not match with expected output";
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("*joe") == "**"));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("*") == ""));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("1") == ""));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("11") == "1*"));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("**") == "**"));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("***") == "**"));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("111") == "1*"));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("222") == "22*"));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("3333") == "333*"));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("333") == ""));
  }

  void test_basic()
  {
    string msg = "Result does not match with expected output";
    
    map<string,string> testcases;
    testcases["Ask"] = "Ask";
    testcases["AskBot"] = "Ask*";
    testcases["Mozilla/2.0 (compatible; Ask Jeeves)"] = "Mozilla/2.0 (compatible; Ask Jeeves)";
    testcases["Baiduspider-adserver"] = "Baiduspider-ads*";
    testcases["Baiduspider-aserver"] = "";
    testcases["Mozilla/1.0 (compatible; Ask Jeeves"] = "";
    testcases["Baiduspider-favo"] = "";
    testcases["askbot"] = "";

    map<string,string>::iterator testcasesItr = testcases.begin();
    for( ; testcasesItr != testcases.end(); testcasesItr++)
    {
      string result = matchRegEx(testcasesItr->first);
      string msg2 = msg + " [arg=" + (testcasesItr->first) + "][expect=" + (testcasesItr->second) + "][result=" + result + "]";
      CPPUNIT_ASSERT_MESSAGE(msg2.c_str(),(result==(testcasesItr->second)));      
    }
  }

  map<string,list<string> > overlapCache;
  list<string> generatePermutation(const std::string input)
  {
    list<string> result;
    try {

      if(input.length() <= 0)
        return result;
      if(input.length() == 1)
      {
        result.push_back(input);
        return result;
      }

      // generate permutation
      for(int i=0; i < input.length(); i++)
      {
        string tmp;
        if(i > 0)
        {
          std::string prefix;
          std::string suffix;
          prefix = input.substr(i,(input.length() - i));
          suffix = input.substr(0,i);
          tmp = prefix + suffix;
        }
        else
        {
          tmp = input;
        }

        std::string prefix = tmp.substr(0,1);
        std::string subinput = tmp.substr(1,tmp.length()-1);
        list<string> tmpR;
        if(overlapCache.find(subinput) == overlapCache.end())
        {
          overlapCache[subinput] = generatePermutation(subinput);        
        }
        tmpR = overlapCache[subinput];      
        list<string>::iterator tmpRItr = tmpR.begin();
        for( ; tmpRItr != tmpR.end(); tmpRItr++)
        {
          string tmp2 = prefix + (*tmpRItr);
          result.push_back(tmp2);
        }
      }
    }
    catch(std::bad_alloc *e)
    {
      CPPUNIT_ASSERT_MESSAGE("Exceeded memory size for Permutatio allocation",false);
      return result;
    }
    return result;
  }

  void test_trietree()
  {
    TrieTree tt;
    tt.insert("Ask");
    tt.insert("Z");
    tt.insert("bo");

    string msg = "Result does not match";

    // Success cases

    // Testing 3 letters prefix
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(tt.findPrefix("AskBot") == "Ask*"));
    // Testing 2 letters prefix
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(tt.findPrefix("bobo") == "bo*"));
    // Testing 1 letter prefix
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(tt.findPrefix("ZigZag") == "Z*"));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(tt.findPrefix("Zoe") == "Z*"));

    // Failure cases
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(tt.findPrefix("ba") == ""));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(tt.findPrefix("bo") == ""));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(tt.findPrefix("A") == ""));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(tt.findPrefix("Z") == ""));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(tt.findPrefix("Ask") == ""));
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(tt.findPrefix("") == ""));
  }

  /**
   *  We want to test with large number of inputs, but 
   *  given a small regex set.
   */
  void test_performanceLotsOfInput()
  {
    string msg = "Result does not match with expected output";
    
    map<string,string> testcases;
    testcases["Ask"] = "Ask";
    testcases["AskBot"] = "Ask*";
    testcases["Mozilla/2.0 (compatible; Ask Jeeves)"] = "Mozilla/2.0 (compatible; Ask Jeeves)";
    testcases["Baiduspider-adserver"] = "Baiduspider-ads*";
    testcases["Baiduspider-aserver"] = "";
    testcases["Mozilla/1.0 (compatible; Ask Jeeves"] = "";
    testcases["Baiduspider-favo"] = "";
    testcases["askbot"] = "";

    // Basic Variables for time measurements
    ptime stime,etime;
    time_duration diff;
    int iterations = 10000;

    // Generates all test cases
    list<string> jtestcases = generatePermutation("abcdefg");

    std::cerr << "[start 50% failure]" << std::endl;
    stime = microsec_clock::local_time();

    /**
     *  Time - 50% success
     */
    for(int i = 0; i < iterations ; i++)
    {      
      map<string,string>::iterator testcasesItr = testcases.begin();
      for( ; testcasesItr != testcases.end(); testcasesItr++)
      {
        string result = matchRegEx(testcasesItr->first);
        string msg2 = msg + " [arg=" + (testcasesItr->first) + "][expect=" + (testcasesItr->second) + "][result=" + result + "]";
        CPPUNIT_ASSERT_MESSAGE(msg2.c_str(),(result==(testcasesItr->second)));      
      }
    }

    etime = microsec_clock::local_time();
    diff = etime - stime;
    std::cerr << "diff(seconds)=" << diff.total_seconds() << std::endl;
    std::cerr << "diff(milliseconds)=" << diff.total_milliseconds() << std::endl;
    std::cerr << "diff(nanoseconds)=" << diff.total_nanoseconds() << std::endl;
    std::cerr << std::endl;
    CPPUNIT_ASSERT_MESSAGE("Process took too long",diff.total_seconds() < 1);

    /**
     *  Time - exact match test
     */
    testcases.clear();
    testcases["Ask"] = "Ask";
    testcases["Mozilla/2.0 (compatible; Ask Jeeves)"] = "Mozilla/2.0 (compatible; Ask Jeeves)";

    std::cerr << "[start exact match test]" << std::endl;
    stime = microsec_clock::local_time();
    /**
     *  Time - 100% failure
     */
    for(int i = 0; i < iterations ; i++)
    {  
      map<string,string>::iterator testcasesItr = testcases.begin();
      for( ; testcasesItr != testcases.end(); testcasesItr++)
      {
        string result = matchRegEx(testcasesItr->first);
        string msg2 = msg + " [arg=" + (testcasesItr->first) + "][expect=" + (testcasesItr->second) + "][result=" + result + "]";
        CPPUNIT_ASSERT_MESSAGE(msg2.c_str(),(result==(testcasesItr->second)));      
      }    
    }

    etime = microsec_clock::local_time();
    diff = etime - stime;
    std::cerr << "diff(seconds)=" << diff.total_seconds() << std::endl;
    std::cerr << "diff(milliseconds)=" << diff.total_milliseconds() << std::endl;
    std::cerr << "diff(nanoseconds)=" << diff.total_nanoseconds() << std::endl;
    std::cerr << std::endl;

    CPPUNIT_ASSERT_MESSAGE("Process took too long",diff.total_seconds() < 1);

    /**
     *  Time - 100% failure
     */
    std::cerr << "[start 100% failure]" << std::endl;
    stime = microsec_clock::local_time();
    for(int i = 0; i < iterations ; i++)
    {      
      list<string>::iterator jtestcasesItr = jtestcases.begin();
      for( ; jtestcasesItr != jtestcases.end(); jtestcasesItr++)
      {
        string arg = *jtestcasesItr;
        matchRegEx(arg);
      }
    }

    etime = microsec_clock::local_time();
    diff = etime - stime;
    std::cerr << "diff(seconds)=" << diff.total_seconds() << std::endl;
    std::cerr << "diff(milliseconds)=" << diff.total_milliseconds() << std::endl;
    std::cerr << "diff(nanoseconds)=" << diff.total_nanoseconds() << std::endl;
    std::cerr << std::endl;

    CPPUNIT_ASSERT_MESSAGE("Process took too long",diff.total_seconds() < 30);

    /**
     *  Time - 100% failure but forces all checks
     */
    std::cerr << "[start 100% failure but forces all checks]" << std::endl;
    stime = microsec_clock::local_time();
    for(int i = 0; i < iterations ; i++)
    {      
      list<string>::iterator jtestcasesItr = jtestcases.begin();
      for( ; jtestcasesItr != jtestcases.end(); jtestcasesItr++)
      {
        string arg = *jtestcasesItr;
        arg += "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        CPPUNIT_ASSERT_MESSAGE("should be empty",matchRegEx(arg)=="");
      }
    }

    etime = microsec_clock::local_time();
    diff = etime - stime;
    std::cerr << "diff(seconds)=" << diff.total_seconds() << std::endl;
    std::cerr << "diff(milliseconds)=" << diff.total_milliseconds() << std::endl;
    std::cerr << "diff(nanoseconds)=" << diff.total_nanoseconds() << std::endl;
    std::cerr << std::endl;

    CPPUNIT_ASSERT_MESSAGE("Process took too long",diff.total_seconds() < 150);

  }

  /**
   *  We want to test with large number of regex, but 
   *  given a small number of inputs.
   */
  void test_performanceLargeRegex()
  {
    string msg = "Result does not match with expected output";
    
    map<string,string> testcases;
    testcases["Ask"] = "Ask";
    testcases["AskBot"] = "Ask*";
    testcases["Mozilla/2.0 (compatible; Ask Jeeves)"] = "Mozilla/2.0 (compatible; Ask Jeeves)";
    testcases["Baiduspider-adserver"] = "Baiduspider-ads*";
    testcases["Baiduspider-aserver"] = "";
    testcases["Mozilla/1.0 (compatible; Ask Jeeves"] = "";
    testcases["Baiduspider-favo"] = "";
    testcases["askbot"] = "";

    list<string> generatedRegExpFinal;
    list<string> generatedRegExp = generatePermutation("abcdefgh");
    list<string>::iterator generatedRegExpItr = generatedRegExp.begin();
    for( ; generatedRegExpItr != generatedRegExp.end(); generatedRegExpItr++)
    {
      generatedRegExpFinal.push_back((*generatedRegExpItr) + "*");
    }
    std::cerr << "start initialize" << std::endl;
    try {
      matchRegExInit(generatedRegExpFinal,false);
    }
    catch(std::bad_alloc *e)
    {
      CPPUNIT_ASSERT_MESSAGE("Exceeded memory size for Trie Tree allocation",false);
      return;
    }
    std::cerr << "start counter" << std::endl;
    ptime stime = microsec_clock::local_time();

    for(int i = 0; i < 10 ; i++)
    {
      map<string,string>::iterator testcasesItr = testcases.begin();
      for( ; testcasesItr != testcases.end(); testcasesItr++)
      {
        string result = matchRegEx(testcasesItr->first);
        string msg2 = msg + " [arg=" + (testcasesItr->first) + "][expect=" + (testcasesItr->second) + "][result=" + result + "]";
        CPPUNIT_ASSERT_MESSAGE(msg2.c_str(),(result==(testcasesItr->second)));
      }
    }
    ptime etime = microsec_clock::local_time();
    time_duration diff = etime - stime;
    std::cerr << "diff(seconds)=" << diff.total_seconds() << std::endl;
    std::cerr << "diff(milliseconds)=" << diff.total_milliseconds() << std::endl;
    std::cerr << "diff(nanoseconds)=" << diff.total_nanoseconds() << std::endl;

    CPPUNIT_ASSERT_MESSAGE("Process took too long",diff.total_seconds() < 1);

  }
    
}; // testMatchRegEx

// Execute and run unit test
int main(int argc, char** argv)
{
  CppUnit::TextUi::TestRunner runnerStdcerr;
  runnerStdcerr.addTest(testMatchRegEx::suite());
  runnerStdcerr.setOutputter(new CppUnit::CompilerOutputter( &runnerStdcerr.result(), cerr ));
  runnerStdcerr.run();
  return runnerStdcerr.result().testFailuresTotal();        
} 

