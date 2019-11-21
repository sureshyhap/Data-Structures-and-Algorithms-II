#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

const int max_letters = 1000;

bool matrix[max_letters] = {{false}};

bool check(const std::string& a, const std::string& b, const std::string& c, int i, int j);
std::string retrieve(const std::string& a, const std::string& b);

int main(int argc, char* argv[]) {
  std::string input, output;
  std::cout << "Enter name of input file: ";
  std::cin >> input;
  std::ifstream infile(input);
  std::cout << "Enter name of output file: ";
  std::cin >> output;
  std::ofstream outfile(output);
  std::string a, b, c, final_str;
  while (std::getline(infile, a)) {
    std::getline(infile, b);
    std::getline(infile, c);
    if (check(a, b, c, 0, 0)) {
      final_str = retrieve(a.length(), b.length());
      outfile << final_str << '\n';
    }
    else {
      outfile << "*** NOT A MERGE ***\n";
    }
    // Reset matrix
    for (int i = 0; i <= a.length(); ++i) {
      for (int j = 0; j <= b.length(); ++j) {
	matrix[i][j] = false;
      }
    }
  }
  return 0;
}

bool check(const std::string& a, const std::string& b, const std::string& c, int i, int j) {
  int length = a.length() + b.length();
  // If the length of c is not the sum of a and b's lengths, it can't be a valid merge
  if (length != c.length()) {
    return false;
  }
  while (i + j < length) { //////////////////////////////////// Chek if < or <= /////////////////////
    // No corresponding character of c in either a nor b
    if (c[i + j] != a[i] && c[i + j] != b[j]) {
      return false;
    }
    ////////////////////////////////////////////// Make sure not at the end of one of the strings
    // There is at least one corresponding character in a or b
    else {
      // Matching character in a only
      if (c[i + j] == a[i] && c[i + j] != b[j]) {
	matrix[i++][j] = true;
      }
      // Matching character in b only
      else if (c[i + j] != a[i] && c[i + j] == b[j]) {
	matrix[i][j++] = true;
      }
      // Matching character in both a and b
      else {
	matrix[i][j] = true;
	check(a, b, c, i + 1, j);
	check(a, b, c, i, j + 1);
      }
    }
  }
  // If did not return false yet, c must be a valid merge
  return true;
}

std::string retrieve(const std::string& a, const std::string& b) {
  int i = a.length(), j = b.length();
  std::string result;
  ////////////////////////////////////////////// Make sure not at the end of one of the strings
  while (i > 0 || j > 0) {
    // Came here from a letter from a (along the rows)
    if (matrix[i - 1][j] && !matrix[i][j - 1]) {
      result += toupper(a[--i]);
    }
    // Came here from a letter from b (along the columns)
    else if (!matrix[i - 1][j] && matrix[i][j - 1]) {
      result += b[--j];
    }
    // Came here from both a letter from a and b so choose
    // from b (so that the letter from a will be in front
    // when the string is reversed)
    else if (matrix[i - 1][j] && matrix[i][j - 1]) {
      result += b[--j];
    }
  }
  std::string reversed;
  int length = result.length();
  for (int k = 0; k < length; ++k) {
    reversed[k] = result[length - 1 - k];
  }
  return reversed;
}
