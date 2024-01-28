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

int main(void){
    std::fstream inputFile("input.txt");

    if(!inputFile.is_open()){
        std::cerr << "Unable to open Input File." << std::endl;
        return 1;
    }

    std::string inputText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    inputFile.close();

    std::string encryptedText = encryptCaesar(inputText, SHIFT);

    std::ofstream outputFile("encrypted.txt");

    if(!outputFile.is_open()){
        std::cerr << "Unable to create output file." << std::endl;
        return 1;
    }

    outputFile << encryptedText;
    outputFile.close();

    std::cout << "Encryption Completed!" << std::endl;

    return 0;
}