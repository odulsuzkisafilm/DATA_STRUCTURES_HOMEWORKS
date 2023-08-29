//
// Created by Efe Cinar on 4.08.2023.
//

#include "Dictionary.h"
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// overloaded operator functions for wordObject classes that take the words(keys) into consideration when comparing
bool operator==(const wordObject & lhs, const wordObject & rhs){return lhs.getWord() == rhs.getWord();}
bool operator>(const wordObject & lhs, const wordObject & rhs){return lhs.getWord() > rhs.getWord();}
bool operator<(const wordObject & lhs, const wordObject & rhs){return lhs.getWord() < rhs.getWord();}
wordObject NULL_ITEM("-1", "-1"); // ITEM_NOT_FOUND object for the type wordObject


string & to_lower(string & word){ // to make the program case-insensitive, all the words are made lowercase for the dict
    for (int i=0; i < word.size(); i++){
        if (word[i] >= 'A' && word[i] <= 'Z'){
            word[i] -= ('Z' - 'z');
        }
    }
    return word;
}

Dictionary::Dictionary(): DictionaryTree(NULL_ITEM) {
    ifstream ifs("dict.txt");
    string line, word, meaning, str;

    while (getline(ifs, line)) { // for every line, first word is the word for dict, others are the meaning
        if(!line.empty()) { // skipping blank lines
            istringstream ss(line);
            ss >> word;

            bool firstWord=true;
            meaning = "";

            while (ss >> str){
                if(firstWord) {
                    meaning += str;
                    firstWord = false;
                }else{
                    meaning += " "; meaning += str;
                }
            }
            wordObject wordItem(to_lower(word), to_lower(meaning));
            DictionaryTree.insert(wordItem); // inserting would implicitly sort it in the AVL structure
        }
    }
    ifs.close();
}

string Dictionary::getTranslation(string & key) { // finding and returning the translation for the queried word
    wordObject queryItem(to_lower(key), "");
    return DictionaryTree.find(queryItem).getMeaning();
}

void Dictionary::addNewTranslation(std::string & key, std::string & newValue) { // add a new translation for a word(key)
    wordObject queryItem(to_lower(key), "");

    // deleting the older version from the dictionary
    string meaning = DictionaryTree.find(queryItem).getMeaning();
    DictionaryTree.remove(DictionaryTree.find(queryItem));

    // inserting the updated version into the dictionary
    meaning += ", ";
    meaning += newValue;
    wordObject NewItem(to_lower(key), to_lower(meaning));
    DictionaryTree.insert(NewItem);
}

void Dictionary::addNewWord(string &key, string &value) {
    // inserting a wordItem to dictionary with given word and meaning values
    wordObject queryItem(to_lower(key), "");
    wordObject NewItem(to_lower(key), to_lower(value));
    DictionaryTree.insert(NewItem);
}

