#pragma once

/*
 * Tokenizer.h
 * Omar Ahmed
 * CSS 342
 * 6/6/2021 
 * 
 * 
 */

#include <string>
#include <iostream>
using namespace std;

class Tokenizer
{
private:
    
public:
    enum class TokType {OPERATOR, VARIABLE, NUMBER, LPAREN, RPAREN, END};

    struct Token
    {
        Token(TokType t = TokType::END, string v = "");
        TokType type_;
        string value_;
    };

    class ITokStream
    {
    public:
        ITokStream(istream& inputStream);
        ITokStream& operator >>(Token& rhs);
        explicit operator bool() const;
    private:
        bool isOperator(string s);
        istream& is_;
    };
};