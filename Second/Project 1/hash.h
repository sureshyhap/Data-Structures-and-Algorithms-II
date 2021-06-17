#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>

class Hash_Table {
 public:
  // Initializes the Hash_Table with default empty capacity
  // or prime number capacity
  Hash_Table(int size = 0);
  // Inserts key into Hash_Table and associates a pointer
  // with it if passed in
  // Returns 0 on success
  // Returns 1 if key is already in Hash_Table
  // Returns 2 if rehash fails
  int insert(const std::string& key, void* pv = nullptr);
  // Returns true if key is in Hash_Table; false if not
  bool contains(const std::string& key) const;
  // Returns pointer associated with key
  // Returns nullptr if key is not in Hash_Table
  // bool* serves to differentiate between cases where
  // key is not in table vs. key is in table but pointer
  // associated with it is nullptr
  void* get_pointer(const std::string& key, bool* b = nullptr);
  // Sets pointer associated with key to pv
  // Returns 0 on success
  // Returns 1 if key is not in Hash_Table
  int set_pointer(const std::string& key, void* pv);
  // Lazily deletes Hash_Item with given key
  bool remove(const std::string& key);
 private:
  // Each element of the Hash_Table
  class Hash_Item {
  public:
    Hash_Item() = default;
    // Primary data of each Hash_Item is its key
    std::string key = "";
    // True if a key was inserted into this Hash_Item
    bool is_occupied = false;
    // True if element was lazily deleted
    bool is_deleted = false;
    // Outgoing pointer to be used in next assignments
    void* pv = nullptr;
  };
  // Maps key to index in Hash_Table
  int hash(const std::string& key) const;
  // Returns position of key in Hash_Table
  // Returns -1 if key is not in Hash_Table
  int find_pos(const std::string& key) const;
  // Resizes the Hash_Table to accomodate more data
  // Returns true if allocation succeeded; false otherwise
  bool rehash();
  // Returns a prime number >= size
  static unsigned int get_prime(int size);
  // Current max capacity of Hash_Table
  int capacity;
  // Current number of elements in Hash_Table
  int filled;
  // Actual entries are in here
  std::vector<Hash_Item> data;
};

#endif // HASH_H
