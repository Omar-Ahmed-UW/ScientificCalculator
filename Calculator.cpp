#include "Calculator.h"
#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
#include <cmath>
#include <map>
#define COUNT 10
using namespace std;

/*
 * Calculator.cpp
 * Omar Ahmed
 * CSS 342
 * 6/6/2021 
 * 
 */



Calculator::Calculator(): flag{false}, input_{""}, output_{""}
{}


int Calculator::isAssignment(string& str)
{
    for(int i = 0; i < str.length(); i++){
        if(str[i] == ':'){
            return i;
        }
    }

    return -1;
}


int Calculator::preced(string s)
{
    if(s == "+" || s == "-"){
        return 1;
    } else if(s == "*" || s == "/"){
        return 2;
    } else if(s == "^"){
        return 3;
    } else {
        return 0;
    }
}

vector<Tokenizer::Token> Calculator::postFixConverter(vector<Tokenizer::Token>& v)
{
    stack<Tokenizer::Token> stk;
    Tokenizer::Token tempTok;
    tempTok.value_ = "#";
    stk.push(tempTok);
    vector<Tokenizer::Token> returnVec;

    for(int i = 0; i < v.size(); i++){
        if(isalnum(v[i].value_[0])){
            returnVec.push_back(v[i]);
        } else if(v[i].type_ == Tokenizer::TokType::LPAREN){
            stk.push(v[i]);
        } else if(v[i].value_ == "^"){
            stk.push(v[i]);
        } else if(v[i].type_ == Tokenizer::TokType::RPAREN){
            while(stk.top().value_ != "#" && stk.top().type_ != Tokenizer::TokType::LPAREN){
                returnVec.push_back(stk.top());
                stk.pop();
            }
            stk.pop();
        } else {
            if(preced(v[i].value_) > preced(stk.top().value_)){
                stk.push(v[i]);
            } else {
                while(stk.top().value_ != "#" && preced(v[i].value_) <= preced(stk.top().value_)){
                    returnVec.push_back(stk.top());
                    stk.pop();
                }
                stk.push(v[i]);
            }
        }
    }

    while(stk.top().value_ != "#"){
        returnVec.push_back(stk.top());
        stk.pop();
    }

    return returnVec;
}

vector<Tokenizer::Token> Calculator::tokenizer(std::istream& is, string s)
{
    variablized = false;
    int rtrn = isAssignment(s);
    if(rtrn != -1){
        if(isalpha(s[0])){
            string str = "";
            variablized = true;
            string tempStr(1, tolower(s[0]));
            variableMap[tempStr] = "";
            for(int i = rtrn + 2; i < s.length(); i++){
                str += s[i];
            }
            s = str;
        } else {
            cerr << "Invalid variable entry! \n";
        }
    }
    
    Tokenizer::ITokStream itStream(is);
    vector<Tokenizer::Token> tokenVector;
    for(int i = 0; i < s.length(); i++){
        Tokenizer::Token t;
        int j = i;
        if(isdigit(s[i])){
            while(isdigit(s[j]) && j < s.length()){
                t.value_ += s[j];
                j++;
            }
            i = j - 1;
        } else {
            t.value_ = s[i];
        }
        itStream >> t;
        tokenVector.push_back(t);
    }
    
    return tokenVector;
}


std::istream& operator>>(std::istream& is, Calculator& calc)
{
    calc.flag = false;
    is >> calc.input_;
    if(calc.input_ == "."){
        calc.flag = true;
    } else {
        vector<Tokenizer::Token> tempV = calc.tokenizer(is, calc.input_);


        string str(1, calc.input_[0]);
        if(tempV.size() == 1 && calc.variableMap.count(str) > 0 && tempV[0].type_ == Tokenizer::TokType::VARIABLE){
            tempV = calc.tokenizer(is, calc.variableMap[tempV[0].value_]);
        }

        if(tempV.size() > 1){
            vector<Tokenizer::Token> v = calc.postFixConverter(tempV);
            AST tree(v);
            tree.simplify(calc.variableMap);
            calc.output_ = tree.toInfix(tree.root_);
            if(calc.variablized == true || (tempV.size() == 1 && tempV[0].type_ == Tokenizer::TokType::VARIABLE)){
                string str(1, calc.input_[0]);
                calc.variableMap[str] = calc.output_;
            }
        } else {        
            if(calc.variableMap.count(str) > 0){
                calc.output_ = calc.variableMap[str] = tempV[0].value_;
            } else {
                cerr << "Invalid variable! \n";
            }
        }
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, Calculator& calc)
{
    os << calc.output_;
    
    return os;
}

