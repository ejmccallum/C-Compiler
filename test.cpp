#include <iostream>
#include <fstream>

using namespace std;

// class TestCase {
//   public:
//   virtual bool run()=0; // Test cases when true succeeded
// };

// class TestCaseFile:public TestCase {
//   string filein,fileout;
//   bool shouldFail;
//   public:
//   TestCaseFile(string newFilein,string newFileout,bool newShouldFail=false) {
//     filein=newFilein;
//     fileout=newFileout;
//     shouldFail=newShouldFail;
//   }
//   bool run() {
//     ifstream in;
//     ofstream out;
//     filename=filein;
//     in.open(filename);
//     out.open(fileout);
//     bool result=parse(out,in);
//     in.close();
//     out.close();
//     if (shouldFail && !result) result=true;
//     return result;
//   } 
// };

// vector<TestCase *> testcases;
// void runTests() {
//   cout << "Running tests" << endl;
//   for (auto t:testcases) {
//     if (t->run()) cout << "Success" <<endl;
//     else cout << "Failed" <<endl;
//   }
// }
// void addTestcases() {
//    testcases.push_back((TestCase *)new TestCaseFile("sort.pas","sort.txt"));
//    testcases.push_back((TestCase *)new TestCaseFile("sortOld.pas","sortOld.txt",true));
// }