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

// int main()
// {
    
//     // Calculator::ITokStream str(cin);
//     // Calculator::Token t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13;
//     // t1.value_ = "5";
//     // t2.value_ = "x";
//     // t3.value_ = "4";
//     // t4.value_ = "*";
//     // t5.value_ = "6";
//     // t6.value_ = "2";
//     // t7.value_ = "/";
//     // t8.value_ = "+";
//     // t9.value_ = "8";
//     // t10.value_ = "2";
//     // t11.value_ = "+";
//     // t12.value_ = "-";
//     // t13.value_ = "*";
//     // str >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 >> t11 >> t12 >> t13;
//     // vector<Calculator::Token> v{t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13};
//     // for(int i = 0; i < v.size(); i++){
//     //     cout << v[i].value_ << endl;
//     // }
//     // Calculator::AST tree(v);
//     // map<string, string> mapp;
//     // // cout << tree.toInfix(tree.root_) << endl;
//     // tree.simplify(mapp);

//     // cout << tree.toInfix(tree.root_) << endl;

//     Calculator c;
    
//     for(int i = 0; i < 11111; i++){
//         cin >> c;
//         cout << c << endl << endl;
//     }
//     return 0;
// }