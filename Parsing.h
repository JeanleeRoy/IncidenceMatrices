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

    Token() : value("~") {}
    Token(string tkn) : value(std::move(tkn)) {}
    Token(char tkn) : isOptr(true) {value = tkn;}
};

class ResolveExp {
    string expression;
    IncidenceMatrix M;
    Token* root{};
    vector<string> words;
    vector<Token*> tokens;
    stack<char> stack_optr;
    stack<Token*> stack_tree;
    map<char,int> optr = {{'A',1},{'O',1},{'N',2},{'(',0},{')',0}};

    void loadTokens() {
        string tkn, exp = expression + " ";
        for (int e = 0; e < exp.size(); ++e) {
            if (optr.count(exp[e]) && tkn.empty() &&
                exp[e+1] == ' ') {
                evalOper(exp[e]);
                tkn.clear();
            } else if (exp[e] == ' ') {
                if (tkn.size() > 1) {
                    auto *temp = new Token(tkn);
                    tokens.push_back(temp);
                    words.push_back(tkn);
                }
                tkn.clear();
            } else
                tkn += exp[e];
        }
        while (!stack_optr.empty()) {
            auto* temp = new Token(stack_optr.top());
            tokens.push_back(temp);
            stack_optr.pop();
        }
    }

    void evalOper(char oper) {
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
                tkn->right = stack_tree.top();
                stack_tree.pop();
                if (tkn->value != "N") {
                    tkn->left = stack_tree.top();
                    stack_tree.pop();
                } else {
                    auto* temp = new Token();
                    tkn->left = temp;
                }
            }
            stack_tree.push(tkn);
        }
        root = stack_tree.top();
        stack_tree.pop();
    }

    void solution() {
        cout << "Expresion : \n\t" << expression << endl;
        cout << "\nMatriz de incidencia : \n\t\t";
        for (int i = 0; i < TOTAL_BOOKS; ++i)
            cout << "libro" << i+1 << "\t";
        cout << endl;
        for (const auto& w : words) {
            cout << w << "\t\t";
            string rst = M(w);
            for (auto r : rst)
                cout << r << "\t";
            cout << endl;
        }
        cout << "\nArbol de la expresion : \n";
        showTree();
        cout << "\nResultado : \nLos libros son\n";
        string result = execute(root);
        for (int j = 0; j < result.size(); ++j) {
            if (result[j] == '1')
                cout << "\tLibro " << j+1 << endl;
        }
    }

    string execute(Token* tkn) {
        if (!tkn->isOptr) {
            if (tkn->value == "~") return "";
            else return M(tkn->value);
        }
        string left = execute(tkn->left);
        string right = execute(tkn->right);
        return operation(left,right,tkn->value[0]);
    }

    string operation(const string& x, const string& y, char oper){
        string result;
        switch (oper) {
            case 'A':
                for (int i = 0; i < TOTAL_BOOKS; ++i) {
                    if (x[i]=='1' && y[i]=='1') result+='1';
                    else result+='0';
                } break;
            case 'O':
                for (int i = 0; i < TOTAL_BOOKS; ++i) {
                    if (x[i]=='0' && y[i]=='0') result+='0';
                    else result+='1';
                } break;
            case 'N':
                for (int i = 0; i < TOTAL_BOOKS; ++i) {
                    if (y[i]=='0') result+='1';
                    else result+='0';
                } break;
            default: break;
        }
        return result;
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

public:
    explicit ResolveExp(string exp) : expression(std::move(exp)) {
        loadTokens();
        buildTree();
        solution();
    }

};

#endif //LAB6A_PARSING_H
