#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

const int WORD_LENGTH = 10;

// Change to 1000 by 1000
bool possible[WORD_LENGTH][WORD_LENGTH] = {{0}};

void print_possible_table();

void populate_table(const std::vector<std::string>& words);

int main(int argc, char* argv[]) {
  
  std::cout << "Enter name of input file: ";
  std::string infile_name;
  std::cin >> infile_name;
  
  std::cout << "Enter name of output file: ";
  std::string outfile_name;
  std::cin >> outfile_name;
  
  std::ifstream infile(infile_name);
  std::ofstream outfile(outfile_name);

  if (!infile.is_open()) {
    std::cerr << "Could not open input file.\n";
    exit(1);
  }
  if (!outfile.is_open()) {
    std::cerr << "Could not open output file.\n";
    exit(1);
  }

  std::vector<std::string> words;
  words.resize(3);
  //  while (!infile.eof()) {
  {
    for (int i = 0; i < 3; ++i) {
      std::getline(infile, words[i]);
    }
    populate_table(words);
    //    std::string result = check_if_merge(words);
    //    outfile << result << '\n';
  }

  print_possible_table();

  infile.close();
  outfile.close();
  
  return 0;
}

void populate_table(const std::vector<std::string>& words) {

  const std::string& first = words[0];
  const std::string& second = words[1];
  const std::string& third = words[2];
  
  int length1 = first.length();
  int length2 = second.length();
  
  int rows = length1 + 1;
  int cols = length2 + 1;

  for (int overall_i = 0; overall_i < rows; ++overall_i) {
    for (int overall_j = 0; overall_j < cols; ++overall_j) {
      
      if ((overall_j + 1 < cols and !possible[overall_i][overall_j + 1]) or (overall_j + 1 == cols)) {
	for (int j = 1; overall_j + j < cols and (second[overall_j + j - 1] == third[overall_i + overall_j - 1 + j]); ++j) {
	  possible[overall_i][overall_j + j] = true;
	}
      }
      
      if ((overall_i + 1 < rows and !possible[overall_i + 1][overall_j]) or (overall_i + 1 == rows)) {
	for (int i = 1; overall_i + i < rows and (second[overall_i + i - 1] == third[overall_i + overall_j - 1 + i]); ++i) {
	  possible[overall_i + i][overall_j] = true;
	}
      }
      
    }
  }
  
}

void print_possible_table() {
  for (int i = 0; i < WORD_LENGTH; ++i) {
    std::cout << "__";
  }
  std::cout << "\n";
  for (int i = 0; i < WORD_LENGTH; ++i) {
    for (int j = 0; j < WORD_LENGTH; ++j) {
      std::cout << "|" << possible[i][j];
    }
    std::cout << "|\n";
  }
  for (int i = 0; i < WORD_LENGTH; ++i) {
    std::cout << "--";
  }
  std::cout << "\n";
}
