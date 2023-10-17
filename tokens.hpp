#pragma once
#include <iostream>
#include <fstream>

using namespace std;

#include "error.hpp"
#include "token.hpp"

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
    advance();
    Token t=Tokenizer::getNext(line);
    if (t.getToken()==EOL && done()) return Token(DONE);   
    return t; 
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