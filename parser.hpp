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


bool expression(Tokens &tokens);

bool primaryExpression(Tokens &tokens) {
    cout << "Primary Expression " << tokens.getLine() << endl;
    Token t = tokens.getNext();
    cout << t << endl;
    if(t.getToken() == OPENPAREN)
    {
        bool retval = expression(tokens);
        if(retval)
        {
            t = tokens.getNext();
            if(t.getToken() == CLOSEPAREN)
            {
              return true;
            }
            else
            {
              return error("Expected ')'");
            }
        }
        else{
            return error("Expected expression after '('");
        }
    }
    if (t.getToken() == VARIABLE)
    {
        cout << "Matched VARIABLE"<< endl;
        return true;
    }
    else if (t.getToken() == INTEGER || t.getToken() == REAL)
    {
        cout << "Matched INTEGER or REAL" << endl;
        return true;
    } else {
        return error("Expected identifier, constant, string, or expression in parenthesis");
    }
}


bool unaryExpression(Tokens &tokens) {
    cout << "Unary Expression " << tokens.getLine() << endl;
    Token t = tokens.peekNext();
    if (t.getToken() == UNARY) {
        tokens.getNext();
        return unaryExpression(tokens);
    } 
    else {
        return primaryExpression(tokens);
    }
}


bool multiplicativeExpression(Tokens &tokens) {
    cout << "Multiplicative Expression " << tokens.getLine() << endl;
    if (unaryExpression(tokens)) {
        Token t = tokens.peekNext();
        if (t.getToken() == MULTIPLYING) {
            tokens.getNext();
            return multiplicativeExpression(tokens);
        } else {
            return true;
        }
    } else {
        return error("Expected unary expression");
    }
}

bool additiveExpression(Tokens &tokens) {
    cout << "Additive Expression " << tokens.getLine() << endl;
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
    cout << "Shift Expression " << tokens.getLine() << endl;
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
    cout << "Relational Expression " << tokens.getLine() << endl;
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
    cout << "Equality Expression " << tokens.getLine() << endl;
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
    cout << "And Expression " << tokens.getLine() << endl;
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
    cout << "Exclusive Or Expression " << tokens.getLine() << endl;
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
    cout << "Inclusive Or Expression " << tokens.getLine() << endl;
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
      return true;
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
      return true;
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
    Token t = tokens.peekNext();
    cout << t << endl;
    if(t.getToken() == ASSIGNMENT)
    {
      tokens.getNext();
      return assignmentExpression(tokens);
    }
    else
    {
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

bool expression(Tokens &tokens){
  cout << "Expression " << tokens.getLine() << endl;
    Token t=tokens.peekNext();

  if(assignmentExpression(tokens))
  {
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


// <primary-expression> ::= <identifier>
//                        | <constant>
//                        | <string>
//                        | ( <expression> )
