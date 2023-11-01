#pragma once
#include <iomanip>

using namespace std;

#include "token.hpp"

const int LEFT=0;
const int RIGHT=1;

class ASTNode {
  Token t;
  vector<ASTNode *> children;
  public:
  ASTNode(Token newT=Token(),ASTNode *left=NULL,ASTNode *right=NULL) {
    t=newT;
    if (left!=NULL) add(left);
    if (right!=NULL) add(right);
  }
  friend ostream & operator << (ostream &out,ASTNode *p) {
    static int indent=0;
    int regularIndent=0;
    out << p->t  << '(' <<hex << (long)p << ')';
    if (p->children.size()>0) {
      indent+=2;
      out<< " has Children " << endl;
      bool first=true;
      for(auto child:p->children) {
        if (first) first=false;
        else out << endl;
        out << '+' <<setfill('-') << setw(indent) <<'>'<< setfill(' ')<< child;
      }
      indent-=2;
      if (indent<0)indent=0;
    } 
    return out;
  }
  void add(ASTNode *child) {
    children.push_back(child);
  }
};


