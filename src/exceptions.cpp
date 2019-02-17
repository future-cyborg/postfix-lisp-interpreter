#include <string>
#include <fstream>
#include <iostream>
#include "exceptions.hpp"

using std::string;

Exception::Exception(const char* msg) : e_msg{msg} {}
Exception::~Exception( ) {}
const char* Exception::what( ) const throw () {return(e_msg);}

MissingParamException::MissingParamException(const char* msg)  : Exception{msg} {}
MissingParamException::~MissingParamException( ) {}

NotANumber::NotANumber(const char* msg)  : Exception{msg} {}
NotANumber::~NotANumber( ) {}

NotAString::NotAString(const char* msg)  : Exception{msg} {}
NotAString::~NotAString( ) {}

InvalidLambdaExpression::InvalidLambdaExpression(const char* msg)  : Exception{msg} {}
InvalidLambdaExpression::~InvalidLambdaExpression( ) {}

UnmatchingParenthesis::UnmatchingParenthesis(const char* msg)  : Exception{msg} {}
UnmatchingParenthesis::~UnmatchingParenthesis( ) {}

NotALambda::NotALambda(const char* msg)  : Exception{msg} {}
NotALambda::~NotALambda( ) {}

NotAnAtom::NotAnAtom(const char* msg)  : Exception{msg} {}
NotAnAtom::~NotAnAtom( ) {}

CannotEvaluate::CannotEvaluate(const char* msg) : Exception{msg} {}
CannotEvaluate::~CannotEvaluate( ) {}

CannotCall::CannotCall(const char* msg) : Exception{msg} {}
CannotCall::~CannotCall( ) {}