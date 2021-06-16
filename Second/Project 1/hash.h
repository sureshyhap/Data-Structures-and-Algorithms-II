#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>

class Hash_Table {
 public:
  Hash_Table(int size = 0);
  int insert(const std::string& key, void* pv = nullptr);
 private:
  class Hash_Item {
  public:
    Hash_Item() = default;
    std::string key = "";
    bool is_occupied = false;
    bool is_deleted = false;
    void* pv = nullptr;
  };
  int hash(const std::string& key);
  int find_pos(const std::string& key);
  bool rehash();
  static unsigned int get_prime(int size);
  int capacity;
  int filled;
  std::vector<Hash_Item> data;
};

#endif // HASH_H
