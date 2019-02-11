#include <string>
#include <fstream>
#include <iostream>

#ifndef EXCEPTION_H
#define EXCEPTION_H

using std::string;

class Exception : public std::exception {
public:
   Exception(const char* msg = "Error") : e_msg{msg} {}
  ~Exception( ) {}

   virtual const char* what( ) const throw () {return(e_msg);}
protected:
   const char* e_msg;
};

class MissingParamException : public Exception {
public:
   MissingParamException(const char* msg = "MissingParamException")  : Exception{msg} {}
  ~MissingParamException( ) {}
};

class NotANumber : public Exception {
public:
   NotANumber(const char* msg = "NotANumber")  : Exception{msg} {}
  ~NotANumber( ) {}
};

class NotAString : public Exception {
public:
   NotAString(const char* msg = "NotAString")  : Exception{msg} {}
  ~NotAString( ) {}
};

class InvalidLambdaExpression : public Exception {
public:
   InvalidLambdaExpression(const char* msg = "InvalidLambdaExpression")  : Exception{msg} {}
  ~InvalidLambdaExpression( ) {}
};

class UnmatchingParenthesis : public Exception {
public:
   UnmatchingParenthesis(const char* msg = "UnmatchingParenthesis")  : Exception{msg} {}
  ~UnmatchingParenthesis( ) {}
};

class NotALambda : public Exception {
public:
   NotALambda(const char* msg = "NotALambda")  : Exception{msg} {}
  ~NotALambda( ) {}
};

class NotAnAtom : public Exception {
public:
   NotAnAtom(const char* msg = "NotAnAtom")  : Exception{msg} {}
  ~NotAnAtom( ) {}
};

#endif