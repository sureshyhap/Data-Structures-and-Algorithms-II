#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <string>
#include "hash.h"

class heap {
 private:    
  class node {
  public:
    std::string id;
    int key;
    void* pData;
  };
  int capacity, size;
  std::vector<node> data;
  hashTable mapping;
  void percolateUp(int posCur);
  void percolateDown(int posCur);
  int getPos(node* pn);
 public:
  heap(int capacity);
  int insert(const std::string& id, int key, void* pv = nullptr);
  int setKey(const std::string& id, int key);
  int deleteMin(std::string* pId = nullptr, int* pKey = nullptr, void *ppData = nullptr);
  int remove(const std::string& id, int* pKey = nullptr, void* ppData = nullptr);
};

// HEAP_H
#endif
