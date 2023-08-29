// Efe Cinar

#include <iostream>
#include <vector>
#include <string>
#include "SearchEngine.h"

using namespace std;


int main() {
    vector<string> fileNames;
    int numberOfFiles;
    cout << "Enter number of input files: ";
    cin >> numberOfFiles;
    for (int i=1; i<=numberOfFiles; i++){
        string fileName;
        cout << "Enter " << i << ". file name: ";
        cin >> fileName;
        fileNames.push_back(fileName);
    }
    SearchEngine google;
    for(const string& file : fileNames){
        google.fillHash(file);
        google.fillTree(file);
    }

    cout << endl;
    google.getHashInfo();

    string query;
    cout << "Enter queried words in one line: ";
    cin.ignore();
    getline(cin, query);
    google.queryWords(to_lower(query), fileNames);

    return 0;
}
