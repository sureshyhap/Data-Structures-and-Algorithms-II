#include <iostream>
#include "hash.h"
#include <string>
#include <fstream>
#include <ctime>
#include <cctype>

// Loads dictionary into Hash_Table
void load(Hash_Table& h, const std::string& dictionary);
// Finds valid words in document and spellchecks them
void process_doc(const Hash_Table& h, const std::string& input, const std::string& output);
// Makes a string lowercase
void make_lower(std::string& s);
// Spellchecks a word
void spellcheck(const Hash_Table& h, const std::string& word, int line_num, std::ofstream& out);

int main(int argc, char* argv[]) {
  std::cout << "Enter name of dictionary: ";
  std::string dict;
  std::cin >> dict;
  Hash_Table h(5'000);
  clock_t t1 = clock();
  load(h, dict);
  clock_t t2 = clock();
  // Elapsed is the time duration that load() took
  double elapsed = static_cast<double>(t2 - t1) / CLOCKS_PER_SEC;
  std::cout << "Total time (in seconds) to load dictionary: " << elapsed << '\n';
  std::cout << "Enter name of input file: ";
  std::string in;
  std::cin >> in;
  std::cout << "Enter name of output file: ";
  std::string out;
  std::cin >> out;
  t1 = clock();
  process_doc(h, in, out);
  t2 = clock();
  elapsed = static_cast<double>(t2 - t1) / CLOCKS_PER_SEC;
  std::cout << "Total time (in seconds) to check document: " << elapsed << '\n';
  return 0;
}

void load(Hash_Table& h, const std::string& dictionary) {
  std::ifstream infile(dictionary);
  std::string word;
  while(std::getline(infile, word)) {
    h.insert(word);
  }
  infile.close();
}

void process_doc(const Hash_Table& h, const std::string& input, const std::string& output) {
  std::ifstream infile(input);
  std::ofstream outfile(output);
  std::string line;
  std::vector<std::string> words;
  int line_num = 1;
  // Processes each line
  while (std::getline(infile, line)) {
    make_lower(line);
    int i = 0, j = 0;
    int length = line.length();
    while (true) {
      if ((j < length) and (isalnum(line[j]) or line[j] == '-' or line[j] == '\'')) {
	++j;
	continue;
      }
      // Substring is a valid word
      std::string w = line.substr(i, j - i);
      if (!w.empty()) {
	words.push_back(w);
      }
      j = i = j + 1;
      // Done with extracting words from line
      if (j >= length) {
	for (auto const& word : words) {
	  // Spellcheck each valid word
	  spellcheck(h, word, line_num, outfile);
	}
	++line_num;
	break;
      }
    }
    words.clear();
  }
  infile.close();
  outfile.close();
}

void make_lower(std::string& s) {
    int length = s.length();
    for (int i = 0; i < length; ++i) {
      s[i] = tolower(s[i]);
    }  
}

void spellcheck(const Hash_Table& h, const std::string& word, int line_num, std::ofstream& out) {
  if (word.length() > 20) {
    out << "Long word at line " << line_num << ", starts: " + word.substr(0, 20) << '\n';
    return;
  }
  int length = word.length();
  for (int i = 0; i < length; ++i) {
    if (isdigit(word[i])) {
      return;
    }
  }
  if (!h.contains(word)) {
    out << "Unknown word at line " << line_num << ": " << word << '\n';
  }
}
