#include <iostream>
#include <fstream>
#include <regex>

#include "token.h"

using namespace std;

class Tokenizer {
  public:
  Token peekNext(string line) {
    return getNext(line);
  }
  Token getNext(string &line) {
    string realReg="^[-+]?\\d+\\.\\d+(E[-+]?\\d+)?"; // UnsignedReal
    regex rexp(realReg);
    string intReg="^[-+]?\\d+";
    regex iexp(intReg);    // Unsigned Integers
    string identReg="^[a-zA-Z]\\w*";
    regex idexp(identReg);
    smatch sm;
    Token t;
    while (line[0]==' ' || line[0]=='\t')  line=line.substr(1);
    while (line[0]=='{') {
      line=line.substr(1);
      while (line[0]!='}' && line.length()>0) line=line.substr(1);
      line=line.substr(1);
    }
    if(line.length() == 0 || line == "\n") return Token(EOL);

    string f1=line.substr(0,1);
    string f2=line.substr(0,2);
    string f3=line.substr(0,3);
    string f4=line.substr(0,4);
    string f5=line.substr(0,5);
    string f6=line.substr(0,6);
    string f8=line.substr(0,8);
    
    if (regex_search(line,sm,rexp)) t=Token(REAL,sm[0]);
    else if (regex_search(line,sm,iexp)) t=Token(INTEGER,sm[0]);
    else if (f2=="*=" || f2=="/=" || f2=="%=" || f2=="+=" ||
             f2=="-=" || f2=="&=" || f2=="^=" || f2=="|=") t=Token(ASSIGNMENT,f2);
    else if (f3=="<<=" || f3==">>=") t=Token(ASSIGNMENT,f3);

    else if (f2==">>" || f2=="<<") t=Token(SHIFT, f2);

    else if (f2=="<=" || f2==">=") t=Token(RELATIONAL,f2);

    else if (f2=="==" || f2=="!=") t=Token(EQUALITY,f2);

    else if (f2=="&&") t=Token(AND,f2);
    else if (f2=="||") t=Token(OR,f2);

    else if (f6=="sizeof") t=Token(UNARY,f6);
    else if (f2=="++" || f2=="--") t=Token(UNARY,f2);
    

    else if (f1=="*" || f1=="/" || f1=="%") t=Token(MULTIPLYING,f1);

    else if (f3=="int") t=Token(TYPE,f3);
    else if (f4=="void" || f4=="char" || f4=="long") t=Token(TYPE,f4);
    else if (f5=="short" || f5=="float") t=Token(TYPE,f5);
    else if (f6=="double" || f6=="signed") t=Token(TYPE,f6);
    else if (f8=="unsigned") t=Token(TYPE,f8);

    else if (regex_search(line,sm,idexp)) t=Token(VARIABLE,sm[0]);

    else if (f1=="=") t=Token(ASSIGNMENT,f1);

    else if(f1=="<" || f1==">") t=Token(RELATIONAL,f1);

    else if (f1=="+" || f1=="-") t=Token(ADDING,f1);

    else if (f1=="&" || f1=="*" || f1=="~" || f1=="!") t=Token(UNARY,f1);

    else if (f1=="?" || f1==":") t=Token(CONDITIONAL,f1);

    else if (f1=="|") t=Token(INCLUSIVE_OR,f1);
    else if (f1=="^") t=Token(EXCLUSIVE_OR,f1);

    else if (f1=="(") t=Token(OPENPAREN,f1);
    else if (f1==")") t=Token(CLOSEPAREN,f1);
    //else if (f1.length()==0) t=Token(EOL);
    else t=Token(ERROR,f1);
    line=line.substr(t.getValue().length());
    return t;
  }
};