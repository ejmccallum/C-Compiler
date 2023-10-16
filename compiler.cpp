#include <iostream>
#include <fstream>
#include <regex>

using namespace std;


enum TokenType {OPENPAREN, CLOSEPAREN, OPENBRACKET, CLOSEBRACKET, UNARY, ASSIGNMENT, EOL,
                TYPE, RELATIONAL, SHIFT, MULTIPLYING, ADDING,
                CONDITIONAL, AND, OR, INCLUSIVE_OR, EXCLUSIVE_OR,
                EQUALITY, VARIABLE, INTEGER, REAL, COMMA, ERROR}; 

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

string filename;
int linenumber;
bool success=true;

bool error(string message) {
  cerr << filename <<':'<<linenumber<<':'<< 1 << " error "<< message<< endl;
  return false;
}


class Tokens:public Tokenizer{
  string line;
  ofstream fout;
  ifstream fin;
  void advance() {
    if (line.length()>0) return;
    while ((line.length()==0 || peekNext().getToken()==EOL) && !done() ) {
      linenumber++;
      getline(fin,line);
    }
  }
  public:
  string getLine() { return line; }
  bool done() {return fin.eof(); }
  Tokens(string iFilename,string oFilename) {
    linenumber=0;
    success=true;
    line="";
    filename=iFilename;
    fin.open(iFilename);
    fout.open(oFilename);
    advance();
  }
  Token getNext(){ 
    while (Tokenizer::peekNext(line).getToken()!=EOL)
      advance();
    return Tokenizer::getNext(line); 
  }
  Token peekNext() {
    string tempLine=line;
    int tempLinenumber=linenumber;
    advance();
    Token tempToken=Tokenizer::peekNext(line);
    line=tempLine;
    linenumber=tempLinenumber;
    return tempToken;
  }
  ~Tokens() {
     fin.close();
     fout.close();
  }
};

bool expression(Tokens &tokens);

bool primaryExpression(Tokens &tokens) {
    Token t = tokens.peekNext();
    if (t.getToken() == VARIABLE || t.getToken() == INTEGER || t.getToken() == REAL || t.getToken() == OPENPAREN) {
        tokens.getNext();
        return true;
    } else {
        return error("Expected identifier, constant, string, or expression in parenthesis");
    }
}

bool postfixExpression(Tokens &tokens) {
    cout << "Postfix Expression " << tokens.getLine() << endl;
    Token t = tokens.peekNext();
    if (primaryExpression(tokens)) {
        while (true) {
            if (t.getToken() == OPENBRACKET) {
                tokens.getNext();
                if (!expression(tokens)) {
                    return error("Expected expression inside []");
                }
                if (t.getToken() != CLOSEBRACKET) {
                    return error("Expected ']'");
                }
            } else if (t.getToken() == OPENPAREN) {
                tokens.getNext();
                if (t.getToken() != CLOSEPAREN) {
                    if (!expression(tokens)) {
                        return error("Expected expression inside ()");
                    }
                }
                if (t.getToken() != CLOSEPAREN) {
                    return error("Expected ')'");
                }
            } else if (t.getToken() == UNARY) {
                tokens.getNext();
            } else {
                break;
            }
        }
        return true;
    }
    return false;
}

bool castExpression(Tokens &tokens);

bool unaryExpression(Tokens &tokens) {
    Token t = tokens.peekNext();
    if (t.getToken() == UNARY) {
        tokens.getNext();
        return castExpression(tokens);
    } else {
        return postfixExpression(tokens);
    }
}

bool castExpression(Tokens &tokens) {
    return unaryExpression(tokens);
}

bool multiplicativeExpression(Tokens &tokens) {
    if (castExpression(tokens)) {
        Token t = tokens.peekNext();
        if (t.getToken() == MULTIPLYING) {
            tokens.getNext();
            return multiplicativeExpression(tokens);
        } else {
            return true;
        }
    } else {
        return error("Expected cast expression");
    }
}

bool additiveExpression(Tokens &tokens) {
    if (multiplicativeExpression(tokens)) {
        Token t = tokens.peekNext();
        if (t.getToken() == ADDING) {
            tokens.getNext();
            return additiveExpression(tokens);
        } else {
            return true;
        }
    } else {
        return error("Expected multiplicative expression");
    }
}

bool shiftExpression(Tokens &tokens) {
    if (additiveExpression(tokens)) {
        Token t = tokens.peekNext();
        if (t.getToken() == SHIFT) {
            tokens.getNext();
            return shiftExpression(tokens);
        } else {
            return true;
        }
    } else {
        return error("Expected additive expression");
    }
}

bool relationalExpression(Tokens &tokens) {
    if (shiftExpression(tokens)) {
        Token t = tokens.peekNext();
        if (t.getToken() == RELATIONAL) {
            tokens.getNext();
            return relationalExpression(tokens);
        } else {
            return true;
        }
    } else {
        return error("Expected shift expression");
    }
}

bool equalityExpression(Tokens &tokens) {
    if (relationalExpression(tokens)) {
        Token t = tokens.peekNext();
        if (t.getToken() == EQUALITY) {
            tokens.getNext();
            return equalityExpression(tokens);
        } else {
            return true;
        }
    } else {
        return error("Expected relational expression");
    }
}

bool andExpression(Tokens &tokens) {
    if (equalityExpression(tokens)) {
        Token t = tokens.peekNext();
        if (t.getToken() == AND) {
            tokens.getNext();
            return andExpression(tokens);
        } else {
            return true;
        }
    } else {
        return error("Expected equality expression");
    }
}

bool exclusiveOrExpression(Tokens &tokens) {
    if (andExpression(tokens)) {
        Token t = tokens.peekNext();
        if (t.getToken() == EXCLUSIVE_OR) {
            tokens.getNext();
            return exclusiveOrExpression(tokens);
        } else {
            return true;
        }
    } else {
        return error("Expected and expression");
    }
}

bool inclusiveOrExpression(Tokens &tokens) {
    if (exclusiveOrExpression(tokens)) {
        Token t = tokens.peekNext();
        if (t.getToken() == INCLUSIVE_OR) {
            tokens.getNext();
            return inclusiveOrExpression(tokens);
        } else {
            return true;
        }
    } else {
        return error("Expected exclusive or expression");
    }
}

bool logicalAndExpression(Tokens &tokens) {
  cout << "Logical And Expression " << tokens.getLine() << endl;
  if(inclusiveOrExpression(tokens))
  {
    Token t = tokens.peekNext();
    if(t.getToken() == AND)
    {
      tokens.getNext();
      return logicalAndExpression(tokens);
    }
    else
    {
      return error("Expected logical and operator");
    }
  }
  else
  {
    return error("Expected inclusive or expression");
  }
  
}

bool logicalOrExpression(Tokens &tokens)
{
  cout << "Logical Or Expression " << tokens.getLine() << endl;
  if(logicalAndExpression(tokens))
  {
    Token t = tokens.peekNext();
    if(t.getToken() == OR)
    {
      tokens.getNext();
      return logicalOrExpression(tokens);
    }
    else
    {
      return error("Expected logical or operator");
    }
  }
  else
  {
    return error("Expected logical and expression");
  }

}

bool conditionalExpression(Tokens &tokens){
  cout << "Conditional Expression " << tokens.getLine() << endl;
  if(logicalOrExpression(tokens))
  {
    Token t = tokens.peekNext();
    if (t.getToken() == CONDITIONAL)
    {
      tokens.getNext();
      if(expression(tokens))
      {
        t = tokens.peekNext();
        if(t.getToken() == CONDITIONAL)
        {
          tokens.getNext();
          return conditionalExpression(tokens);
        }
        else
        {
          return error("Expected colon after expression");
        }
      }
      else
      {
        return error("Expected expression");
      }
      
    }
    else
    {
      return true;
    }
  }
  else
  {
    return error("Expected logical or expression");
  }
}

bool assignmentExpression(Tokens &tokens){
  cout << "Assignment Expression " << tokens.getLine() << endl;
  if(conditionalExpression(tokens))
  {
    return true;
  }
  else if (unaryExpression(tokens))
  {
    Token t = tokens.peekNext();
    if(t.getToken() == ASSIGNMENT)
    {
      tokens.getNext();
      return assignmentExpression(tokens);
    }
    else
    {
      return error("Expected assignment operator");
    }

  }
  else
  {
    return error("Expected conditional or unary expression");
  }

}

bool expression(Tokens &tokens){
  cout << "Expression " << tokens.getLine();
  if(assignmentExpression(tokens))
  {
    Token t=tokens.peekNext();
    if(t.getToken() == COMMA){
      tokens.getNext();
      return expression(tokens);
    }
    else{
      return true;
    }

  }
  else{
    return error("Expected assignment expression");
  }
}


// New bnf


// //<expression> ::= <assignment-expression>  |  <assignment-expression> , <expression>

// //<assignment-expression> ::= <conditional-expression>
//                           | <unary-expression> <assignment-operator> <assignment-expression>

// <conditional-expression> ::= <logical-or-expression>
//                           | <logical-or-expression> ? <expression> : <conditional-expression>

// <logical-or-expression> ::= <logical-and-expression>
//                           | <logical-or-expression> || <logical-and-expression>

// <logical-and-expression> ::= <inclusive-or-expression>
//                            | <logical-and-expression> && <inclusive-or-expression>

// <inclusive-or-expression> ::= <exclusive-or-expression>
//                             | <inclusive-or-expression> | <exclusive-or-expression>

// <exclusive-or-expression> ::= <and-expression>
//                             | <exclusive-or-expression> ^ <and-expression>

// <and-expression> ::= <equality-expression>
//                    | <and-expression> & <equality-expression>

// <equality-expression> ::= <relational-expression>
//                         | <equality-expression> == <relational-expression>
//                         | <equality-expression> != <relational-expression>

// <relational-expression> ::= <shift-expression>
//                           | <relational-expression> < <shift-expression>
//                           | <relational-expression> > <shift-expression>
//                           | <relational-expression> <= <shift-expression>
//                           | <relational-expression> >= <shift-expression>

// <shift-expression> ::= <additive-expression>
//                      | <shift-expression> << <additive-expression>
//                      | <shift-expression> >> <additive-expression>

// <additive-expression> ::= <multiplicative-expression>
//                         | <additive-expression> + <multiplicative-expression>
//                         | <additive-expression> - <multiplicative-expression>

// <multiplicative-expression> ::= <cast-expression>
//                               | <multiplicative-expression> * <cast-expression>
//                               | <multiplicative-expression> / <cast-expression>
//                               | <multiplicative-expression> % <cast-expression>

// <cast-expression> ::= <unary-expression>
// <unary-expression> ::= <postfix-expression>
//                      | ++ <unary-expression>
//                      | -- <unary-expression>
//                      | <unary-operator> <cast-expression>
//                      | sizeof <unary-expression>


// <postfix-expression> ::= <primary-expression>
//                        | <postfix-expression> [ <expression> ]
//                        | <postfix-expression> ( {<assignment-expression>}* )
//                        | <postfix-expression> . <identifier>
//                        | <postfix-expression> -> <identifier>
//                        | <postfix-expression> ++
//                        | <postfix-expression> –

// <primary-expression> ::= <identifier>
//                        | <constant>
//                        | <string>
//                        | ( <expression> )








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