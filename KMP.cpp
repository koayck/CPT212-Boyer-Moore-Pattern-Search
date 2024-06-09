#include <iostream>
using namespace std;

void generateLPS(string pattern, int *LPS)
{
    int length = 0;
    LPS[0] = 0; // LPS[0] is always 0
    int i = 1;

    // calculates LPS[i] for i = 1 to M-1
    while (i < pattern.size())
    {
        if (pattern[i] == pattern[length])
        {
            length++;
            LPS[i] = length;
            i++;
        }
        else // (pat[i] != pat[len])
        {
            if (length != 0)
            {
                length = LPS[length - 1];
            }
            else // if (len == 0)
            {
                LPS[i] = 0;
                i++;
            }
        }
    }
}

void KMP(string text, string pattern, int *LPS)
{
    int text_length = text.length();
    int pattern_length = pattern.length();
    int i = 0;          // index for text[]
    int j = 0;          // index for pattern[]
    bool found = false; // flag to indicate if pattern is found
    while (i < text_length)
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }

        if (j == pattern_length)
        {
            cout << "Pattern found at index " << i - j << endl;
            j = LPS[j - 1];
            found = true; // pattern found
        }
        else if ((i < text_length) && (pattern[j] != text[i]))
        {
            if (j != 0)
                j = LPS[j - 1];
            else
                i = i + 1;
        }
    }
    if (!found) // if pattern not found
    {
        cout << "No pattern found" << endl;
    }
}

int main()
{
    // accept string and pattern from user
    string pattern, text;
    cout << "Enter the text: ";
    getline(cin, text);
    cout << "Enter the pattern: ";
    getline(cin, pattern);

    int m = pattern.length(); // get the length of the string pattern and storing it in the integer m
    int *LPS = new int[m];    // create a new integer array of size m

    generateLPS(pattern, LPS);

    // print the LPS array for the pattern
    cout << "LPS of the pattern: ";
    for (int i = 0; i < m; i++)
    {
        cout << LPS[i] << " ";
    }
    cout << endl;

    KMP(text, pattern, LPS);

    delete[] LPS;

    return 0;
}