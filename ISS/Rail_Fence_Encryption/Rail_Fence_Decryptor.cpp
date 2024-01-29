#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int depth = 3;

std::string decryptRailFence(const std::string& cipherText){
    std::string result = "";

    std::vector<std::string> rail_and_fence(depth, "");
    std::vector<int> rowLengths(depth, 0);

    int curRow = 0;
    bool goDown = false;

    for(int i = 0; i < cipherText.size(); i++){
        rowLengths[curRow]++;
        if(curRow == 0 || curRow == (depth-1)) goDown = !goDown;

        curRow += goDown ? 1 : -1;
    }

    int ind = 0;
    for(int i = 0; i < depth; i++) for(int j = 0; j < rowLengths[i]; j++) rail_and_fence[i] += cipherText[ind++];

    curRow = 0;
    goDown = false;

    for(int i = 0; i < cipherText.size(); i++){
        result += rail_and_fence[curRow][0];

        rail_and_fence[curRow] = rail_and_fence[curRow].substr(1);

        if(curRow == 0 || curRow == (depth - 1)) goDown = !goDown;
        curRow += goDown ? 1 : -1;
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

    std::ofstream outputFile("decrypted.txt");

    if(!outputFile.is_open()){
        std::cerr << "Unable to create Output File!" << std::endl;
        return 1;
    }

    std::string decryptedText = decryptRailFence(inputText);

    outputFile << decryptedText;

    outputFile.close();

    std::cout << "Decryption Completed!" << std::endl;

    return 0;
}