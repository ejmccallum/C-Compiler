#include <iostream>
#include <fstream>
#include <regex>

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
      while (line[0]==' ' || line[0]=='\t') {
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

string filename;
int linenumber;
bool success=true;

void error(string message) {
  cerr << filename <<':'<<linenumber<<':'<< 1 << " error "<< message<< endl;
  success=false;
}

bool parse(ostream &out,istream &fin) {
  string line;
  linenumber=0;
  success=true;
  Tokenizer scanner;
  while (!fin.eof()){
    linenumber++;
    getline(fin,line);
    while (line.length()>0) {
      Token t=scanner.getNext(line);
      if (t.getToken()==ERROR)
        error("Unrecognized character");
      else 
        out << t << endl;
    }
  }
  return success;
}

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

vector<TestCase *> testcases;
void runTests() {
  cout << "Running tests" << endl;
  for (auto t:testcases) {
    if (t->run()) cout << "Success" <<endl;
    else cout << "Failed" <<endl;
  }
}
void addTestcases() {
   testcases.push_back((TestCase *)new TestCaseFile("sort.pas","sort.txt"));
   testcases.push_back((TestCase *)new TestCaseFile("sortOld.pas","sortOld.txt",true));
}

int main(int argc,char **argv) {
  if (argc==2 && string(argv[1])=="test") {
    addTestcases(); 
    runTests();
  }
  else if (argc<3) {
    cout << "Usage: pascal.exe <input.pas> <output.pas>"<<endl;
    cout << "<input.pas> is a text file that is the pascal source code." <<endl;
    cout << "<output.pas> is the name of the executable"<<endl;
  } else {
    fstream fin;
    filename=argv[1];
    fin.open(filename);
    parse(cout,fin);
    fin.close();
  }
}