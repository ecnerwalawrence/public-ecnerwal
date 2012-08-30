#include "matchregex.h"
#include "matchregex_priv.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/tokenizer.hpp>

using namespace std;

// Execute and run unit test
int main(int argc, char** argv)
{

  /**
   *  Default - regular expression can be override by a config
   *            "regexp_dataset.txt"
   */
  string filename = "./regexp_dataset.txt";
  ifstream filestreamI;
  filestreamI.open(filename.c_str(), fstream::out);
  if(filestreamI.is_open())
  {
    std::stringstream bufferStreamInput;
    while(filestreamI.good())
      bufferStreamInput << (char) filestreamI.get();
    string buffer = bufferStreamInput.str();
    filestreamI.close();
    
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\n");
    tokenizer tokens(buffer,sep);
    std::list<std::string> regexps;
    for( tokenizer::iterator tok_itr = tokens.begin(); tok_itr != tokens.end(); tok_itr++)
    {
      if((*tok_itr).length() <= 0) continue;
      regexps.push_back(*tok_itr);
    }
    matchRegExInit(regexps,true);      
  }
  
  string input = "";
  cout << "[Enter phrase]" << std::endl;
  getline(cin,input,'\n');
  cout << "[Output]" << std::endl << matchRegEx(input) << endl; 
  return 1;
} 
