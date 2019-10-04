#include "hash.h"

hashTable::hashTable(int size) {
  capacity = getPrime(size);
  filled = 0;
  data.resize(capacity);
}

int hashTable::insert(const std::string& key, void* pv) {
  // If adding a new element will cause it to be more than
  // half full, rehash
  if ((filled + 1) > (capacity / 2)) {
    bool success = rehash();
    // If rehashing failed return 2
    if (success == false) {
      return 2;
    }
  }
  int index = hash(key);
  // While the cell is occupied and not deleted, linearly probe for any empty cell
  while (data[index].isOccupied == true && data[index].isDeleted == false) {
    // If key is already in table return 1
    if (data[index].key == key) {
      return 1;
    }
    ++index;
    index %= capacity;
  }
  // Insert key and pv into available cell
  data[index].key = key;
  data[index].isOccupied = true;
  data[index].isDeleted = false;
  data[index].pv = pv;
  ++filled;
  return 0;
}

bool hashTable::contains(const std::string& key) const {
  int found = findPos(key);
  if (found == -1) {
    return false;
  }
  else {
    return true;
  }
}

void* hashTable::getPointer(const std::string& key, bool* b) {
  int index = findPos(key);
  // Key not found in table
  if (index == -1) {
    *b = false;
    return nullptr;
  }
  // Key found in table
  else {
    *b = true;
    return data[index].pv;
  }
}

int hashTable::setPointer(const std::string& key, void* pv) {
  int index = findPos(key);
  // Key not found in table
  if (index == -1) {
    return 1;
  }
  // Key found in table
  else {
    data[index].pv = pv;
    return 0;
  }
}

bool hashTable::remove(const std::string& key) {
  int index = findPos(key);
  // Key not found in table  
  if (index == -1) {
    return false;
  }
  // Key found in table
  else {
    data[index].isDeleted = true;
    return true;
  }
}

int hashTable::hash(const std::string& key) const {
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

int hashTable::findPos(const std::string& key) const {
  int index = hash(key);
  while (data[index].isOccupied == true) {
    if (data[index].key == key && data[index].isDeleted == false) {
      return index;
    }
    ++index;
    index %= capacity;
  }
  // Key is not in the table
  return -1;
}

bool hashTable::rehash() {
  // Make new capacity to be at least twice the old capacity
  int new_capacity = getPrime(capacity * 2);
  int old_capacity = capacity;
  // Copy of old data vector
  std::vector<hashItem> data2(data);
  data.clear();
  data.resize(new_capacity);
  filled = 0;
  capacity = new_capacity;
  for (int i = 0; i < old_capacity; ++i) {
    // If filled cell that has not been lazily deleted, insert it
    // into the newly resized data vector
    if (data2[i].isOccupied == true && data2[i].isDeleted == false) {
      insert(data2[i].key, data2[i].pv);
      ++filled;
    }
  }
  return true;
}

unsigned int hashTable::getPrime(int size) {
  static const std::vector<unsigned int> primes{24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739};
  int length = primes.size();
  for (int i = 0; i < length; ++i) {
    if (primes[i] >= size) {
      return primes[i];
    }
  }
}
