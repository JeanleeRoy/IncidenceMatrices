#ifndef LAB6A_INCIDENCEMATRIX_H
#define LAB6A_INCIDENCEMATRIX_H

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <cstdlib>

#define TOTAL_BOOKS 6

using namespace std;

class IncidenceMatrix {
    set<string> stop_words;
    set<string> books[TOTAL_BOOKS];

    void loadStopWords() {
        fstream file;
        string stop_word;
        file.open("../stopwords-es.txt", ios::in);
        while (file >> stop_word)
            stop_words.insert(stop_word);
        file.close();
    }

    void cleanWord(string& word) {
        set<char> signs = {',', ';', '.', '-', '_', ':', '?','"'};
        for (int i = 0; i < word.size(); ++i)
            if (signs.find(word[i]) != signs.end())
                word.erase(word.begin()+i);
    }

    void getWords(string line, int book_num) {
        if (line.empty()) return;
        string word;
        line += " ";
        int i = 0;
        for(auto c : line) {
            if (c == ' ') {
                i++;
                cleanWord(word);
                if (stop_words.find(word) == stop_words.end())
                    books[book_num].insert(word);
                word.clear();
            } else
                word += c;
        }
    }

    void loadBooks() {
        fstream book;
        string line;
        for (int b_num = 0; b_num < TOTAL_BOOKS; ++b_num) {
            string book_name = "../docs/libro" + to_string(b_num + 1) + ".txt";
            book.open(book_name, ios::in);
            while(getline(book,line)) {
                getWords(line, b_num);
            }
            book.close();
        }
    }

public:
    IncidenceMatrix() {
        loadStopWords();
        loadBooks();
    }

    string find(const string& word) {
        string incidences;
        for (auto & book : books)
            if (book.find(word) != book.end())
                incidences += '1';
            else incidences += '0';
        return incidences;
    }

    basic_string<char> operator ()(const string& word) {
        return find(word);
    }
};

#endif //LAB6A_INCIDENCEMATRIX_H
