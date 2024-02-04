#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <ctime>

std::vector<std::vector<int>> keyMatrix = {
    {1, 5},
    {6, 7}
};

void showMatrix(const std::vector<std::vector<int>>& matrix){
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

int minv(int a, int m){
    int x, y;
    int gcd_ = egcd(a, m, x, y);

    if(gcd_ != 1){
        std::cerr << "Inverse does not exist!" << std::endl;
        return -1;
    }
    else{
        int res = (x % m + m) % m;
        return res;
    }
}

std::vector<std::vector<int>> convertToVector(const std::string& text){
    std::vector<std::vector<int>> result(text.size(), std::vector<int>(1));

    for(int i = 0; i < text.size(); i++) result[i][0] = static_cast<int>(text[i] - 'A');

    return result;
}

std::string convertToString(const std::vector<std::vector<int>>& block){
    std::string result = "";

    for(std::vector<int> i : block) for(int j : i) result += static_cast<char>(j + 'A');

    return result;
}

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

std::vector<std::vector<int>> getAdjMatrix(const std::vector<std::vector<int>>& matrix){
    int dim = matrix.size();

    std::vector<std::vector<int>> adjMatrix(dim, std::vector<int>(dim, 0));

    for(int row = 0; row < dim; row++) for(int col = 0; col < dim; col++) adjMatrix[col][row] = ((row+col)%2 ? -1 : 1) * getDeterminant(getShorterMatrix(matrix, row, col));

    return adjMatrix;
}


std::string decryptHill(const std::string& cipherText) {
    int blockSize = keyMatrix.size();

    if (!blockSize || cipherText.size() % blockSize) {
        std::cerr << "Invalid Cipher Text or Key Matrix!" << std::endl;
        return "";
    }

    std::vector<std::vector<int>> adjKeyMatrix = getAdjMatrix(keyMatrix);

    int mKeyDeterminant = minv(getDeterminant(keyMatrix), 26);

    for(int i = 0; i < blockSize; i++) for(int j = 0; j < blockSize; j++) adjKeyMatrix[i][j] = (emod(adjKeyMatrix[i][j], 26) * mKeyDeterminant) % 26;

    std::string decryptedText = "";

    for(int i = 0; i < cipherText.size(); i += blockSize){
        std::vector<std::vector<int>> block = convertToVector(cipherText.substr(i, blockSize));
        block = matrixMul(adjKeyMatrix, block);

        for(int j = 0; j < blockSize; j++) block[j][0] = emod(block[j][0], 26);

        decryptedText += convertToString(block);
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
