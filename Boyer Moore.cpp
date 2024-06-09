#include <iostream>
#include <string>

using namespace std;

const int numChar = 256;

void badCharHeuristic(string str, int size, int badchar[numChar])
{
    // bad character heuristic
    int i;

    for (i = 0; i < numChar; i++)
        badchar[i] = -1;

    for (i = 0; i < size; i++)
        badchar[(int)str[i]] = i;
}

void suffixes(string pattern, int patternSize, int *suff)
{
    // determine how far the pattern can be shifted when a mismatch occurs in GOOD SUFFIX HEURISTIC
    int f, g, i;

    suff[patternSize - 1] = patternSize;
    g = patternSize - 1;
    f = 0;
    for (i = patternSize - 2; i >= 0; --i)
    {
        if (i > g && suff[i + patternSize - 1 - f] < i - g)
            suff[i] = suff[i + patternSize - 1 - f];
        else
        {
            if (i < g)
                g = i;
            f = i;
            while (g >= 0 && pattern[g] == pattern[g + patternSize - 1 - f])
                --g;
            suff[i] = f - g;
        }
    }
}

void goodSuffixHeuristic(string pattern, int patternSize, int *goodSuffix)
{
    int i, suff[patternSize];

    suffixes(pattern, patternSize, suff);

    for (i = 0; i < patternSize; ++i)
        goodSuffix[i] = patternSize;

    for (i = patternSize - 1; i >= 0; --i)
        if (suff[i] == i + 1)
            for (int j = 0; j < patternSize - 1 - i; ++j)
                if (goodSuffix[j] == patternSize)
                    goodSuffix[j] = patternSize - 1 - i;

    for (i = 0; i <= patternSize - 2; ++i)
        goodSuffix[patternSize - 1 - suff[i]] = patternSize - 1 - i;
}
void search(string text, string pattern)
{
    int textLen = text.length();
    int patternLen = pattern.length();

    int badchar[numChar];
    badCharHeuristic(pattern, patternLen, badchar);

    int goodSuffix[patternLen];
    goodSuffixHeuristic(pattern, patternLen, goodSuffix);

    int s = 0;
    int badCharStride = 0;
    int goodSuffixStride = 0;
    bool found = false;

    // iterates over the string from left to right
    while (s <= (textLen - patternLen))
    {
        int j = patternLen - 1;

        // compares the pattern with the current window in the text from right to left.
        while (j >= 0 && pattern[j] == text[s + j])
            j--;

        if (j < 0)
        {
            cout << "\nPattern occurs at shift = " << s << endl;
            s += (s + patternLen < textLen) ? patternLen - badchar[text[s + patternLen]] : 1;
            found = true;
        }
        else
        {
            /*
            if mismatched is not in pattern, j - badchar[text[s + j]] will be negative,
            so max(1, j - badchar[text[s + j]]) ensures the shift is at least 1.
            */
            int badCharShift = max(1, j - badchar[text[s + j]]);

            // get shift by good suffix heuristic
            int goodSuffixShift = goodSuffix[j];

            // get the maximum shift given by both heuristics
            int shift = max(badCharShift, goodSuffixShift);

            if (shift == badCharShift)
                badCharStride++;
            if (shift == goodSuffixShift)
                goodSuffixStride++;

            s += shift;

            cout << "\nIteration: " << s << "\nBad Heuristic Stride: " << badCharStride << "\nGood Heuristic Stride: " << goodSuffixStride << endl;
        }
    }
    // print if the pattern is not in the text
    if (!found)
    {
        cout << "Pattern is not in the text!" << endl;
    }
}

int main()
{
    string str;
    string pattern;
    cout << "Boyer-Moore algorithm\n";
    cout << "Enter a string: ";
    getline(cin, str);
    cout << "Enter pattern: ";
    getline(cin, pattern);
    if (str.length() < pattern.length())
    {
        cout << "The pattern is bigger than the string are you dumb???";
        return 0;
    }
    search(str, pattern);
    return 0;
}