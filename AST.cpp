#include "AST.h"
#include <sstream>
#include <cmath>


using namespace std;

AST::Node::Node(Tokenizer::Token t): tok_{t}, left_{NULL}, right_{NULL}
{}

AST::AST(vector<Tokenizer::Token>& postfixExpr)
{
    stack<Node *> s;
    Node *t, *t1, *t2;

    for(int i = 0; i < postfixExpr.size(); i++)
    {
        if(postfixExpr[i].type_ != Tokenizer::TokType::OPERATOR){
            t = new Node(postfixExpr[i]);
            s.push(t);
        } else {
            t = new Node(postfixExpr[i]);
            t1 = s.top();
            s.pop();
            t2 = s.top();
            s.pop();

            t->right_ = t1;
            t->left_ = t2;

            s.push(t);
        }
    }
    root_ = s.top();
    s.pop();
    first = true;
    // print2D(root_);
    curr_ = root_;
}

void AST::deleteTree(Node* &node)
{
    if(node == NULL){
        return;
    }

    deleteTree(node->left_);
    deleteTree(node->right_);

    deleteNode(node);
}

AST::~AST()
{
    deleteTree(root_);
}

string AST::process(string left, string right, string parent)
{
    stringstream s1(left);
    stringstream s2(right);
    stringstream ss;
    string str = "";
    int i1, i2, i3 = 0;
    s1 >> i1;
    s2 >> i2;
    if(parent == "+"){
        i3 = i1 + i2;
    } else if(parent == "-"){
        i3 = i1 - i2;
    } else if(parent == "*"){
        i3 = i1 * i2;
    } else if(parent == "/"){
        i3 = i1/i2;
    } else if(parent == "^"){
        i3 = pow(i1, i2);
    }
    ss << i3;
    ss >> str;
    return str;
}

void AST::deleteNode(Node* &n)
{
    delete n->left_;
    delete n->right_;
    delete n;
    n = nullptr;
}

AST& AST::simplify(map<string, string>& m) 
{
    if(parent.empty() == true && first == false && (root_->left_ == nullptr || root_->right_ == nullptr)){
        // print2D(root_);
        return *this;
    } else if(curr_->left_->tok_.type_ == Tokenizer::TokType::OPERATOR){
        parent.push(curr_);
        curr_ = curr_->left_;
        return simplify(m);
    } else if(curr_->right_->tok_.type_ == Tokenizer::TokType::OPERATOR){
        parent.push(curr_);
        curr_ = curr_->right_;
        return simplify(m);
    } else {
        if(((curr_->left_->tok_.type_ == Tokenizer::TokType::VARIABLE && m.count(curr_->left_->tok_.value_) == 0) && 
            (curr_->right_->tok_.type_ == Tokenizer::TokType::VARIABLE && m.count(curr_->right_->tok_.value_) != 0)) || 
            ((curr_->left_->tok_.type_ == Tokenizer::TokType::VARIABLE && m.count(curr_->left_->tok_.value_) != 0) && 
            (curr_->right_->tok_.type_ == Tokenizer::TokType::VARIABLE && m.count(curr_->right_->tok_.value_) == 0)))
        {
            if(m.count(curr_->left_->tok_.value_) > 0){
                curr_->left_->tok_.value_ = m[curr_->left_->tok_.value_];
                curr_->left_->tok_.type_ = Tokenizer::TokType::NUMBER;
            } else if(m.count(curr_->right_->tok_.value_) > 0){
                curr_->right_->tok_.value_ = m[curr_->left_->tok_.value_];
                curr_->right_->tok_.type_ = Tokenizer::TokType::NUMBER;
            }
            curr_->tok_.value_ = "(" + curr_->left_->tok_.value_ + curr_->tok_.value_ + curr_->right_->tok_.value_ + ")";
            curr_->tok_.type_ = Tokenizer::TokType::VARIABLE;
            deleteNode(curr_->left_);
            deleteNode(curr_->right_);
        } else {
            if(curr_->left_->tok_.type_ == Tokenizer::TokType::VARIABLE && curr_->right_->tok_.type_ != Tokenizer::TokType::VARIABLE){
                stringstream ss;
                string valueStored = "";
                ss << m[curr_->left_->tok_.value_];
                ss >> valueStored;
                curr_->left_->tok_.value_ = valueStored;
                curr_->left_->tok_.type_ = Tokenizer::TokType::NUMBER;
            } else if (curr_->right_->tok_.type_ == Tokenizer::TokType::VARIABLE && curr_->left_->tok_.type_ != Tokenizer::TokType::VARIABLE){
                stringstream ss;
                string valueStored = "";
                ss << m[curr_->right_->tok_.value_];
                ss >> valueStored;
                curr_->right_->tok_.value_ = valueStored;
                curr_->right_->tok_.type_ = Tokenizer::TokType::NUMBER;
            } else if(curr_->left_->tok_.type_ == Tokenizer::TokType::VARIABLE && curr_->right_->tok_.type_ == Tokenizer::TokType::VARIABLE){
                stringstream ss1;
                stringstream ss2;
                string valueStored1 = "";
                string valueStored2 = "";
                ss1 << m[curr_->right_->tok_.value_];
                ss1 >> valueStored1;
                ss2 << m[curr_->left_->tok_.value_];
                ss2 >> valueStored2;
                curr_->right_->tok_.value_ = valueStored1;
                curr_->right_->tok_.type_ = Tokenizer::TokType::NUMBER;
                curr_->left_->tok_.value_ = valueStored2;
                curr_->left_->tok_.type_ = Tokenizer::TokType::NUMBER;
            }
            string temp = process(curr_->left_->tok_.value_, curr_->right_->tok_.value_, curr_->tok_.value_);
            curr_->tok_.value_ = temp;
            curr_->tok_.type_ = Tokenizer::TokType::NUMBER;
            deleteNode(curr_->left_);
            deleteNode(curr_->right_);
        }
        if(curr_ != root_){
            curr_ = parent.top();
            parent.pop();
        }
        first = false;
        return simplify(m);
    }
    return *this;
}

string AST::toInfix(Node* cur)
{
    
    if(cur == nullptr){
       return str;
    } else {
        toInfix(cur->left_);
        str += cur->tok_.value_;
        toInfix(cur->right_);
    }
    return str;
}