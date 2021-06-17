#include "hash.h"

Hash_Table::Hash_Table(int size) : capacity(get_prime(size)), filled(0), data(capacity) {
  
}

int Hash_Table::insert(const std::string& key, void* pv) {
  // If Hash_Table becomes more than half full, resize
  if ((filled + 1) > (capacity / 2)) {
    bool success = rehash();
    if (!success) {
      return 2;
    }
  }
  int position = hash(key);
  while (data[position].is_occupied) {
    // If found key already
    if (data[position].key == key and !data[position].is_deleted) {
      return 1;
    }
    ++position;
    position %= capacity;
  }
  // Insert key if not found
  Hash_Item& element = data[position];
  element.key = key;
  element.is_occupied = true;
  element.is_deleted = false;
  element.pv = pv;
  ++filled;
  return 0;
}

bool Hash_Table::contains(const std::string& key) const {
  return (find_pos(key) != -1);
}

void* Hash_Table::get_pointer(const std::string& key, bool* b) {
  int position = find_pos(key);
  if (position == -1) {
    if (b) {
      *b = false;
    }
    return nullptr;
  }
  else {
    if (b) {
      *b = true;
    }
    return data[position].pv;
  }
}

int Hash_Table::set_pointer(const std::string& key, void* pv) {
  int position = find_pos(key);
  if (position == -1) {
    return 1;
  }
  else {
    data[position].pv = pv;
    return 0;
  }
}

bool Hash_Table::remove(const std::string& key) {
  int position = find_pos(key);
  if (position == -1) {
    return false;
  }
  else {
    data[position].is_deleted = true;
    return true;
  }
}

int Hash_Table::hash(const std::string& key) const {
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

int Hash_Table::find_pos(const std::string& key) const {
  int position = hash(key);
  while (data[position].is_occupied) {
    // Found a match
    if (data[position].key == key and !data[position].is_deleted) {
      return position;
    }
    ++position;
    position %= capacity;
  }
  return -1;
}

bool Hash_Table::rehash() {
  int old_cap = capacity;
  capacity = get_prime(capacity * 2);
  // Move data to temp vector
  std::vector<Hash_Item> temp(std::move(data));
  data.reserve(capacity);
  for (int i = 0; i < old_cap; ++i) {
    if (temp[i].is_occupied and !temp[i].is_deleted) {
      // Reinsert into newly resized data vector
      insert(temp[i].key, temp[i].pv);
    }
  }
  return true;
}

unsigned int Hash_Table::get_prime(int size) {
  static const std::vector<unsigned int> primes = {6'217, 12'437, 24'889, 50'021, 100'043, 200'087, 400'187, 800'399, 1'600'811};
  int length = primes.size();
  for (int i = 0; i < length; ++i) {
    if (primes[i] >= size) {
      return primes[i];
    }
  }
  return primes.back();
}
