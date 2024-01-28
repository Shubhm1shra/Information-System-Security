#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

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

// Function to decrypt a digraph using Playfair Cipher
std::string decryptPlayfair(const char keyMatrix[5][5], char ch1, char ch2) {
    int row1, col1, row2, col2;
    findPositions(keyMatrix, ch1, row1, col1);
    findPositions(keyMatrix, ch2, row2, col2);

    std::string result = "";
    if (row1 == row2) {
        result += keyMatrix[row1][(col1 - 1 + 5) % 5];
        result += keyMatrix[row2][(col2 - 1 + 5) % 5];
    } else if (col1 == col2) {
        result += keyMatrix[(row1 - 1 + 5) % 5][col1];
        result += keyMatrix[(row2 - 1 + 5) % 5][col2];
    } else {
        result += keyMatrix[row1][col2];
        result += keyMatrix[row2][col1];
    }

    return result;
}

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
    // Key for the Playfair Cipher (should be the same key used for encryption)
    std::string key = "KEYWORD";

    // Prepare the key matrix
    char keyMatrix[5][5];
    prepareKeyMatrix(key, keyMatrix);

    // Open encrypted input file
    std::ifstream encryptedInputFile("encrypted.txt");
    
    if (!encryptedInputFile.is_open()) {
        std::cerr << "Unable to open encrypted input file." << std::endl;
        return 1;
    }

    // Read encrypted content from file
    std::string encryptedText((std::istreambuf_iterator<char>(encryptedInputFile)), std::istreambuf_iterator<char>());
    
    // Close encrypted input file
    encryptedInputFile.close();

    // Clean and prepare the encrypted text
    std::string cleanedEncryptedText = prepareInputText(encryptedText);

    // Decrypt the text
    std::string decryptedText = "";
    for (std::size_t i = 0; i < cleanedEncryptedText.length(); i += 2) {
        char ch1 = cleanedEncryptedText[i];
        char ch2 = (i + 1 < cleanedEncryptedText.length()) ? cleanedEncryptedText[i + 1] : 'X';
        decryptedText += decryptPlayfair(keyMatrix, ch1, ch2);
    }

    // Open output file for decrypted text
    std::ofstream decryptedOutputFile("decrypted.txt");
    
    if (!decryptedOutputFile.is_open()) {
        std::cerr << "Unable to create decrypted output file." << std::endl;
        return 1;
    }

    // Write decrypted text to output file
    decryptedOutputFile << decryptedText;

    // Close decrypted output file
    decryptedOutputFile.close();

    std::cout << "Decryption completed!" << std::endl;

    return 0;
}