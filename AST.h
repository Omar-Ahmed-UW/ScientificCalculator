#pragma once

/*
 * AST.h
 * Omar Ahmed
 * CSS 342
 * 6/6/2021 
 * 
 */

#include "Tokenizer.h"
#include <vector>
#include <stack>
#include <map>

class AST
    {
private:
    struct Node
    {
        Node(Tokenizer::Token t);
        Tokenizer::Token tok_;
        Node* left_;
        Node* right_;
    };
    
    stack<Node*> parent;
    stack<Node*> copier;
    bool first;
    Node* curr_;
    string str;
    Node* cur;
    void deleteNode(Node* &n);
    void deleteTree(Node* &node);
public:    
    Node* root_;
    AST(vector<Tokenizer::Token>& postfixExpr);
    ~AST();
    string process(string left, string right, string parent);
    AST& simplify(map<string, string>& m);
    string toInfix(Node* cur);

    

    void print2DUtil(AST::Node* root, int space);
    void print2D(Node *root);
};