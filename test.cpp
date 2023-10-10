#include <iostream>
#include <fstream>

using namespace std;

class TestCase {
  public:
  virtual bool run()=0; // Test cases when true succeeded
};

class TestCaseFile:public TestCase {
  string filein,fileout;
  bool shouldFail;
  public:
  TestCaseFile(string newFilein,string newFileout,bool newShouldFail=false) {
    filein=newFilein;
    fileout=newFileout;
    shouldFail=newShouldFail;
  }
  bool run() {
    ifstream in;
    ofstream out;
    filename=filein;
    in.open(filename);
    out.open(fileout);
    bool result=parse(out,in);
    in.close();
    out.close();
    if (shouldFail && !result) result=true;
    return result;
  } 
};