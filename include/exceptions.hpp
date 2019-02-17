#include <string>
#include <fstream>
#include <iostream>

#ifndef EXCEPTION_H
#define EXCEPTION_H

using std::string;

class Exception : public std::exception {
public:
   Exception(const char* msg = "Error");
  ~Exception( );

   virtual const char* what( ) const throw();
protected:
   const char* e_msg;
};

class MissingParamException : public Exception {
public:
   MissingParamException(const char* msg = "MissingParamException");
  ~MissingParamException( );
};

class NotANumber : public Exception {
public:
   NotANumber(const char* msg = "NotANumber");
  ~NotANumber( );
};

class NotAString : public Exception {
public:
   NotAString(const char* msg = "NotAString");
  ~NotAString( );
};

class InvalidLambdaExpression : public Exception {
public:
   InvalidLambdaExpression(const char* msg = "InvalidLambdaExpression");
  ~InvalidLambdaExpression( );
};

class UnmatchingParenthesis : public Exception {
public:
   UnmatchingParenthesis(const char* msg = "UnmatchingParenthesis");
  ~UnmatchingParenthesis( );
};

class NotALambda : public Exception {
public:
   NotALambda(const char* msg = "NotALambda");
  ~NotALambda( );
};

class NotAnAtom : public Exception {
public:
   NotAnAtom(const char* msg = "NotAnAtom");
  ~NotAnAtom( );
};

class CannotEvaluate : public Exception {
public:
   CannotEvaluate(const char* msg = "CannotEvaluate");
  ~CannotEvaluate( );
};

class CannotCall : public Exception {
public:
   CannotCall(const char* msg = "CannotCall");
  ~CannotCall( );
};

#endif