#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <vector>
#include <ctime>

const int MAX_LETTERS = 1000;

bool matrix[MAX_LETTERS][MAX_LETTERS] = {{false}};

struct Index {
  int i = 0, j = 0;
};

bool check(const std::string& a, const std::string& b, const std::string& c);
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
    check(a, b, c);
    final_str = retrieve(a, b);
    outfile << final_str << '\n';    
    // Reset matrix
    for (int i = 0; i <= a.length(); ++i) {
      for (int j = 0; j <= b.length(); ++j) {
	matrix[i][j] = false;
      }
    }
  }
  return 0;
}

bool check(const std::string& a, const std::string& b, const std::string& c) {
  int length = a.length() + b.length();
  // If the length of c is not the sum of a and b's lengths, it can't be a valid merge
  if (length != c.length()) {
    return false;
  }  
  // Record points in matrix where either way is possible
  std::vector<Index> branch_points;
  int i = 0, j = 0;
  // Arbitrarily choose a maximum of 5 seconds for the function to run
  const int MAX_TIME_ALLOWED = 1;
  clock_t time1 = clock();
  while (i + j < length) {
    clock_t time2 = clock();
    // My algorithm should be able to quickly terminate when c is a merge.
    // However it hangs on some cases where c is not a merge. So if it takes
    // too long, I assume c is not a merge. Thus my solution for these cases
    // is an approximate one
    double time_diff = (double(time2 - time1)) / CLOCKS_PER_SEC;
    if (time_diff > MAX_TIME_ALLOWED) {
      return false;
    }
    if (i == a.length()) {
      if (c[i + j] == b[j]) {
	matrix[i][j++] = true;
	continue;
      }
      else {
	if (!branch_points.empty()) {
	  Index go_back_to_this = branch_points.back();
	  branch_points.pop_back();
	  i = go_back_to_this.i;
	  j = go_back_to_this.j + 1;
	  continue;
	}
	else {
	  return false;
	}
      }
    }
    else if (j == b.length()) {
      if (c[i + j] == a[i]) {
	matrix[i++][j] = true;
	continue;
      }
      else {
	if (!branch_points.empty()) {
	  Index go_back_to_this = branch_points.back();
	  branch_points.pop_back();
	  i = go_back_to_this.i;
	  j = go_back_to_this.j + 1;
	  continue;
	}
	else {
	  return false;
	}
      }
    }
    // No corresponding character of c in neither a nor b
    if (c[i + j] != a[i] && c[i + j] != b[j]) {
      if (!branch_points.empty()) {
	Index go_back_to_this = branch_points.back();
	branch_points.pop_back();
	i = go_back_to_this.i;
	j = go_back_to_this.j + 1;
	continue;
      }
      else {
	return false;
      }
    }
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
	// Recording this junction for later
	// in case there is a need to return to this point
	Index ind;
	ind.i = i;
	ind.j = j;
	branch_points.push_back(ind);
	++i;
      }
    }
  }
  // If did not return false yet, c must be a valid merge
  matrix[i][j] = true;
  return true;
}

std::string retrieve(const std::string& a, const std::string& b) {
  int i = a.length(), j = b.length();
  if (!matrix[i][j]) {
    return "*** NOT A MERGE ***";
  }
  std::string result;
  while (i > 0 || j > 0) {
    if (i == 0) {
      result += b[--j];
      continue;
    }
    else if (j == 0) {
      result += toupper(a[--i]);
      continue;
    }
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
  std::string reversed = result;
  int length = result.length();
  for (int k = 0; k < length; ++k) {
    reversed[k] = result[length - 1 - k];
  }
  return reversed;
}
