#include <sys/types.h>
#include <dirent.h>
#include <sstream>
#include <fstream>

#include "matchregex.h"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <cppunit/XmlOutputter.h>

using namespace std;

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
  CPPUNIT_TEST(test_basic);
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

