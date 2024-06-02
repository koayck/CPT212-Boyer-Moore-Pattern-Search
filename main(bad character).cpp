#include <iostream>
#include <string>

// BOYER MOORE ALGORITHM BAD CHARACTER HEURISTIC

using namespace std;

int const numChar = 256;

int max(int a, int b)
{
	return (a > b) ? a : b;
}

void charTable(string pattern, int pattern_len, int badchar[numChar])
{
	// initialize all occurrences as -1
	for (int i = 0; i < numChar; i++)
	{
		badchar[i] = -1;
	}

	cout << "Character Table for the pattern: [ ";

	for (int i = 0; i < pattern_len; i++)
	{
		badchar[(int)pattern[i]] = pattern_len - 1 - i;
	}
	for (int i = 0; i < pattern_len; i++)
	{
		cout << badchar[(int)pattern[i]] << " ";
	}
	cout << "]" << endl;
}

void search(string str, string pattern)
{
	int string_len = str.length();
	int pattern_len = pattern.length();
	int badchar[numChar];
	charTable(pattern, pattern_len, badchar);

	int index = 0;
	while (index <= (string_len - pattern_len))
	{

		int j = pattern_len - 1;
		while (j >= 0 && pattern[j] == str[index + j])
		{
			j--;
		}
		if (j < 0)
		{
			cout << "The image starts at index: " << index << endl;
			int temp;
			if (index + pattern_len < string_len)
			{
				temp = pattern_len - badchar[str[index + pattern_len]];
			}
			else
			{
				temp = 1;
			}
			index = index + temp;
		}
		else
		{
			index = index + max(1, j - badchar[str[index + j]]);
		}
	}
}

int main()
{
	string str;
	string image;
	cout << "Boyer-Moore-Horspool algorithm (Bad Character Heuristic)\n";
	cout << "Enter a string: ";
	getline(cin, str);
	cout << "Enter pattern: ";
	getline(cin, image);
	if (str.length() < image.length())
	{
		cout << "The image is bigger than the string";
		return 0;
	}

	search(str, image);
	return 0;
}