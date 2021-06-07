#include "Tokenizer.h"

/*
 * Tokenizer.cpp
 * Omar Ahmed
 * CSS 342
 * 6/6/2021 
 * 
 */

using namespace std;

Tokenizer::Token::Token(TokType t, string v): type_{t}, value_{v}
{}

Tokenizer::ITokStream::ITokStream(istream& inputStream): is_{inputStream}
{}

bool Tokenizer::ITokStream::isOperator(string s)
{
    if(s == "+" || s == "-" || s == "*" || s == "/" || s == "^"){
        return true;
    }

    return false;
}

Tokenizer::ITokStream& Tokenizer::ITokStream::operator >>(Token& rhs)
{
    if(isdigit(rhs.value_[0])){
        rhs.type_ = TokType::NUMBER;
    } else if(isOperator(rhs.value_) == true){
        rhs.type_ = TokType::OPERATOR;
    } else if(rhs.value_ == "("){
        rhs.type_ = TokType::LPAREN;
    } else if(rhs.value_ == ")"){
        rhs.type_ = TokType::RPAREN;
    } else if(rhs.value_ == "."){
        rhs.type_ = TokType::END;
    } else if(isalpha(rhs.value_[0])) {
        rhs.value_ = tolower(rhs.value_[0]);
        rhs.type_ = TokType::VARIABLE;
    } else {
        cerr << "Character error! \n";
    }
    return *this;
}

Tokenizer::ITokStream::operator bool() const
{
    if(is_){
        return true;
    } else {
        cerr << "Cin error! \n";
    }
    return false;
}

