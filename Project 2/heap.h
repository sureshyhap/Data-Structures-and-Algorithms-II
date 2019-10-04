#ifndef _HEAP_H
#define _HEAP_H

#include "hash.h"
#include <vector>
#include <string>

class heap {
 public:
  heap(int capacity);
  int insert(const std::string& id, int key, void* pv = nullptr);
  /////////  int set_key(const std::string& id, int key);
  int delete_min(std::string* p_id, int* p_key, void* pp_data = nullptr);
  //////////// remove
 private:
  class heap_node {
  public:
    std::string id;
    int key;
    void* p_data;
  };
  void percolate_up(int current_pos);
  void percolate_down(int current_pos);
  ////////////////////
  int size = 0, capacity = 0;
  std::vector<heap_node> data;
  hashTable str_map;
};

#endif // _HEAP_H
