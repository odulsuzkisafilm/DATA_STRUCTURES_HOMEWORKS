//
// Created by Efe Cinar on 16.08.2023.
//

#ifndef CS300_HW3_SEARCHENGINE_H
#define CS300_HW3_SEARCHENGINE_H

#include "AvlSearchTree.h"
#include "HashTable.h"
#include <fstream>
#include <chrono>
#include <sstream>

struct DocumentItem {
    std::string documentName;
    int count;
    explicit DocumentItem(const std::string & docName, int num): documentName(docName), count(num){}
};
struct WordItem {
    std::string word;
    std::vector<DocumentItem> listOfDocuments;
    WordItem(): word("-1"){};
    explicit WordItem(const std::string & Word): word(Word){}
};

class SearchEngine {
private:
    HashTable <WordItem> hashQuery;
    AvlSearchTree <WordItem> treeQuery;
public:
    SearchEngine();
    void fillHash(const std::string & fileName);
    void fillTree(const std::string & fileName);
    void getHashInfo();
    void queryWords(const std::string & queryLine, const std::vector<std::string> & fileNames);
};

bool operator==(const WordItem & lhs, const WordItem & rhs){return lhs.word == rhs.word;}
bool operator!=(const WordItem & lhs, const WordItem & rhs){return lhs.word != rhs.word;}
bool operator<(const WordItem & lhs, const WordItem & rhs){return lhs.word < rhs.word;}
bool operator>(const WordItem & lhs, const WordItem & rhs){return lhs.word > rhs.word;}
WordItem WordItemNotFound; WordItem WordItemNotFound2; // ITEM_NOT_FOUND objects for the hashtable and tree classes

std::string & to_lower(std::string & word){ // to make the program case-insensitive, all the words are made lowercase for the dict
    for (int i=0; i < word.size(); i++){
        if (word[i] >= 'A' && word[i] <= 'Z'){
            word[i] -= ('Z' - 'z');
        }
    }
    return word;
}

SearchEngine::SearchEngine(): hashQuery(WordItemNotFound), treeQuery(WordItemNotFound2){}

bool isAlphabetic(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void SearchEngine::fillHash(const std::string &fileName) {
    std::ifstream ifs(fileName);
    std::string line, word;
    while (getline(ifs, line)) {
        std::string cleanedWord;
        if(!line.empty()) { // skipping blank lines
            std::istringstream ss(line);
            while(ss>>word) {
                for (char c: word) {
                    if (isAlphabetic(c)) { // till the loop encounters a non-alph word, chars add up to the word
                        cleanedWord += c;
                    } else { // when it encounters, belonging wordItem in the hash is updated
                        if (!cleanedWord.empty()) {
                            WordItem item(to_lower(cleanedWord));
                            if (hashQuery.find(item).word == "-1") {
                                item.listOfDocuments.push_back(DocumentItem(fileName, 1));
                                hashQuery.insert(item);
                            } else {
                                bool fileHadTheWord = false;
                                for (DocumentItem &d: hashQuery.find(item).listOfDocuments) {
                                    if (d.documentName == fileName) {
                                        fileHadTheWord = true;
                                        d.count++;
                                        break;
                                    }
                                }
                                if(!fileHadTheWord) hashQuery.find(item).listOfDocuments.push_back(DocumentItem(fileName, 1));
                            }
                            cleanedWord = ""; // word to be add is initialized for future words
                        }
                    }
                }
                if (!cleanedWord.empty()) { // if the line does not end with a non-alph char, operation is remade again
                    WordItem item(to_lower(cleanedWord));
                    if (hashQuery.find(item).word == "-1") {
                        item.listOfDocuments.push_back(DocumentItem(fileName, 1));
                        hashQuery.insert(item);
                    } else {
                        bool a = false;
                        for (DocumentItem &d: hashQuery.find(item).listOfDocuments) {
                            if (d.documentName == fileName) {
                                a = true;
                                d.count++;
                                break;
                            }
                        }
                        if(!a) hashQuery.find(item).listOfDocuments.push_back(DocumentItem(fileName, 1));
                    }
                    cleanedWord = "";
                }
            }
        }
    }
}

void SearchEngine::fillTree(const std::string &fileName) {
    std::ifstream ifs(fileName);
    std::string line, word;
    while (getline(ifs, line)) {
        std::string cleanedWord;
        if(!line.empty()) { // skipping blank lines
            std::istringstream ss(line);
            while(ss>>word) {
                for (char c: word) {
                    if (isAlphabetic(c)) { // till the loop encounters a non-alph word, chars add up to the word
                        cleanedWord += c;
                    } else { // when it encounters, belonging wordItem in the tree is updated
                        if (!cleanedWord.empty()) {
                            WordItem item(to_lower(cleanedWord));
                            if (treeQuery.find(item).word == "-1") {
                                item.listOfDocuments.push_back(DocumentItem(fileName, 1));
                                treeQuery.insert(item);
                            } else {
                                bool fileHadTheWord = false;
                                for (DocumentItem &d: treeQuery.find(item).listOfDocuments) {
                                    if (d.documentName == fileName) {
                                        fileHadTheWord = true;
                                        d.count++;
                                        break;
                                    }
                                }
                                if(!fileHadTheWord) treeQuery.find(item).listOfDocuments.push_back(DocumentItem(fileName, 1));
                            }
                            cleanedWord = ""; // word to be add is initialized for future words
                        }
                    }
                }
                if (!cleanedWord.empty()) { // if the line does not end with a non-alph char, operation is remade again
                    WordItem item(to_lower(cleanedWord));
                    if (treeQuery.find(item).word == "-1") {
                        item.listOfDocuments.push_back(DocumentItem(fileName, 1));
                        treeQuery.insert(item);
                    } else {
                        bool a = false;
                        for (DocumentItem &d: treeQuery.find(item).listOfDocuments) {
                            if (d.documentName == fileName) {
                                a = true;
                                d.count++;
                                break;
                            }
                        }
                        if(!a) treeQuery.find(item).listOfDocuments.push_back(DocumentItem(fileName, 1));
                    }
                    cleanedWord = "";
                }
            }
        }
    }
}



void SearchEngine::getHashInfo() {
    std::cout << "After preprocessing, the unique word count is " << hashQuery.getCurrentSize()
              << ". Current load ratio is " << hashQuery.getCurrentSize() * 1.0 /hashQuery.getTableSize() << std::endl;
}

void queryOutput(const std::vector <WordItem> & wordItems, const std::vector<std::string> & fileNames){
    bool relevantFiles = false;

    for (const std::string& file : fileNames) {
        bool passThisFile = false;

        for (int i=0; i < wordItems.size(); i++) {
            bool found = false;

            for (const DocumentItem& documentItem : wordItems[i].listOfDocuments) {
                if (documentItem.documentName == file) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                passThisFile = true;
                break;
            }
        }

        if (!passThisFile) {
            bool notFirstWord = false;

            for (int i=0; i < wordItems.size(); i++) {

                for (const DocumentItem& documentItem : wordItems[i].listOfDocuments) {
                    if (documentItem.documentName == file) {
                        if (!notFirstWord) {
                            notFirstWord = true;
                            std::cout << "In Document " << file;
                        }
                        std::cout << ", " << wordItems[i].word << " found " << documentItem.count << " times";
                    }
                }
            }

            if (notFirstWord) {
                std::cout << "." << std::endl;
                relevantFiles = true;
            }
        }
    }

    if (!relevantFiles) {
        std::cout << "No document contains the given query" << std::endl;
    }
}

void SearchEngine::queryWords(const std::string & queryLine, const std::vector<std::string> & fileNames) {
    std::string word;
    std::vector<std::string> wordsToQuery;

    // formatting the queryLine into a query words list
    if(!queryLine.empty()) {
        for (char c: queryLine) {
            if (isAlphabetic(c)) {
                word += c;
            } else {
                if(!word.empty()){
                    wordsToQuery.push_back(word);
                }
                word = "";
            }
        }
        if(!word.empty()){
            wordsToQuery.push_back(word);
        }
    }else return;

    /**********************************************************************/
    std::vector <WordItem> wordItems; // to find and store the query words' corresponding items in the data structures
    int k = 20;

    auto start = std::chrono::high_resolution_clock::now();
    // QueryDocuments(with BST);

    for (const std::string& queryWord : wordsToQuery) {
        wordItems.push_back(treeQuery.find(WordItem(queryWord)));
    }
    for(int i=0; i<k-1; i++){
        wordItems.clear();
        for (const std::string& queryWord : wordsToQuery) {
            wordItems.push_back(treeQuery.find(WordItem(queryWord)));
        }
    }

    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    queryOutput(wordItems, fileNames);
    /**********************************************************************/
    wordItems.clear(); // initializing to use the same vector for the hash table to reduce memory usage
    start = std::chrono::high_resolution_clock::now();
    // QueryDocuments (with hashtable);
    for (const std::string& queryWord : wordsToQuery) {
        wordItems.push_back(hashQuery.find(WordItem(queryWord)));
    }

    for(int i=0; i<k-1; i++){
        wordItems.clear();
        for (const std::string& queryWord : wordsToQuery) {
            wordItems.push_back(hashQuery.find(WordItem(queryWord)));
        }
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    queryOutput(wordItems, fileNames);
    /**********************************************************************/

    std::cout << "\nTime: " << BSTTime.count()/20.0 << "\n";
    std::cout << "\nTime: " << HTTime.count()/20.0 << "\n";
    std::cout << "Speed Up: " << BSTTime.count() * 1.0 / HTTime.count() << std::endl;
}

#endif //CS300_HW3_SEARCHENGINE_H
