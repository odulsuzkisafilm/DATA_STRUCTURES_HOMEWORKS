// Hamit Efe Cinar 30925
#include <iostream>
#include <string>
#include "Dictionary.h"
using namespace std;

int main() {
    Dictionary EngToTrDict;
    cout << "***Help***\nQuery: 1\nAdd new translation: 2\nAdd new word: 3\nExit: 0" << endl;

    string command;
    cout << "Enter Command: ";
    cin >> command;
    while(command!="0"){
        if(command=="1"){ // Query specific word: gets a word from user and shows it’s translation;
            string queriedWord;
            cout << "Enter queried word: ";
            cin >> queriedWord;

            string translation = EngToTrDict.getTranslation(queriedWord);
            cout << queriedWord << " ---> " << translation << endl;
        }else if(command=="2"){ // Add new translation for existing word: checks whether the word exists or not,
            // if it exists program gets translation from user, otherwise shows message that word does not exist;
            string key, newMeaning;
            cout << "Which word do you want to edit: ";
            cin >> key;
            if(EngToTrDict.getTranslation(key) != "-1") {
                cout << "Enter the new translation: ";
                cin.ignore();
                getline (cin, newMeaning);
                EngToTrDict.addNewTranslation(key, newMeaning);
                cout << "***UPDATED***" << endl;
            }
            else cout << "Word does not exist, if you want to add new word please use command 3." << endl;
        }else if(command=="3"){ // Add new word: checks whether the word exists or not, if it exists the program shows
            // message that word exists, otherwise program gets word and its translation from user;
            string newWord, translations;
            cout << "Enter New Word: ";
            cin >> newWord;
            if(EngToTrDict.getTranslation(newWord) == "-1") { // if newWord is not in the dictionary
                cout << "Enter Translations: ";
                cin.ignore();
                getline (cin, translations);
                EngToTrDict.addNewWord(newWord, translations);
            }else{
                cout << "Word already exists, if you want to add new translation please use command 2." << endl;
            }
        }

        cout << "Enter Command: ";
        cin >> command;
    }
    cout << "Bye!" << endl;

    return 0;
}
