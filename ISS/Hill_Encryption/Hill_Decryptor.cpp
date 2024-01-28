#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <ctime>
#include <cmath>

std::vector<std::vector<int>> keyMatrix = {
    {6, 24, 1},
    {13, 16, 10},
    {20, 17, 15}
};

void showMatrix(const std::vector<std::vector<int>>& matrix){
    for(int row = 0; row < matrix.size(); row++){
        for(int col = 0; col < matrix[0].size(); col++) std::cout << matrix[row][col] << " ";
        std::cout << std::endl;
    }
}

void showMatrix(const std::vector<std::vector<float>>& matrix){
    for(int row = 0; row < matrix.size(); row++){
        for(int col = 0; col < matrix[0].size(); col++) std::cout << matrix[row][col] << " ";
        std::cout << std::endl;
    }
}

int emod(int a, int b){
    return ((a%b)+b)%b;
}

int egcd(int a, int b, int& x, int& y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    int x1, y1;
    int gcd = egcd(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

    return gcd;
}

int minv(int a, int m) {
    int x, y;
    int gcd = egcd(a, m, x, y);

    if (gcd != 1) {
        std::cerr << "Modular inverse does not exist!" << std::endl;
        return -1;
    } else {
        return emod(x, m);
    }
}

std::vector<std::vector<float>> convertToVector(const std::string& text) {
    std::vector<std::vector<float>> block(1, std::vector<float>(text.size(), 0));

    for (int i = 0; i < text.size(); i++) 
        block[0][i] = static_cast<float>(toupper(text[i]) - 'A');

    return block;
}

std::vector<std::vector<float>> matrixMul(const std::vector<std::vector<float>>& matrix1, const std::vector<std::vector<float>>& matrix2){
    std::size_t m1 = matrix1.size();
    std::size_t n1 = matrix1[0].size();

    std::size_t m2 = matrix2.size();
    std::size_t n2 = matrix2[0].size();

    if(n1 != m2){
        std::cout << "Multiplication Error! Order 1 : " << m1 << " X " << n1 << ", Order 2 : " << m2 << " X " << n2 << std::endl;
        return std::vector<std::vector<float>>();
    }

    std::vector<std::vector<float>> resultMatrix(m1, std::vector<float>(n2, 0.f));

    for(std::size_t i = 0; i < m1; i++){
        for(std::size_t j = 0; j < n2; j++){
            for(std::size_t k = 0; k < n1; k++) resultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
        }
    }
    return resultMatrix;
}

std::vector<std::vector<int>> getShorterMatrix(const std::vector<std::vector<int>>& matrix, int row, int col){
    int dim = matrix.size();
    int rowCount = 0;
    int colCount = 0;

    std::vector<std::vector<int>> shortMatrix(dim-1, std::vector<int>(dim-1, 0));

    for(int rrow = 0; rrow < dim; rrow++){
        if(rrow == row) continue;
        for(int ccol = 0; ccol < dim; ccol++){
            if(col == ccol) continue;
            shortMatrix[rowCount][colCount] = matrix[rrow][ccol];
            colCount++;
        }
        rowCount++;
        colCount = 0;
    }

    return shortMatrix;
}

int getDeterminant(const std::vector<std::vector<int>>& matrix){
    int dim1 = matrix.size();
    int dim2 = matrix[0].size();

    if(dim1 != dim2){
        std::cerr << "Unable to perform determinant operation on non-square matrix!" << std::endl;
        return int();
    }

    if(dim1 == 1) return matrix[0][0];
    if(dim1 == 2) return matrix[0][0] * matrix[1][1] - (matrix[0][1] * matrix[1][0]);

    int deter = 0;
    int colCount;

    for(int ccol = 0; ccol < dim2; ccol++){
        std::vector<std::vector<int>> shortMatrix = getShorterMatrix(matrix, 0, ccol);

        if(ccol%2) deter += matrix[0][ccol] * getDeterminant(shortMatrix) * -1;
        else deter += matrix[0][ccol] * getDeterminant(shortMatrix);
    }

    return deter;
}

std::vector<std::vector<float>> getAdjMatrix(const std::vector<std::vector<int>>& matrix){
    int dim = matrix.size();
    float determinant = 1.0f / static_cast<float>(getDeterminant(matrix));

    std::vector<std::vector<float>> adjMatrix(dim, std::vector<float>(dim, 0));

    for(int row = 0; row < dim; row++) for(int col = 0; col < dim; col++) adjMatrix[col][row] = static_cast<float>((row+col)%2 ? -1 : 1) * static_cast<float>(getDeterminant(getShorterMatrix(matrix, row, col)));

    return adjMatrix;
}


std::string decryptHill(const std::string& cipherText) {
    int blockSize = keyMatrix.size();

    if (!blockSize || cipherText.size() % blockSize) {
        std::cerr << "Invalid Cipher Text or Key Matrix!" << std::endl;
        return "";
    }

    std::vector<std::vector<float>> adjKeyMatrix = getAdjMatrix(keyMatrix);

    int mKeyDeterminant = minv(getDeterminant(keyMatrix), 26);

    std::string decryptedText = "";

    for (std::size_t i = 0; i < cipherText.length(); i += blockSize) {
        std::vector<std::vector<float>> block = convertToVector(cipherText.substr(i, blockSize));

        std::vector<std::vector<float>> resultMatrix = matrixMul(block, adjKeyMatrix);

        for (std::size_t j = 0; j < blockSize; j++) {
            int decryptedChar = emod(static_cast<int>(resultMatrix[0][j]), 26);
            decryptedChar = emod(decryptedChar * mKeyDeterminant, 26);
            decryptedText += static_cast<char>(decryptedChar + 'A');
        }
    }

    return decryptedText;
}


int main(void){
    std::ifstream inputFile("encrypted.txt");

    if(!inputFile.is_open()){
        std::cerr << "Unable to open Input File!" << std::endl;
        return 1;
    }

    std::string inputText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    inputFile.close();

    std::string decryptedText = decryptHill(inputText);

    std::ofstream outputFile("decrypted.txt");

    if(!outputFile.is_open()){
        std::cerr << "Unable to create Output File!" << std::endl;
        return 1;
    }

    outputFile << decryptedText;
    outputFile.close();

    std::cout << "Decryption Completed!" << std::endl;

    return 0;
}