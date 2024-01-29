#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int depth = 3;

/*
hloel

h l o
 e l

h   o
 e l
  l
*/

std::string encryptRailFence(const std::string& plainText){
    std::vector<std::string> rail_and_fence(depth, "");
    std::string result = "";

    bool goDown = false;

    int curRow = 0;

    for(char c : plainText){
        if(curRow == 0 || curRow == (depth-1)) goDown = !goDown;

        rail_and_fence[curRow] += c;
        curRow += goDown ? 1 : -1;
    }

    for(int i = 0; i < depth; i++) result += rail_and_fence[i];

    return result;
}

int main(void){
    std::fstream inputFile("input.txt");

    if(!inputFile.is_open()){
        std::cerr << "Unable to open Input file!" << std::endl;
        return 1;
    }

    std::string inputText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    inputFile.close();

    std::ofstream outputFile("encrypted.txt");

    if(!outputFile.is_open()){
        std::cerr << "Unable to create Output File!" << std::endl;
        return 1;
    }

    std::string encryptedText = encryptRailFence(inputText);

    outputFile << encryptedText;

    outputFile.close();

    std::cout << "Encryption Completed!" << std::endl;

    return 0;
}