#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <vector>

const int max_letters = 1000;

bool matrix[max_letters][max_letters] = {{false}};

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

    ///////////////////////////////////

    
    for (int i = 0; i <= a.length(); ++i) {
      for (int j = 0; j <= b.length(); ++j) {
	std::cout << matrix[i][j];
      }
      std::cout << std::endl;
    }


    ///////////////////////////////////


    
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

  //////////////////////
  /*
  int smaller = a.length() < b.length() ? a.length() : b.length();
  // If the strings start the same, either path is ok so choose from a
  while (i < smaller && a[i] == b[i]) {
    matrix[i++][j] = true;
  }
  while (i < j) {
    matrix[i][j++] = true;
  }
  */
  ///////////////////////
  while (i + j < length) {
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
    ////////////////     for (int k = )
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
	Index ind;
	ind.i = i;
	ind.j = j;
	branch_points.push_back(ind);
	++i;

	
	////////////////////////////////// Might run out of stack space
	/*
	bool first_passes, second_passes;
	first_passes = check(a, b, c, i + 1, j);
	if (!first_passes) {
	  second_passes = check(a, b, c, i, j + 1);
	  if (!second_passes) {
	    return false;
	  }
	  else {
	    return true;
	  }
	}
	else {
	  return true;
	}
	*/
	/*
	if (!first_passes && !second_passes) {
	  return false;
	}
	else {
	  return true;
	}
	*/
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
