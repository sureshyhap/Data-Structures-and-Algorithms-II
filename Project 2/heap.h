#ifndef _HEAP_H
#define _HEAP_H

#include "hash.h"
#include <vector>
#include <string>

class heap {
 public:
  heap(int capacity);
  int insert(const std::string& id, int key, void* pv = nullptr);
  int setKey(const std::string& id, int key);
  int deleteMin(std::string* p_id = nullptr, int* p_key = nullptr, void* pp_data = nullptr);
  int remove(const std::string& id, int* p_key = nullptr, void* pp_data = nullptr);
 private:
  class heap_node {
  public:
    std::string id;
    int key;
    void* p_data;
  };
  void percolate_up(int current_pos);
  void percolate_down(int current_pos);
  int get_pos(heap_node* phn);
  int size = 0, capacity = 0;
  std::vector<heap_node> data;
  hashTable str_map;
};

#endif // _HEAP_H
