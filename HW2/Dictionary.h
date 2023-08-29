//
// Created by Efe Cinar on 4.08.2023.
//

#ifndef CS300HW2_DICTIONARY_H
#define CS300HW2_DICTIONARY_H

#include <string>
#include "AvlSearchTree.h"

class Dictionary;

class wordObject{
private:
    std::string meaning;
    std::string word;
public:
    explicit wordObject(const std::string & engWord, const std::string & turkWord) // constructor
    : word(engWord), meaning(turkWord){}

    wordObject& operator=(const wordObject & rhs){ // overloaded operator function that simply does copy assignment
        if(this != &rhs) {meaning = rhs.meaning; word = rhs.word;}
        return *this;
    }

    // accessor member functions
    const std::string getWord() const {return word;}
    const std::string getMeaning() const {return meaning;}

    friend class Dictionary;
};



class Dictionary{
private:
    AvlSearchTree<wordObject> DictionaryTree;
public:
    Dictionary(); // constructor that constructs the dictionary tree with words and meanings in the dict.txt file
    std::string getTranslation(std::string & key); // command 1: Query specific word
    void addNewTranslation(std::string & key, std::string & newValue); // command 2: Add new translation for existing word
    void addNewWord(std::string & key, std::string & value); // command 3: Add new word
};


#endif //CS300HW2_DICTIONARY_H
