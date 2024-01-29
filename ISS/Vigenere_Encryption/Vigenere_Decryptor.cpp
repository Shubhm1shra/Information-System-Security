#include <iostream>
#include <string>
#include <fstream>

std::string key = "key";

std::string decryptVigenere(const std::string& cipherText,  std::string key){
    std::string result;

    if(key.size() > cipherText.size()) key = key.substr(0, cipherText.size());
    else if(key.size() < cipherText.size()){
        int count = 0;
        while(key.size() < cipherText.size()){
            key += key[count];
            if(++count == key.size()) count = 0;
        }
    }

    for(int i = 0; i < cipherText.size(); i++){
        if(isalpha(cipherText[i])){
            char shift = std::toupper(key[i]) - 'A';

            if(isupper(cipherText[i])) result += (cipherText[i] - shift - 'A' + 26) % 26 + 'A';
            else result += (cipherText[i] - shift - 'a' + 26) % 26 + 'a';
        }
        else result += cipherText[i];
    }

    return result;
}

int main(void){
    std::fstream inputFile("encrypted.txt");

    if(!inputFile.is_open()){
        std::cerr << "Unable to open Input File!" << std::endl;
        return 1;
    }

    std::string inputText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    inputFile.close();

    std::string encryptedText = decryptVigenere(inputText, key);

    std::ofstream outputFile("decrypted.txt");

    if(!outputFile.is_open()){
        std::cerr << "Unable to create Encrypted File!" << std::endl;
        return 1;
    }

    outputFile << encryptedText;
    outputFile.close();

    std::cout << "Encryption Completed!" << std::endl;

    return 0;
}