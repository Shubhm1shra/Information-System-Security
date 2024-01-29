#include <iostream>
#include <string>
#include <fstream>

std::string key = "key";

std::string encryptVigenere(const std::string& plainText,  std::string key){
    std::string result;

    if(key.size() > plainText.size()) key = key.substr(0, plainText.size());
    else if(key.size() < plainText.size()){
        int count = 0;
        while(key.size() < plainText.size()){
            key += key[count];
            if(++count == key.size()) count = 0;
        }
    }

    for(int i = 0; i < plainText.size(); i++){
        if(isalpha(plainText[i])){
            char shift = std::toupper(key[i]) - 'A';

            if(isupper(plainText[i])) result += (plainText[i] + shift - 'A') % 26 + 'A';
            else result += (plainText[i] + shift - 'a') % 26 + 'a';
        }
        else result += plainText[i];
    }

    return result;
}

int main(void){
    std::fstream inputFile("input.txt");

    if(!inputFile.is_open()){
        std::cerr << "Unable to open Input File!" << std::endl;
        return 1;
    }

    std::string inputText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    inputFile.close();

    std::string encryptedText = encryptVigenere(inputText, key);

    std::ofstream outputFile("encrypted.txt");

    if(!outputFile.is_open()){
        std::cerr << "Unable to create Encrypted File!" << std::endl;
        return 1;
    }

    outputFile << encryptedText;
    outputFile.close();

    std::cout << "Encryption Completed!" << std::endl;

    return 0;
}