#include <iostream>
#include <fstream>

using namespace std;

enum TokenType {OPENPAREN, CLOSEPAREN, UNARY, ASSIGNMENT, EOL,
                TYPE, RELATIONAL, SHIFT, MULTIPLYING, ADDING,
                CONDITIONAL, AND, OR, INCLUSIVE_OR, EXCLUSIVE_OR,
                EQUALITY, VARIABLE, INTEGER, REAL, ERROR}; 

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
      case ERROR: return "ERROR";
      case EOL: return "EOL";
      default: return "UNRECOGNIZED";
    }
  }
  friend ostream & operator <<(ostream &out,Token t) {
    return out << t.str() << " " << t.value;
  }
};
