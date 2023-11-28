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
#include "testAST.hpp"

string uniqueLabel() {
  static int labelCount=0;
  string s=to_string(labelCount);
  string retval="Comp"+s;
  labelCount++;
  return retval;
}


void compile(ASTNode *root, string &s) {
  switch (root->getToken().getToken())
  {
    case RELATIONAL:
      if(root->left() == NULL || root->right() == NULL)
      {
        cerr << "Relational without operands" << endl;
        return;
      }
      else
      {
        stringstream ss;
        string a,b;
        compile(root->left(), a);
        compile(root->right(), b);
        ss << b;
        ss << a;
        ss << "   pop %rax\n";
        ss << "   pop %rbx\n";
        ss << "   cmp %rax, %rbx\n";
        ss << "   mov $1, %rax\n";
        string label = uniqueLabel();
        if(root->getToken().getValue()=="<") {
            ss << "    jl "<< label << "\n";
        } else if(root->getToken().getValue()==">"){
            ss << "    jg "<< label << "\n";
        }else if(root->getToken().getValue()=="<=") {
            ss << "    jle "<< label << "\n";
        }else if(root->getToken().getValue()==">=") {
            ss << "    jge "<< label << "\n";
        }else if(root->getToken().getValue()=="<>") {
            ss << "    jne "<< label << "\n";
        }else if(root->getToken().getValue()=="="){
            ss << "    jeq "<< label << "\n";
        }else if(root->getToken().getValue()=="in"){
            
        }
        ss << "    mov $0, %rax\n";
        ss << label << ":\n";
        ss << "    push %rax\n";
        s = ss.str();
      }
      break;
    case EQUALITY:
      if(root->left() == NULL || root->right() == NULL)
      {
        cerr << "Equality without operands" << endl;
        return;
      }
      else
      {
        stringstream ss;
        string a,b;
        compile(root->left(), a);
        compile(root->right(), b);
        ss << b;
        ss << a;
        ss << "   pop %rax\n";
        ss << "   pop %rbx\n";
        ss << "   cmp %rax, %rbx\n";
        ss << "   mov $1, %rax\n";
        string label = uniqueLabel();
        if(root->getToken().getValue()=="=") {
            ss << "    jeq "<< label << "\n";
        } else if(root->getToken().getValue()=="<>"){
            ss << "    jne "<< label << "\n";
        }
        ss << "    mov $0, %rax\n";
        ss << label << ":\n";
        ss << "    push %rax\n";
        s = ss.str();
      }
      break;
    case SHIFT:
      if(root->left() == NULL || root->right() == NULL)
      {
        cerr << "Shift without operands" << endl;
        return;
      }
      else
      {
        stringstream ss;
        string a,b;
        compile(root->left(), a);
        compile(root->right(), b);
        ss << b;
        ss << a;
        ss << "   pop %rax\n";
        ss << "   pop %rbx\n";
        ss << "   mov %rax, %rcx\n";
        ss << "   mov %rbx, %rax\n";
        if(root->getToken().getValue()=="<<") {
            ss << "    shl %cl, %rax\n";
        } else if(root->getToken().getValue()==">>"){
            ss << "    shr %cl, %rax\n";
        }
        ss << "    push %rax\n";
        s = ss.str();
      }
      break;
    case ADDING:
      if(root->left() == NULL || root->right() == NULL)
      {
        cerr << "Adding without operands" << endl;
        return;
      }
      else
      {
        stringstream ss;
        string a,b;
        compile(root->left(), a);
        compile(root->right(), b);
        ss << b;
        ss << a;
        ss << "   pop %rax\n";
        ss << "   pop %rbx\n";
        ss << "   mov %rax, %rcx\n";
        ss << "   mov %rbx, %rax\n";
        if(root->getToken().getValue()=="+") {
            ss << "    add %cl, %rax\n";
        } else if(root->getToken().getValue()=="-"){
            ss << "    sub %cl, %rax\n";
        }
        ss << "    push %rax\n";
        s = ss.str();
      }
      break;
    case MULTIPLYING:
      if(root->left() == NULL || root->right() == NULL)
      {
        cerr << "Multiplying without operands" << endl;
        return;
      }
      else
      {
        stringstream ss;
        string a,b;
        compile(root->left(), a);
        compile(root->right(), b);
        ss << b;
        ss << a;
        ss << "   pop %rax\n";
        ss << "   pop %rbx\n";
        ss << "   mov %rax, %rcx\n";
        ss << "   mov %rbx, %rax\n";
        if(root->getToken().getValue()=="*") {
            ss << "    mul %cl, %rax\n";
        } else if(root->getToken().getValue()=="/"){
            ss << "    div %cl, %rax\n";
        } else if(root->getToken().getValue()=="%"){
            ss << "    mod %cl, %rax\n";
        }
        ss << "    push %rax\n";
        s = ss.str();
      }
      break;
    case INCLUSIVE_OR:
      if(root->left() == NULL || root->right() == NULL)
      {
        cerr << "Inclusive or without operands" << endl;
        return;
      }
      else
      {
        stringstream ss;
        string a,b;
        compile(root->left(), a);
        compile(root->right(), b);
        ss << b;
        ss << a;
        ss << "   pop %rax\n";
        ss << "   pop %rbx\n";
        ss << "   mov %rax, %rcx\n";
        ss << "   mov %rbx, %rax\n";
        if(root->getToken().getValue()=="|") {
            ss << "    or %cl, %rax\n";
        }
        ss << "    push %rax\n";
        s = ss.str();
      }
      break;
    case EXCLUSIVE_OR:
      if(root->left() == NULL || root->right() == NULL)
      {
        cerr << "Exclusive or without operands" << endl;
        return;
      }
      else
      {
        stringstream ss;
        string a,b;
        compile(root->left(), a);
        compile(root->right(), b);
        ss << b;
        ss << a;
        ss << "   pop %rax\n";
        ss << "   pop %rbx\n";
        ss << "   mov %rax, %rcx\n";
        ss << "   mov %rbx, %rax\n";
        if(root->getToken().getValue()=="^") {
            ss << "    xor %cl, %rax\n";
        }
        ss << "    push %rax\n";
        s = ss.str();
      }
      break;
    case AND:
      if(root->left() == NULL || root->right() == NULL)
      {
        cerr << "And without operands" << endl;
        return;
      }
      else
      {
        stringstream ss;
        string a,b;
        compile(root->left(), a);
        compile(root->right(), b);
        ss << b;
        ss << a;
        ss << "   pop %rax\n";
        ss << "   pop %rbx\n";
        ss << "   mov %rax, %rcx\n";
        ss << "   mov %rbx, %rax\n";
        if(root->getToken().getValue()=="&") {
            ss << "    and %cl, %rax\n";
        }
        ss << "    push %rax\n";
        s = ss.str();
      }
      break;
    case ASSIGNMENT:
      if(root->left() == NULL || root->right() == NULL)
      {
        cerr << "Assignment without operands" << endl;
        return;
      }
      else
      {
        stringstream ss;
        string a,b;
        compile(root->right(), a);
        compile(root->left(), b);
        ss << a;
        ss << b;
        ss << "   pop %rax\n";
        ss << "   pop %rbx\n";
        if(root->getToken().getValue()=="=") {
            ss << "    mov %rax, %rbx\n";
        } else if(root->getToken().getValue()=="*="){
            ss << "    mul %rbx, %rax\n";
        } else if(root->getToken().getValue()=="/="){
            ss << "    div %rbx, %rax\n";
        } else if(root->getToken().getValue()=="+="){
            ss << "    add %rbx, %rax\n";
        } else if(root->getToken().getValue()=="-="){
            ss << "    sub %rbx, %rax\n";
        } else if(root->getToken().getValue()=="%="){
            ss << "    mod %rbx, %rax\n";
        } else if(root->getToken().getValue()=="<<="){
            ss << "    shl %rbx, %rax\n";
        } else if(root->getToken().getValue()==">>="){
            ss << "    shr %rbx, %rax\n";
        } else if(root->getToken().getValue()=="&="){
            ss << "    and %rbx, %rax\n";
        } else if(root->getToken().getValue()=="^="){
            ss << "    xor %rbx, %rax\n";
        } else if(root->getToken().getValue()=="|="){
            ss << "    or %rbx, %rax\n";
        }
        ss << "    push %rax\n";
        s = ss.str();
      }
      break;
    case INTEGER:
      s = "    mov $" + root->getToken().getValue() + ", %rax\n";
      s += "    push %rax\n";
      break;
    case REAL:
      s = "    mov $" + root->getToken().getValue() + ", %rax\n";
      s += "    push %rax\n";
      break;
    default:
      break;
}
}

void compileHelper(ASTNode *root,string &s) {
  string preAmble=
    "    .data\n"
    "    .align 4\n"
    "  	.section	.rodata\n"
    "  format: .string	\"%li\\n\"\n"
    "  	.text\n"
    "  	.globl	main\n"
    "  main:\n"
    "  	endbr64\n"
    "  	push	%rbp\n";
  string codeAssembly;
  compile(root,codeAssembly);
    string postAmble=
    "    pop  %rax\n"
    "    mov	%rax, %rsi\n"
	  "    lea	format(%rip), %rax\n"
	  "    mov	%rax, %rdi\n"
	  "    mov	$0, %rax\n"
	  "    call	printf@PLT\n"
    "    mov	$0, %rax\n"
    "    pop	%rbp\n"
    "    ret\n"
    "    .section	.note.GNU-stack,\"\",@progbits\n";
  s=preAmble+codeAssembly+postAmble;
}



void interpret(ASTNode *root, double &result){
  switch(root->getToken().getToken())
  {
    case RELATIONAL:
      if(root->left()==NULL || root->right()==NULL)
      {
        cerr << "Error: Relational operator missing operands" << endl;
        return;
      } else{
        double a, b;
        interpret(root->left(), a);
        interpret(root->right(), b);
        if(root->getToken().getValue()=="<=")
        {
          if(a<=b) result = 1;
          else result = 0;
        } else if(root->getToken().getValue()=="<")
        {
          if(a<b) result = 1;
          else result = 0;
        } else if(root->getToken().getValue()==">=")
        {
          if(a>=b) result = 1;
          else result = 0;
        } else if(root->getToken().getValue()==">")
        {
          if (a>b) result = 1;
          else result = 0;
        }
      }
      break;
    case EQUALITY:
      if(root->left()==NULL || root->right()==NULL)
      {
        cerr << "Error: Equality operator missing operands" << endl;
        return;
      } else{
        double a, b;
        interpret(root->left(), a);
        interpret(root->right(), b);
        if(root->getToken().getValue()=="==")
        {
          if(a==b) result = 1;
          else result = 0;
        } else if(root->getToken().getValue()=="!=")
        {
          if(a!=b) result = 1;
          else result = 0;
        }
      }
      break;
    case SHIFT:
      if(root->left()==NULL || root->right()==NULL)
      {
        cerr << "Error: Shift operator missing operands" << endl;
        return;
      } else{
        double a, b;
        interpret(root->left(), a);
        interpret(root->right(), b);
        if(root->getToken().getValue()=="<<")
        {
          result = (int)a << (int)b;
        } else if(root->getToken().getValue()==">>")
        {
          result = (int)a >> (int)b;
        }
      }
      break;
    case ADDING:
      if(root->left()==NULL || root->right()==NULL)
      {
        cerr << "Error: Adding operator missing operands" << endl;
        return;
      } else{
        double a, b;
        interpret(root->left(), a);
        interpret(root->right(), b);
        if(root->getToken().getValue()=="+")
        {
          result = a+b;
        } else if(root->getToken().getValue()=="-")
        {
          result = a-b;
        }
      }
      break;
    case MULTIPLYING:
      if(root->left()==NULL || root->right()==NULL)
      {
        cerr << "Error: Multiplying operator missing operands" << endl;
        return;
      } else{
        double a, b;
        interpret(root->left(), a);
        interpret(root->right(), b);
        if(root->getToken().getValue()=="*")
        {
          result = a*b;
        } else if(root->getToken().getValue()=="/")
        {
          result = a/b;
        } else if(root->getToken().getValue()=="%")
        {
          result = (int)a % (int)b;
        }
      }
      break;
    case INCLUSIVE_OR:
      if(root->left()==NULL || root->right()==NULL)
      {
        cerr << "Error: Inclusive or operator missing operands" << endl;
        return;
      } else{
        double a, b;
        interpret(root->left(), a);
        interpret(root->right(), b);
        if(root->getToken().getValue()=="|")
        {
          result = (int)a | (int)b;
        }
      }
      break;
    case EXCLUSIVE_OR:
      if(root->left()==NULL || root->right()==NULL)
      {
        cerr << "Error: Exclusive or operator missing operands" << endl;
        return;
      } else{
        double a, b;
        interpret(root->left(), a);
        interpret(root->right(), b);
        if(root->getToken().getValue()=="||")
        {
          result = (int)a || (int)b;
        }
      }
      break;
    case AND:
      if(root->left()==NULL || root->right()==NULL)
      {
        cerr << "Error: And operator missing operands" << endl;
        return;
      } else{
        double a, b;
        interpret(root->left(), a);
        interpret(root->right(), b);
        if(root->getToken().getValue()=="&&")
        {
          result = (int)a && (int)b;
        }
      }
      break;
    case ASSIGNMENT:
      if(root->left()==NULL || root->right()==NULL)
      {
        cerr << "Error: Assignment operator missing operands" << endl;
        return;
      } else{
        double a, b;
        interpret(root->right(), a);
        interpret(root->left(), b);
        if(root->getToken().getValue()=="=")
        {
          result = a;
        }
        else if(root->getToken().getValue()=="*=")
        {
          result = b * a;
        }
        else if(root->getToken().getValue()=="/=")
        {
          result = b / a;
        }
        else if(root->getToken().getValue()=="+=")
        {
          result = b + a;
        }
        else if(root->getToken().getValue()=="-=")
        {
          result = b - a;
        }
        else if(root->getToken().getValue()=="%=")
        {
          b = (int)b % (int)a;
          result = b;
        }
        else if(root->getToken().getValue()=="<<=")
        {
          b = (int)b << (int)a;
          result = b;
        }
        else if(root->getToken().getValue()==">>=")
        {
          b = (int)b >> (int)a;
          result = b;
        }
        else if(root->getToken().getValue()=="&=")
        {
          b = (int)b & (int)a;
          result = b;
        }
        else if(root->getToken().getValue()=="^=")
        {
          b = (int)b ^ (int)a;
          result = b;
        }
        else if(root->getToken().getValue()=="|=")
        {
          b = (int)b | (int)a;
          result = b;
        
        }
      }
      break;

    case INTEGER:
      result = stoi(root->getToken().getValue());
      break;
    case REAL:
      result = stod(root->getToken().getValue());
      break;
    

  }
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
    cout << "Before Parsing " << root << endl;
    bool b = expression(tokens, root);
    if (b) {
      cout << "Success your code parses" << endl;
      double result;
      string assembly;
      interpret(root, result);
      compileHelper(root, assembly);
      ofstream out(argv[2]);
      out << assembly;
      out.close();
      cout << "Result: " << result << endl;
    }
    else cout << "Failure your code does not parse" << endl;
    cout << root << endl;
  }
}