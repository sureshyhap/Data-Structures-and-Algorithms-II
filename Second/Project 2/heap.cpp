#include "heap.h"

heap::heap(int capacity) : capacity(capacity + 1), size(0), mapping(capacity * 2) {
  data.resize(this->capacity);
}

int heap::insert(const std::string& id, int key, void* pv) {
  // Filled already
  if (size == capacity) {
    return 1;
  }
  // Id is already in hash table and heap
  if (mapping.contains(id)) {
    return 2;
  }
  // Insert at the end of the heap
  data[++size] = {id, key, pv};
  // Insert into the hash table
  mapping.insert(id, &data[size]);
  // Fix its spot
  percolateUp(size);
  return 0;
}

int heap::setKey(const std::string& id, int key) {
  bool b = false;
  // Get pointer to node from hash table
  node* pn = static_cast<node*>(mapping.getPointer(id, &b));
  // Not in hash table or heap
  if (!b) {
    return 1;
  }
  int pos = getPos(pn);
  // New key is smaller so possibly percolate up
  if (key < data[pos].key) {
    data[pos].key = key;
    percolateUp(pos);
  }
  // New key is bigger so possibly percolate down
  if (key > data[pos].key) {
    data[pos].key = key;
    percolateDown(pos);
  }
  return 0;
}

int heap::deleteMin(std::string* pId, int* pKey, void *ppData) {
  // Empty heap
  if (!size) {
    return 1;
  }
  // Save the deleted minimum's data
  if (pId) {
    *pId = data[1].id;
  }
  if (pKey) {
    *pKey = data[1].key;
  }
  if (ppData) {
    *(static_cast<void**>(ppData)) = data[1].pData;
  }
  // Remove from hash table
  mapping.remove(data[1].id);
  // Replace root with last element
  data[1] = data[size--];
  // Change pointer to last element to point to root
  mapping.setPointer(data[1].id, &data[1]);
  // Fix its spot
  percolateDown(1);
  return 0;
}

int heap::remove(const std::string& id, int* pKey, void* ppData) {
  // Id is not in hash table or heap
  if (!mapping.contains(id)) {
    return 1;
  }
  bool b = {};
  node* pn = static_cast<node*>(mapping.getPointer(id, &b));
  if (pKey) {
    *pKey = pn->key;
  }
  if (ppData) {
    *(static_cast<void**>(ppData)) = pn->pData;
  }
  // Beware if minimum is the minimum integer possible /////////////////  
  setKey(id, data[1].key - 1);
  deleteMin();
  return 0;
}

void heap::percolateUp(int posCur) {
  node temp = data[posCur];
  // While not the root
  while (posCur > 1) {
    int parent = posCur / 2;
    // Node belongs above its parent    
    if (temp.key < data[parent].key) {
      data[posCur] = data[parent];
      mapping.setPointer(data[posCur].id, &data[posCur]);
    }
    else {
      break;
    }
    posCur /= 2;
  }
  data[posCur] = temp;
  mapping.setPointer(data[posCur].id, &data[posCur]);
}

void heap::percolateDown(int posCur) {
  node temp = data[posCur];
  // Has no children in range
  while (posCur * 2 <= size) {
    int left_child = 2 * posCur, right_child = 2 * posCur + 1;
    int smaller = {};
    if (right_child > size) {
      smaller = left_child;
    }
    else {
      // Will need to swap with the smaller of the children
      smaller = data[left_child].key < data[right_child].key ? left_child : right_child;
    }
    // If bigger than one of the children move it down
    if (temp.key > data[smaller].key) {
      data[posCur] = data[smaller];
      mapping.setPointer(data[posCur].id, &data[posCur]);
    }
    else {
      break;
    }
    if (smaller == left_child) {
      posCur *= 2;
    }
    else {
      posCur = posCur * 2 + 1;
    }
  }
  data[posCur] = temp;
  mapping.setPointer(data[posCur].id, &data[posCur]);
}

int heap::getPos(node* pn) {
  return pn - &data[0];
}
