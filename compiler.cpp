#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <iomanip>

using namespace std;

#include "token.hpp"
#include "tokens.hpp"
#include "error.hpp"
#include "parser.hpp"




int main(int argc,char **argv) {
  if (argc==2 && string(argv[1])=="test") {
    // addTestcases(); 
    // runTests();
  }
  else if (argc<3) {
    cout << "Usage: pascal.exe <input.pas> <output.pas>"<<endl;
    cout << "<input.pas> is a text file that is the pascal source code." <<endl;
    cout << "<output.pas> is the name of the executable"<<endl;
  } else {
    Tokens tokens("sort.txt",argv[2]);
    bool b = expression(tokens);
    if (b) cout << "Success your code parses" << endl;
    else cout << "Failure your code does not parse" << endl;
  }
}