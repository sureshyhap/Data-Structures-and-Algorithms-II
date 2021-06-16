#include <iostream>
#include "hash.h"
#include <string>
#include <fstream>
#include <ctime>

void load(Hash_Table& h, std::string dictionary);

int main(int argc, char* argv[]) {
  std::cout << "Enter name of dictionary: ";
  std::string dict;
  std::cin >> dict;
  Hash_Table h(5'000);
  clock_t t1 = clock();
  load(h, dict);
  clock_t t2 = clock();
  double elapsed = static_cast<double>(t2 - t1) / CLOCKS_PER_SEC;
  return 0;
}

void load(Hash_Table& h, std::string dictionary) {
  std::ifstream infile(dictionary);
  std::string word;
  while(std::getline(infile, word)) {
    
  }
}
