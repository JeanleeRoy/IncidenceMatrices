#ifndef LAB6A_PARSING_H
#define LAB6A_PARSING_H

#include "IncidenceMatrix.h"
#include <utility>
#include <vector>
#include <stack>
#include <map>

struct Token {
    string value;
    bool isOptr{};
    Token* left{};
    Token* right{};

    Token(string tkn) : value(std::move(tkn)) {}
    Token(char tkn) : isOptr(true) {value = tkn;}
};

class ResolveExp {
    Token* root{};
    vector<Token*> tokens;
    stack<char> stack_optr;
    stack<Token*> stack_tree;
    map<char,int> optr = {{'A',1},{'O',1},{'N',1},{'(',0},{')',0}};

public:
    explicit ResolveExp(string expresion) {
        expresion += " ";
        string tkn;
        for (auto e : expresion) {
            if (optr.count(e) && tkn.empty()) {
                evalOper(e);
                tkn.clear();
            } else if (e == ' ') {
                if (tkn.size() > 1) {
                    auto* temp = new Token(tkn);
                    tokens.push_back(temp);
                    //cout << "word\n";
                }
                tkn.clear();
            } else
                tkn += e;
        }
        while (!stack_optr.empty()) {
            auto temp = new Token(stack_optr.top());
            tokens.push_back(temp);
            stack_optr.pop();
        }
        cout << "Antes del arbol\n";
        buildTree();
        printn();
        cout << endl;
        showTree();
    }

    void evalOper(char oper) {
        //cout << oper << endl;
        if (oper == '(') {
            stack_optr.push(oper);
        } else if (oper == ')') {
            while (!stack_optr.empty() && stack_optr.top() != '(') {
                auto* temp = new Token(stack_optr.top());
                tokens.push_back(temp);
                stack_optr.pop();
            }
            stack_optr.pop();
        } else {
            while (!stack_optr.empty() && optr.at(stack_optr.top()) >= optr.at(oper) && stack_optr.top() != '(') {
                auto* temp = new Token(stack_optr.top());
                tokens.push_back(temp);
                stack_optr.pop();
            }
            stack_optr.push(oper);
        }
    }

    void buildTree() {
        for (auto tkn : tokens) {
            if (tkn->isOptr) {
                tkn->left = stack_tree.top();
                stack_tree.pop();
                if (tkn->value != "O") {
                    tkn->right = stack_tree.top();
                    stack_tree.pop();
                }
            }
            stack_tree.push(tkn);
        }
        root = stack_tree.top();
        stack_tree.pop();
    }

    void printn() {
        cout << "Postfix : \n\t";
        for (auto x : tokens) {
            cout << x->value << " ";
        }
        cout << endl;
    }

    void printTree(Token* _root, int space) {
        if (!_root) return;
        space += 4;
        printTree(_root->right, space);
        for (int i = 4; i < space; i++)
            printf(" ");
        printf("%s\n", _root->value.c_str());
        printTree(_root->left, space);
    }

    void showTree() {
        printTree(root, 0);
    }

};

#endif //LAB6A_PARSING_H
