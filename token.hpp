#pragma once
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

using namespace std;



enum TokenType {OPENPAREN, CLOSEPAREN, OPENBRACKET, CLOSEBRACKET, UNARY, ASSIGNMENT, EOL,
                TYPE, RELATIONAL, SHIFT, MULTIPLYING, ADDING,
                CONDITIONAL, AND, OR, INCLUSIVE_OR, EXCLUSIVE_OR,
                EQUALITY, VARIABLE, INTEGER, REAL, COMMA, ERROR, DONE}; 

class Token {
  TokenType token;
  string value;
  public:
  Token(TokenType newToken=ERROR,string newValue="") {
    token=newToken;
    value=newValue;
  }
  string getValue() { return value;}
  TokenType getToken() {return token;}
  string str() {
    switch(token) {
      case MULTIPLYING: return "MULTIPLYING";
      case ADDING: return "ADDING";
      case RELATIONAL: return "RELATIONAL";
      case OPENPAREN: return "OPENPAREN";
      case CLOSEPAREN: return "CLOSEPAREN";
      case OPENBRACKET: return "OPENBRACKET";
      case DONE: return "DONE";
      case CLOSEBRACKET: return "CLOSEBRACKET";
      case VARIABLE: return "VARIABLE";
      case INTEGER: return "INTEGER";
      case REAL: return "REAL";
      case UNARY: return "UNARY";
      case ASSIGNMENT: return "ASSIGNMENT";
      case TYPE: return "TYPE";
      case SHIFT: return "SHIFT";
      case CONDITIONAL: return "CONDITIONAL";
      case AND: return "AND";
      case OR: return "OR";
      case INCLUSIVE_OR: return "INCLUSIVE_OR";
      case EXCLUSIVE_OR: return "EXCLUSIVE_OR";
      case EQUALITY: return "EQUALITY";
      case COMMA: return "COMMA";
      case ERROR: return "ERROR";
      case EOL: return "EOL";
      default: return "UNRECOGNIZED";
    }
  }
  friend ostream & operator <<(ostream &out,Token t) {
    return out << t.str() << " " << t.value;
  }
};


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

    if(line == "\n" || line == "\r\n" || line == "\r") {
      line = ""; 
      return Token(EOL);
    }

    bool changed = true;
    while (changed) {
      changed=false;
      while (line[0]==' ' || line[0]=='\t' || line[0]=='\r' || line[0]=='\n') {
        line=line.substr(1);
        changed=true;
      }
      while (line[0]=='{') {
        line=line.substr(1);
        while (line[0]!='}' && line.length()>0) line=line.substr(1);
        line=line.substr(1);
        changed=true;
      }
    }


    if(line.length() == 0) return Token(EOL);

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

    else if (f1==",") t=Token(COMMA,f1);
    
    else if (f1=="|") t=Token(INCLUSIVE_OR,f1);
    else if (f1=="^") t=Token(EXCLUSIVE_OR,f1);

    else if (f1=="(") t=Token(OPENPAREN,f1);
    else if (f1==")") t=Token(CLOSEPAREN,f1);
    else if (f1=="[") t=Token(OPENBRACKET,f1);
    else if (f1=="]") t=Token(CLOSEBRACKET,f1);
    //else if (f1.length()==0) t=Token(EOL);
    else t=Token(ERROR,f1);
    line=line.substr(t.getValue().length());
    return t;
  }
};