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
#include "AST.hpp"



void testAST() {
  ASTNode *three=new ASTNode(Token(INTEGER,"3"));
  ASTNode *c=new ASTNode(Token(VARIABLE,"c"));
  ASTNode *mul=new ASTNode(Token(MULTIPLYING,"*"),three,c);
  ASTNode *a=new ASTNode(Token(VARIABLE,"a"));
  ASTNode *lessEqual=new ASTNode(Token(RELATIONAL,"<="),mul,a);
  ASTNode *root=lessEqual;
  cout << root;
}


void testAST2() {
  ASTNode *four = new ASTNode(Token(INTEGER, "4"));
  ASTNode *e = new ASTNode(Token(VARIABLE, "e"));
  ASTNode *mul = new ASTNode(Token(MULTIPLYING, "*"), e, four);
  ASTNode *d = new ASTNode(Token(VARIABLE, "d"));
  ASTNode *sub = new ASTNode(Token(ADDING, "-"), d, mul);
  ASTNode *root = sub;
  cout << root;
  cout << endl;
}


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
    ASTNode *root = new ASTNode();
    bool b = expression(tokens, root);
    if (b) cout << "Success your code parses" << endl;
    else cout << "Failure your code does not parse" << endl;
    //testAST();
    testAST2();
  }
}