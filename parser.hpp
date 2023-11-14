#pragma once

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <iomanip>

using namespace std;

#include "error.hpp"
#include "token.hpp"
#include "tokens.hpp"
#include "AST.hpp"


bool expression(Tokens &tokens, ASTNode *tree);

bool primaryExpression(Tokens &tokens, ASTNode *tree) {
    Token t = tokens.peekNext();
    if(t.getToken() == OPENPAREN) {
        tokens.getNext();
        ASTNode *subtree = new ASTNode();
        if (expression(tokens, subtree)) {
            t = tokens.getNext();
            if(t.getToken() == CLOSEPAREN) {
                *tree = *subtree;
                cout << tree;
                return true;
            } else {
                return error("Expected ')'");
            }
        } else {
            return error("Expected expression after '('");
        }
    }
    if (t.getToken() == VARIABLE) {
        cout << "Matched VARIABLE"<< endl;
        tokens.getNext();
        *tree = ASTNode(t);
        return true;
    } else if (t.getToken() == INTEGER || t.getToken() == REAL) {
        cout << "Matched INTEGER or REAL" << endl;
        tokens.getNext();
        *tree = ASTNode(t);
        return true;
    } else {
        return error("Expected identifier, constant, string, or expression in parenthesis");
    }
}

bool unaryExpression(Tokens &tokens, ASTNode *tree) {
    cout << "Unary Expression " << tokens.getLine() << endl;
    Token t = tokens.peekNext();
    if (t.getToken() == UNARY) {
        tokens.getNext();
        ASTNode *subtree = new ASTNode();
        if (unaryExpression(tokens, subtree)) {
            *tree = ASTNode(t, subtree);
            return true;
        } else {
            return false;
        }
    } else {
        return primaryExpression(tokens, tree);
    }
}


bool multiplicativeExpression(Tokens &tokens, ASTNode *tree) {
    cout << "Multiplicative Expression " << tokens.getLine() << endl;
    ASTNode *lsubtree = new ASTNode();
    if (unaryExpression(tokens, lsubtree)) {
        Token t = tokens.peekNext();
        if (t.getToken() == MULTIPLYING) {
            tokens.getNext();
            ASTNode *rsubtree = new ASTNode();
            bool success = multiplicativeExpression(tokens, rsubtree);
            if(success) {
                cout << "Perform multiplication of ";
                cout << lsubtree;
                cout << " and ";
                cout << rsubtree;
                cout << endl;
                *tree = ASTNode(t, lsubtree, rsubtree);
                return true;
            } else {
                return false;
            }
        } else {
            *tree = *lsubtree;
            return true;
        }
    } else {
        return error("Expected unary expression");
    }
}


bool additiveExpression(Tokens &tokens, ASTNode *tree) {
    cout << "Additive Expression " << tokens.getLine() << endl;
    ASTNode *lsubtree = new ASTNode();
    if (multiplicativeExpression(tokens, lsubtree)) {
        Token t = tokens.peekNext();
        if (t.getToken() == ADDING) {
            tokens.getNext();
            ASTNode *rsubtree = new ASTNode();
            bool success = additiveExpression(tokens, rsubtree);
            if(success)
            {
              cout << "Perform addition of ";
              cout << lsubtree;
              cout << " and ";
              cout << rsubtree;
              cout << endl;
              *tree = ASTNode(t, lsubtree, rsubtree);
              return true;
            }
            else
            {
              return false;
            }
        } else {
            *tree = *lsubtree;
            return true;
        }
    } else {
        return error("Expected multiplicative expression");
    }
}

bool shiftExpression(Tokens &tokens, ASTNode *tree) {
    cout << "Shift Expression " << tokens.getLine() << endl;
    ASTNode *lsubtree = new ASTNode();
    if (additiveExpression(tokens, lsubtree)) {
        Token t = tokens.peekNext();
        if (t.getToken() == SHIFT) {
            tokens.getNext();
            ASTNode *rsubtree = new ASTNode();
            bool success = shiftExpression(tokens, rsubtree);
            if(success)
            {
              cout << "Perform shift of ";
              cout << lsubtree;
              cout << " and ";
              cout << rsubtree;
              cout << endl;
              *tree = ASTNode(t, lsubtree, rsubtree);
              return true;
            }
            else
            {
              return false;
            }
        }
        else
        {
          *tree = *lsubtree;
          return true;
        }
    } else {
        return error("Expected additive expression");
    }
}

bool relationalExpression(Tokens &tokens, ASTNode *tree) {
    cout << "Relational Expression " << tokens.getLine() << endl;
    ASTNode *lsubtree=new ASTNode();
    if (shiftExpression(tokens, lsubtree)) {
        Token t = tokens.peekNext();
        if (t.getToken() == RELATIONAL) {
            tokens.getNext();
            ASTNode *rsubtree=new ASTNode();
            bool success = relationalExpression(tokens, rsubtree);
            if(success)
            {
              cout << "Perform relational of ";
              cout << lsubtree;
              cout << " and ";
              cout << rsubtree;
              cout << endl;
              *tree = ASTNode(t, lsubtree, rsubtree);
              return true;
            }
            else
            {
              return false;
            }
        } else {
            *tree = *lsubtree;
            return true;
        }
    } else {
        return error("Expected shift expression");
    }
}

bool equalityExpression(Tokens &tokens, ASTNode *tree) {
    cout << "Equality Expression " << tokens.getLine() << endl;
    ASTNode *lsubtree=new ASTNode();
    if (relationalExpression(tokens, lsubtree)) {
        Token t = tokens.peekNext();
        if (t.getToken() == EQUALITY) {
            tokens.getNext();
            ASTNode *rsubtree=new ASTNode();
            bool success = equalityExpression(tokens, rsubtree);
            if(success)
            {
              cout << "Perform equality of ";
              cout << lsubtree;
              cout << " and ";
              cout << rsubtree;
              cout << endl;
              *tree = ASTNode(t, lsubtree, rsubtree);
              return true;
            }
            else
            {
              return false;
            }

        } else {
            *tree = *lsubtree;
            return true;
        }
    } else {
        return error("Expected relational expression");
    }
}

bool andExpression(Tokens &tokens, ASTNode *tree) {
    cout << "And Expression " << tokens.getLine() << endl;
    ASTNode *lsubtree=new ASTNode();
    if (equalityExpression(tokens, lsubtree)) {
        Token t = tokens.peekNext();
        if (t.getToken() == AND) {
            tokens.getNext();
            ASTNode *rsubtree=new ASTNode();
            bool success = andExpression(tokens, rsubtree);
            if(success)
            {
              cout << "Perform and of ";
              cout << lsubtree;
              cout << " and ";
              cout << rsubtree;
              cout << endl;
              *tree = ASTNode(t, lsubtree, rsubtree);
              return true;
            }
            else
            {
              return false;
            }
        } else {
            *tree = *lsubtree;
            return true;
        }
    } else {
        return error("Expected equality expression");
    }
}

bool exclusiveOrExpression(Tokens &tokens, ASTNode *tree) {
    cout << "Exclusive Or Expression " << tokens.getLine() << endl;
    ASTNode *lsubtree=new ASTNode();
    if (andExpression(tokens, lsubtree)) {
        Token t = tokens.peekNext();
        if (t.getToken() == EXCLUSIVE_OR) {
            tokens.getNext();
            ASTNode *rsubtree=new ASTNode();
            bool success = exclusiveOrExpression(tokens, rsubtree);
            if(success)
            {
              cout << "Perform exclusive or of ";
              cout << lsubtree;
              cout << " and ";
              cout << rsubtree;
              cout << endl;
              *tree = ASTNode(t, lsubtree, rsubtree);
              return true;
            }
            else
            {
              return false;
            }
        } else {
            *tree = *lsubtree;
            return true;
        }
    } else {
        return error("Expected and expression");
    }
}

bool inclusiveOrExpression(Tokens &tokens, ASTNode *tree) {
    cout << "Inclusive Or Expression " << tokens.getLine() << endl;
    ASTNode *lsubtree=new ASTNode();
    if (exclusiveOrExpression(tokens, lsubtree)) {
        Token t = tokens.peekNext();
        if (t.getToken() == INCLUSIVE_OR) {
            tokens.getNext();
            ASTNode *rsubtree=new ASTNode();
            bool sucess = inclusiveOrExpression(tokens, rsubtree);
            if(sucess)
            {
              cout << "Perform inclusive or of ";
              cout << lsubtree;
              cout << " and ";
              cout << rsubtree;
              cout << endl;
              *tree = ASTNode(t, lsubtree, rsubtree);
              return true;
            }
            else
            {
              return false;
            }
        } else {
            *tree = *lsubtree;
            return true;
        }
    } else {
        return error("Expected exclusive or expression");
    }
}

bool logicalAndExpression(Tokens &tokens,ASTNode *tree) {
  cout << "Logical And Expression " << tokens.getLine() << endl;
  ASTNode *lsubtree=new ASTNode();
  if(inclusiveOrExpression(tokens,lsubtree))
  {
    Token t = tokens.peekNext();
    if(t.getToken() == AND)
    {
      tokens.getNext();
      ASTNode *rsubtree=new ASTNode();
      bool success = logicalAndExpression(tokens, rsubtree);
      if(success)
      {
        cout << "Perform logical and of ";
        cout << lsubtree;
        cout << " and ";
        cout << rsubtree;
        cout << endl;
        *tree = ASTNode(t, lsubtree, rsubtree);
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      *tree = *lsubtree;
      return true;
    }
  }
  else
  {
    return error("Expected inclusive or expression");
  }
  
}

bool logicalOrExpression(Tokens &tokens, ASTNode *tree)
{
  cout << "Logical Or Expression " << tokens.getLine() << endl;
  ASTNode *lsubtree=new ASTNode();
  if(logicalAndExpression(tokens, lsubtree))
  {
    Token t = tokens.peekNext();
    if(t.getToken() == OR)
    {
      tokens.getNext();
      ASTNode *rsubtree=new ASTNode();
      bool success = logicalOrExpression(tokens, rsubtree);
      if(success)
      {
        cout << "Perform logical or of ";
        cout << lsubtree;
        cout << " and ";
        cout << rsubtree;
        cout << endl;
        *tree = ASTNode(t, lsubtree, rsubtree);
        return true;
      }
      else
      {
        return false;
      }

    }
    else
    {
      *tree = *lsubtree;
      return true;
    }
    
  }
  else
  {
    return error("Expected logical and expression");
  }

}

bool conditionalExpression(Tokens &tokens, ASTNode *tree){
  cout << "Conditional Expression " << tokens.getLine() << endl;
  ASTNode *lsubtree=new ASTNode();
  if(logicalOrExpression(tokens, lsubtree))
  {
    Token t = tokens.peekNext();
    if (t.getToken() == CONDITIONAL)
    {
      tokens.getNext();
      ASTNode *rsubtree=new ASTNode();
      if(expression(tokens, rsubtree))
      {
        t = tokens.peekNext();
        if(t.getToken() == CONDITIONAL)
        {
          tokens.getNext();
          bool success = conditionalExpression(tokens, rsubtree);
          if(success)
          {
            cout << "Perform conditional of ";
            cout << lsubtree;
            cout << " and ";
            cout << rsubtree;
            cout << endl;
            *tree = ASTNode(t, lsubtree, rsubtree);
            return true;
          }
          else
          {
            return false;
          }
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
      *tree = *lsubtree;
      return true;
    }
  }
  else
  {
    return error("Expected logical or expression");
  }
}

bool assignmentExpression(Tokens &tokens,ASTNode *tree){
  cout << "Assignment Expression " << tokens.getLine() << endl;
  ASTNode *lsubtree=new ASTNode();
  if(conditionalExpression(tokens, lsubtree))
  {
    Token t = tokens.peekNext();
    ASTNode *rsubtree=new ASTNode();
    if(t.getToken() == ASSIGNMENT)
    {
      tokens.getNext();
      bool success = assignmentExpression(tokens,rsubtree);
      if(success)
      {
        cout << "Perform assignment of ";
        cout << lsubtree;
        cout << " and ";
        cout << rsubtree;
        cout << endl;
        *tree = ASTNode(t, lsubtree, rsubtree);
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      *tree = *lsubtree;
      return true;
    }
  }
  else
  {
    return error("Expected conditional expression");
  }

  // if (unaryExpression(tokens))
  // {
  //   Token t = tokens.peekNext();
  //   if(t.getToken() == ASSIGNMENT)
  //   {
  //     tokens.getNext();
  //     return assignmentExpression(tokens);
  //   }
  //   else
  //   {
  //     return error("Expected assignment operator");
  //   }

  // }
  // else
  // {
  //   return error("Expected unary expression");
  // }

}

bool expression(Tokens &tokens, ASTNode *tree){
  cout << "Expression " << tokens.getLine() << endl;
  Token t=tokens.peekNext();
  ASTNode *lsubtree=new ASTNode();

  if(assignmentExpression(tokens, lsubtree))
  {
    if(t.getToken() == COMMA){
      tokens.getNext();
      ASTNode *rsubtree=new ASTNode();
      bool success = expression(tokens, rsubtree);
      if(success)
      {
        cout << lsubtree;
        cout << " and ";
        cout << rsubtree;
        cout << " are an expression" << endl;
        *tree = ASTNode(t, lsubtree, rsubtree);
        return true;
      }
      else
      {
        return false;
      }
    }
    else{
      *tree = *lsubtree;  
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
// <unary-expression> ::= <primary-expression>
//                      | ++ <unary-expression>
//                      | -- <unary-expression>
//                      | <unary-operator> <cast-expression>
//                      | sizeof <unary-expression>


// <primary-expression> ::= <identifier>
//                        | <constant>
//                        | <string>
//                        | ( <expression> )
