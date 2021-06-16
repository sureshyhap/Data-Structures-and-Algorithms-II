#include "hash.h"

Hash_Table::Hash_Table(int size) : capacity(get_prime(size)), filled(0), data(capacity) {
  
}

int Hash_Table::insert(const std::string& key, void* pv) {
  if ((filled + 1) > (capacity / 2)) {
    bool success = rehash(); ///////////////////////////
    if (!success) {
      return 2;
    }
  }
  int position = hash(key);
  while (data[position].is_occupied) {
    if (data[position].key == key and !data[position].is_deleted) {
      return 1;
    }
    ++position;
    position %= capacity;
  }
  Hash_Item& element = data[position];
  element.key = key;
  element.is_occupied = true;
  element.is_deleted = false;
  element.pv = pv;
  ++filled;
  return 0;
}

int Hash_Table::hash(const std::string& key) {
  int hash_val = 0;
  int length = key.length();
  for (int i = 0; i < length; ++i) {
    hash_val = hash_val * 37 + key[i];
  }
  hash_val %= capacity;
  if (hash_val < 0) {
    hash_val += capacity;
  }
  return hash_val;
}

int find_pos(const std::string& key) {
  //////////////
}

bool Hash_Table::rehash() {
  int old_cap = capacity;
  capacity = get_prime(capacity * 2);
  std::vector<Hash_Item> temp(std::move(data));
  data.reserve(capacity);
  for (int i = 0; i < old_cap; ++i) {
    if (temp[i].is_occupied and !temp[i].is_deleted) {
      insert(temp[i].key, temp[i].pv);
    }
  }
  return true;
}

unsigned int Hash_Table::get_prime(int size) {
  static const unsigned int[] primes = {6'217, 12'437, 24'889, 50'021, 100'043, 200'087, 400'187, 800'399, 1'600'811};
  int length = sizeof(primes) / sizeof(unsigned int);
  for (int i = 0; i < length; ++i) {
    if (primes[i] >= size) {
      return primes[i];
    }
  }
}
