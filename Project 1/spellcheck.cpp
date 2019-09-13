#include <iostream>
#include "hash.h"
#include <fstream>
#include <ctime>
#include <cctype>

void load_dictionary(hashTable& dictionary);
std::string get_in();
std::string get_out();
void spellcheck(const hashTable& dictionary, std::ifstream& in, std::ofstream& out);

int main(int argc, char* argv[]) {
  // 50000 is the average dictionary size
  hashTable d(50000 * 2);
  load_dictionary(d);
  std::ifstream infile(get_in());
  std::ofstream outfile(get_out());
  spellcheck(d, infile, outfile);
  return 0;
}

void load_dictionary(hashTable& dictionary) {
  std::cout << "Enter name of dictionary: ";
  std::string dict;
  std::cin >> dict;
  std::string word;
  std::ifstream dict_file(dict);
  clock_t t1 = clock();
  while (std::getline(dict_file, word)) {
    int length = word.length();
    for (int i = 0; i < length; ++i) {
      word[i] = tolower(word[i]);
    }
    dictionary.insert(word);
  }
  clock_t t2 = clock();
  dict_file.close();
  double time_diff = (double(t2 - t1)) / CLOCKS_PER_SEC;
  std::cout << "Total time (in seconds) to load dictionary: " << time_diff << std::endl;
}

std::string get_in() {
  std::cout << "Enter name of input file: ";
  std::string input;
  std::cin >> input;
  return input;
}

std::string get_out() {
  std::cout << "Enter name of output file: ";
  std::string output;
  std::cin >> output;
  return output;
}

void spellcheck(const hashTable& dictionary, std::ifstream& in, std::ofstream& out) {
  // Keep track of the line numbers
  int line_num = 1;
  std::string line;
  clock_t t1 = clock();
  while (std::getline(in, line)) {
    int start = 0, end = 0;
    int length = line.length();
    std::string word;
    // Holds all the words in a line
    std::vector<std::string> words;
    int i = 0;
    for (; i < length; ++i) {
      // If the character is a word separator
      if (isalnum(line[i]) == false && line[i] != '-' && line[i] != '\'') {
	end = i;
	word = line.substr(start, end - start);
	// If there are any valid characters in word
	if (word.length() != 0) {
	  words.push_back(word);
	}
	start = i + 1;
      }
    }
    end = i;
    word = line.substr(start, end - start);
    if (word.length() != 0) {
      words.push_back(word);
    }
    // Go through all of the words in the line
    for (auto& element : words) {
      int len = element.length();
      for (int i = 0; i < len; ++i) {
	element[i] = tolower(element[i]);
      }
      // If word is too long, say so
      if (element.length() > 20) {
	out << "Long word at line " << line_num << ", starts: " << element.substr(0, 20) << std::endl;
	continue;
      }
      bool has_digit = false;
      for (int i = 0; i < len; ++i) {
	// If word includes a digit, do not spellcheck
	if (isdigit(element[i]) == true) {
	  has_digit = true;
	  break;
	}
      }
      if (has_digit == true) {
	continue;
      }
      // If is not too long and does not have digits, spellcheck.
      // If the word is not in the dictionary, it is unknown
      if (dictionary.contains(element) == false) {
	out << "Unknown word at line " << line_num << ": " << element << std::endl;
      }
      // If it is found, do nothing
    }
    words.clear();
    ++line_num;
  }
  clock_t t2 = clock();
  double time_diff = (double (t2 - t1)) / CLOCKS_PER_SEC;
  std::cout << "Total time (in seconds) to check document: " << time_diff << std::endl;
  in.close();
  out.close();
}
