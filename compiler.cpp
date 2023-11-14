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
      interpret(root, result);
      cout << "Result: " << result << endl;
    }
    else cout << "Failure your code does not parse" << endl;
    cout << root << endl;
  }
}