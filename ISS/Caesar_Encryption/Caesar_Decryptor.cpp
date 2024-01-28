#include <iostream>
#include <fstream>

#define SHIFT 3

std::string encryptCaesar(const std::string& text, int shift){
    std::string result = "";

    for(char c : text){
        if(isalpha(c)){
            char base = isupper(c) ? 'A' : 'a';
            result += char((c - base + shift) % 26 + base);
        } else result += c;
    }
    return result;
}

std::string decryptCaesar(const std::string& text, int shift){
    return encryptCaesar(text, -shift);
}

int main(void){
    std::fstream inputFile("encrypted.txt");

    if(!inputFile.is_open()){
        std::cerr << "Unable to open Encrypted File." << std::endl;
        return 1;
    }

    std::string inputText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    inputFile.close();

    std::string decryptedText = decryptCaesar(inputText, SHIFT);

    std::ofstream outputFile("decrypted.txt");

    if(!outputFile.is_open()){
        std::cerr << "Unable to create output file." << std::endl;
        return 1;
    }

    outputFile << decryptedText;
    outputFile.close();

    std::cout << "Decryption Completed!" << std::endl;

    return 0;
}