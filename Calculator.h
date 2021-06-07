#pragma once

#include "Tokenizer.h"
#include "AST.h"
#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
using namespace std;

/*
 * Calculator.h
 * Omar Ahmed
 * CSS 342
 * 6/6/2021 
 * 
 */

class Calculator
{
private:
    bool variablized;
    map<string, string> variableMap;
    int isAssignment(string& s);
    int preced(string s);
public:
    Calculator();
    bool flag;
    string input_;
    string output_;
    vector<Tokenizer::Token> tokenizer(std::istream& is, string s);
    
    vector<Tokenizer::Token> postFixConverter(vector<Tokenizer::Token>& v);

    friend std::istream& operator>>(std::istream& is, Calculator& calc);

    friend std::ostream& operator<<(std::ostream& os, Calculator& calc);

};