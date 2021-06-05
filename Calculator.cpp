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

Calculator::Token::Token(TokType t, string v): type_{t}, value_{v}
{}

Calculator::ITokStream::ITokStream(istream& inputStream): is_{inputStream}
{}

bool Calculator::ITokStream::isOperator(string s)
{
    if(s == "+" || s == "-" || s == "*" || s == "/" || s == "^" || s == ":="){
        return true;
    }

    return false;
}

Calculator::ITokStream& Calculator::ITokStream::operator >>(Token& rhs)
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

Calculator::ITokStream::operator bool() const
{
    if(is_){
        return true;
    } else {
        cerr << "Cin error! \n";
    }
    return false;
}

Calculator::AST::Node::Node(Token t): tok_{t}, left_{NULL}, right_{NULL}
{}

// void Calculator::AST::copy(AST& tree)
// {
//     if()
// }

Calculator::AST::AST(AST& tree)
{
    tree.root_ = root_;
}

Calculator::AST::AST(vector<Token>& postfixExpr)
{
    stack<Node *> s;
    Node *t, *t1, *t2;

    for(int i = 0; i < postfixExpr.size(); i++)
    {
        if(postfixExpr[i].type_ != TokType::OPERATOR){
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

Calculator::AST::~AST()
{
    deleteNode(root_);
}

string Calculator::AST::process(string left, string right, string parent)
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

void Calculator::AST::deleteNode(Node* &n)
{
    delete n->left_;
    delete n->right_;
    delete n;
    n = nullptr;
}

Calculator::AST& Calculator::AST::simplify(map<string, int>& m) 
{
    if(parent.empty() == true && first == false && (root_->left_ == nullptr || root_->right_ == nullptr)){
        // print2D(root_);
        return *this;
    } else if(curr_->left_->tok_.type_ == TokType::OPERATOR){
        parent.push(curr_);
        curr_ = curr_->left_;
        return simplify(m);
    } else if(curr_->right_->tok_.type_ == TokType::OPERATOR){
        parent.push(curr_);
        curr_ = curr_->right_;
        return simplify(m);
    } else {
        if((curr_->left_->tok_.type_ == TokType::VARIABLE && m.count(curr_->left_->tok_.value_) == 0) || 
            (curr_->right_->tok_.type_ == TokType::VARIABLE && m.count(curr_->right_->tok_.value_) == 0))
        {
            curr_->tok_.value_ = "(" + curr_->left_->tok_.value_ + curr_->tok_.value_ + curr_->right_->tok_.value_ + ")";
            curr_->tok_.type_ = TokType::VARIABLE;
            deleteNode(curr_->left_);
            deleteNode(curr_->right_);
        } else {
            if(curr_->left_->tok_.type_ == TokType::VARIABLE){
                stringstream ss;
                string valueStored = "";
                ss << m[curr_->left_->tok_.value_];
                ss >> valueStored;
                curr_->left_->tok_.value_ = valueStored;
                curr_->left_->tok_.type_ = TokType::NUMBER;
            } else if (curr_->right_->tok_.type_ == TokType::VARIABLE){
                stringstream ss;
                string valueStored = "";
                ss << m[curr_->right_->tok_.value_];
                ss >> valueStored;
                curr_->right_->tok_.value_ = valueStored;
                curr_->right_->tok_.type_ = TokType::NUMBER;
            }
            string temp = process(curr_->left_->tok_.value_, curr_->right_->tok_.value_, curr_->tok_.value_);
            curr_->tok_.value_ = temp;
            curr_->tok_.type_ = TokType::NUMBER;
            deleteNode(curr_->left_);
            deleteNode(curr_->right_);
        }
        if(curr_ != root_){
            curr_ = parent.top();
            parent.pop();
            first = false;
        }
        return simplify(m);
    }
    return *this;
}

// Calculator::AST& Calculator::AST::operator =(const AST& tree)
// {

// }

string Calculator::AST::toInfix(Node* cur)
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


void Calculator::AST::print2DUtil(Node *root, int space) 
{ 
    // Base case 
    if (root == NULL) 
        return; 
  
    // Increase distance between levels 
    space += COUNT; 
  
    // Process right child first 
    print2DUtil(root->right_, space); 
  
    // Print current node after space 
    // count 
    cout<<endl; 
    for (int i = COUNT; i < space; i++) 
        cout<<" "; 
    cout<< root->tok_.value_ <<"\n"; 
  
    // Process left child 
    print2DUtil(root->left_, space); 
} 
  
// Wrapper over print2DUtil() 
void Calculator::AST::print2D(Node *root) 
{ 
    // Pass initial space count as 0 
    print2DUtil(root, 0); 
} 

int main()
{
    
    Calculator::ITokStream str(cin);
    Calculator::Token t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13;
    t1.value_ = "5";
    t2.value_ = "x";
    t3.value_ = "4";
    t4.value_ = "*";
    t5.value_ = "6";
    t6.value_ = "2";
    t7.value_ = "/";
    t8.value_ = "+";
    t9.value_ = "8";
    t10.value_ = "2";
    t11.value_ = "+";
    t12.value_ = "-";
    t13.value_ = "*";
    str >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 >> t11 >> t12 >> t13;
    vector<Calculator::Token> v{t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13};
    for(int i = 0; i < v.size(); i++){
        cout << v[i].value_ << endl;
    }
    Calculator::AST tree(v);
    map<string, int> mapp;
    // cout << tree.toInfix(tree.root_) << endl;
    tree.simplify(mapp);

    cout << tree.toInfix(tree.root_) << endl;
    return 0;
}