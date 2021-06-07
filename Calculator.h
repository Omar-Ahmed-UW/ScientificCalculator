#pragma once

// #include <bits/stdc++.h>
#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
using namespace std;

class Calculator
{
private:
    enum class TokType {OPERATOR, VARIABLE, NUMBER, LPAREN, RPAREN, END};
    string input_;
    map<string, string> variableMap;
public:
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
        bool isOperator(string s);
    private:
        
        istream& is_;
    };
    
    class AST
    {
    private:
        struct Node
        {
            Node(Token t);
            Token tok_;
            Node* left_;
            Node* right_;
        };
        stack<Node*> parent;
        bool first;
        stack<Node*> copier;
        void deleteNode(Node* &n);
    public:    
        Node* curr_;
        void copy(AST& tree);
        AST(AST& tree);
        AST(vector<Token>& postfixExpr);
        ~AST();
        string process(string left, string right, string parent);
        AST& simplify(map<string, string>& m);
        AST& operator =(const AST& tree);
        Node* root_;
        string str = "";
        Node* cur = root_;
        string toInfix(Node* cur);

        

        void print2DUtil(AST::Node* root, int space);
        void print2D(Node *root);
    };

    int isAssignment(string& s);
    bool variablized;
    string output_;
    vector<Token> tokenizer(std::istream& is, string s);

    int preced(string s);
    vector<Token> postFixConverter(vector<Token>& v);

    friend std::istream& operator>>(std::istream& is, Calculator& calc);

    friend std::ostream& operator<<(std::ostream& os, Calculator& calc);

};