#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <ctime>

std::vector<std::vector<int>> keyMatrix = {
    {1, 5},
    {6, 7}
};

std::vector<std::vector<int>> matrixMul(const std::vector<std::vector<int>>& matrix1, const std::vector<std::vector<int>>& matrix2){
    std::size_t m1 = matrix1.size();
    std::size_t n1 = matrix1[0].size();

    std::size_t m2 = matrix2.size();
    std::size_t n2 = matrix2[0].size();

    if(n1 != m2){
        std::cout << "Multiplication Error! Order 1 : " << m1 << " X " << n1 << ", Order 2 : " << m2 << " X " << n2 << std::endl;
        return std::vector<std::vector<int>>();
    }

    std::vector<std::vector<int>> resultMatrix(m1, std::vector<int>(n2, 0));

    for(std::size_t i = 0; i < m1; i++){
        for(std::size_t j = 0; j < n2; j++){
            for(std::size_t k = 0; k < n1; k++) resultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
        }
    }
    return resultMatrix;
}

std::vector<std::vector<int>> convertToVector(const std::string& block){
    std::vector<std::vector<int>> result;
    std::vector<int> vectorBlock;
    char base;

    for(char c : block){
        base = isupper(c) ? 'A' : 'a';
        vectorBlock.push_back(static_cast<int>(c - base));
        result.push_back(vectorBlock);
        vectorBlock.clear();
    }

    return result;
}

std::string convertToString(const std::vector<std::vector<int>> vec){
    std::string result = "";

    for(std::vector<int> row : vec) for(int i : row) result += static_cast<char>((i % 26) + 'A');

    return result;
}

std::string encryptHill(const std::string& plainText){
    std::string cipherText = "";
    std::string paddedText = plainText;

    std::size_t blockSize = keyMatrix[0].size();
    std::vector<std::vector<int>> convertedToVector;
    std::vector<std::vector<int>> convertedBlock;

    while(paddedText.size() % blockSize != 0) paddedText += 'X';

    for(std::size_t i = 0; i < paddedText.size(); i += blockSize){
        convertedToVector = convertToVector(paddedText.substr(i, blockSize));
        convertedBlock = matrixMul(keyMatrix, convertedToVector);
        cipherText += convertToString(convertedBlock);
    }

    return cipherText;
}

int main(void){
    std::ifstream inputFile("input.txt");

    if(!inputFile.is_open()){
        std::cerr << "Unable to open Input File!" << std::endl;
        return 1;
    }

    std::string inputText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    inputFile.close();

    std::string encryptedText = encryptHill(inputText);

    std::ofstream outputFile("encrypted.txt");

    if(!outputFile.is_open()){
        std::cerr << "Unable to create Output File!" << std::endl;
        return 1;
    }

    outputFile << encryptedText;
    outputFile.close();

    std::cout << "Encryption Completed!" << std::endl;

    return 0;
}
