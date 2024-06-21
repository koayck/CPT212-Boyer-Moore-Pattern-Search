#include <iostream>
using namespace std;
#define NO_OF_CHARS 256

// Create lookup table for bad character heuristic
void badCharHeuristic (string pattern, int size, int badchar[NO_OF_CHARS]) {
    int i;

    // Initialize the table to -1 which means never occur
    for (int i = 0;i < NO_OF_CHARS;i++) {
        badchar[i] = -1;
    }

    // Fill the index/position of last occurrence of a character in the pattern
    for (i = 0; i < size; i++) {
        // Cast string into integer
        badchar[(int)pattern[i]] = i;
    }
}

// Preprocessing for strong good suffix heuristic
void good_preprocessing_strong(int* shift, int* bpos, string pattern, int pattern_size) {
    int i = pattern_size, j = pattern_size + 1;
    
    // Set the value of last index in bpos array to j
    bpos[i] = j;

    // If there exist character that haven't be checked
    while (i > 0) {
        // Pointer is not out of bound and the character at each pointer not match
        while (j <= pattern_size && pattern[i-1] != pattern[j-1]) {
            // Current position that mismatched haven't be filled
            if (shift[j] == 0) {
                // Set the value to the distance of the two pointers being compared
                shift[j] = j - i;
            }

            // Move the pointer back to the position recorded in bpos to find position of the next border
            j = bpos[j];
        }

        // If j is out of bound or the two patterns, pattern[i-1] and pattern[j-1], border discovered
        i--;
        j--;
        // Store the beginning position of the border into bpos
        bpos[i] = j;
    }
}

// Handles cases where the suffix that mismatches does not occur elsewhere in the pattern
void good_preprocessing_case2(int* shift, int* bpos, int pattern_size) {
    int j;

    // initialize j to the border position for the whole pattern
    j = bpos[0];

    // Loop through the whole pattern
    for (int i = 0;i <= pattern_size;i++) {
        // If current position still doesn't have a value needed to be shifted if mismatched
        if (shift[i]==0) {
            // Set the value to the border of the first suffix
            shift[i] = j;
        }

        // If suffix becomes shorter than bpos[0]
        if (i == j) {
            // Change j to the border of the second suffix
            j = bpos[j];
        }
    }
}

// Search for pattern in provided text
void search(string text, string pattern) {
    int iteration = 1, bad = 0, good = 0;

    int pattern_size = pattern.size();
    int text_size = text.size();
    int badchar[NO_OF_CHARS];
    // Lookup table for good suffix heuristic and border position that contains the starting index of border for suffix starting at current index in the pattern
    int goodShiftTable[pattern_size + 1], bpos[pattern_size + 1];

    // Initialize all occurrence of shift to 0
    for(int i = 0;i<pattern_size + 1;i++) {
        goodShiftTable[i] = 0;
    } 

    // Create the lookup table for bad character heuristic using preprocessing function badCharHeuristic()
    badCharHeuristic(pattern, pattern_size, badchar);

    // Create the lookup table for the good suffix heuristic using preprocessing functions
    // goodShiftTable[cur + 1] stores how much the pattern needs to be shifted to right if a mismatch occurs at pattern[cur]
    good_preprocessing_strong(goodShiftTable, bpos, pattern, pattern_size);
    good_preprocessing_case2(goodShiftTable, bpos, pattern_size);

    // Initialize the counter to record how many shifts done after each compare
    int shift = 0;

    // Check pattern occurence at which index
    // Loop if remaining characters that haven't check in the text is larger than pattern size
    while (shift <= (text_size - pattern_size)) {
        int badCharShift = shift, goodSuffixShift = shift;

        // Initialize the variable of current pointer index in the pattern (start from last character)
        int cur = pattern_size - 1;
        
        cout << "iteration= " << iteration << endl;

        // Compare characters from the end of the pattern with the corresponding characters in the text, moving from right to left
        // Continue as long as characters match and there are characters left to compare
        while (cur >= 0 && pattern[cur] == text[shift + cur]) {
            cur--;
        }

        // If pattern matched
        if (cur < 0) {
            cout << "Pattern found at shift = " << shift << endl;
            
            // If the remaining characters haven't check in the text larger than the pattern size
            if ((shift + pattern_size) < text_size) {
                // Shift the pattern so that the next character in text aligns with the last occurrence of it in pattern
                // If the next character in text doesn't occur in pattern, skip the character
                badCharShift += pattern_size - badchar[text[shift + pattern_size]];
                goodSuffixShift += goodShiftTable[0];
            }
            else {
                badCharShift++;
            }
        }
        // If pattern doesn't match
        else {
            int last_occurrence = -1;
            // Check if there is still character at the left of the pattern
            for (int i = cur - 1;i >= 0;i--) {
                if (pattern[i] == text[shift + cur]) {
                    last_occurrence = i;
                    break;
                }
            }

            if (last_occurrence == -1) {
                // No occurrence to the left, shift by whole pattern length
                badCharShift += pattern_size;
            }
            else {
                // Shift pattern so that it aligns with last occurrence
                badCharShift += cur - last_occurrence;
            }

            // Shift the pattern goodShiftTable[cur + 1] times
            goodSuffixShift += goodShiftTable[cur + 1];
        }

        if (badCharShift >= goodSuffixShift) {
            bad++;
        }
        else {
            good++;
        }

        // Choose the max value shifts to get most efficient solution
        shift = max(badCharShift, goodSuffixShift);
        cout << "bad= " << bad << endl;
        cout << "bad suggested= " << badCharShift << endl;
        cout << "good= " << good << endl;
        cout << "good suggested= " << goodSuffixShift << endl;
        cout << "shift= " << shift << endl << endl;

        iteration++;
    }
}

// Driver code
int main() {
    string text, pattern;

    while (true) {
        cout << "Enter the text: " << endl;
        cin >> text;
        cout << "Enter the pattern you want to find: " << endl;
        cin >> pattern;

        if (text.size() < pattern.size()) {
            cout << "The text should be longer than the pattern!" << endl;
        }
        else {
            search(text, pattern);
            break;
        }
    }
    
    return 0;
}