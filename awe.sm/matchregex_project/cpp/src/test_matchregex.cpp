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
  // CPPUNIT_TEST(test_emptyInput);
  // CPPUNIT_TEST(test_basic);
  CPPUNIT_TEST(test_performance);
  // CPPUNIT_TEST(test_performanceRegex);
  // End test suite definitions
  CPPUNIT_TEST_SUITE_END() ;

public:
  
  // Clean-up and initializer
  void setUp() {}
  void tearDown(){}

  void test_emptyInput()
  {
    string msg = "Result does not match with expected output";
    CPPUNIT_ASSERT_MESSAGE(msg.c_str(),(matchRegEx("") == ""));
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
    return result;
  }

  void test_performance()
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

#ifdef TEST1
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

#endif // TEST1


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

  }

  void test_performanceRegex()
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
    list<string> generatedRegExp = generatePermutation("abcdefghijklm");
    list<string>::iterator generatedRegExpItr = generatedRegExp.begin();
    for( ; generatedRegExpItr != generatedRegExp.end(); generatedRegExpItr++)
    {
      generatedRegExpFinal.push_back((*generatedRegExpItr) + "*");
    }
    matchRegExInit(generatedRegExpFinal,false);
    std::cerr << "start counter" << std::endl;

    ptime stime = microsec_clock::local_time();

    for(int i = 0; i < 1000 ; i++)
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

