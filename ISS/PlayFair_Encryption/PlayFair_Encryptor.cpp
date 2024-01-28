#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

// Function to prepare the key matrix for Playfair Cipher
void prepareKeyMatrix(const std::string& key, char keyMatrix[5][5]) {
    std::string uniqueKey = "";
    bool usedChars[26] = {false};

    // Create a string with unique characters from the key
    for (char c : key) {
        if (isalpha(c) && !usedChars[toupper(c) - 'A']) {
            uniqueKey += toupper(c);
            usedChars[toupper(c) - 'A'] = true;
        }
    }

    // Fill the key matrix with the uniqueKey
    int k = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (k < uniqueKey.length()) {
                keyMatrix[i][j] = uniqueKey[k++];
            } else {
                for (char ch = 'A'; ch <= 'Z'; ++ch) {
                    if (ch != 'J' && !usedChars[ch - 'A']) {
                        keyMatrix[i][j] = ch;
                        usedChars[ch - 'A'] = true;
                        break;
                    }
                }
            }
        }
    }
}

// Function to find the positions of two characters in the key matrix
void findPositions(const char keyMatrix[5][5], char ch, int& row, int& col) {
    if (ch == 'J')
        ch = 'I';  // Treat 'J' as 'I' in the key matrix

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (keyMatrix[i][j] == ch) {
                row = i;
                col = j;
                return;
            }
        }
    }
}

// Function to encrypt a digraph using Playfair Cipher
std::string encryptPlayfair(const char keyMatrix[5][5], char ch1, char ch2) {
    int row1, col1, row2, col2;
    findPositions(keyMatrix, ch1, row1, col1);
    findPositions(keyMatrix, ch2, row2, col2);

    std::string result = "";
    if (row1 == row2) {
        result += keyMatrix[row1][(col1 + 1) % 5];
        result += keyMatrix[row2][(col2 + 1) % 5];
    } else if (col1 == col2) {
        result += keyMatrix[(row1 + 1) % 5][col1];
        result += keyMatrix[(row2 + 1) % 5][col2];
    } else {
        result += keyMatrix[row1][col2];
        result += keyMatrix[row2][col1];
    }

    return result;
}

// Function to clean and prepare the input text for Playfair Cipher
std::string prepareInputText(const std::string& input) {
    std::string cleanedText = "";
    for (char c : input) {
        if (isalpha(c)) {
            cleanedText += toupper(c);
            if (c == 'J') {
                // Treat 'J' as 'I' in the input text
                cleanedText += 'I';
            }
        }
    }
    return cleanedText;
}

int main() {
    // Key for the Playfair Cipher (you can change this key)
    std::string key = "KEYWORD";

    // Prepare the key matrix
    char keyMatrix[5][5];
    prepareKeyMatrix(key, keyMatrix);

    // Open input file
    std::ifstream inputFile("input.txt");
    
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open input file." << std::endl;
        return 1;
    }

    // Read content from input file
    std::string inputText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    
    // Close input file
    inputFile.close();

    // Clean and prepare the input text
    std::string cleanedInput = prepareInputText(inputText);

    // Encrypt the text
    std::string encryptedText = "";
    for (std::size_t i = 0; i < cleanedInput.length(); i += 2) {
        char ch1 = cleanedInput[i];
        char ch2 = (i + 1 < cleanedInput.length()) ? cleanedInput[i + 1] : 'X';
        encryptedText += encryptPlayfair(keyMatrix, ch1, ch2);
    }

    // Open output file
    std::ofstream outputFile("encrypted.txt");
    
    if (!outputFile.is_open()) {
        std::cerr << "Unable to create output file." << std::endl;
        return 1;
    }

    // Write encrypted text to output file
    outputFile << encryptedText;

    // Close output file
    outputFile.close();

    std::cout << "Encryption completed. Check encrypted_output.txt for the result." << std::endl;

    return 0;
}