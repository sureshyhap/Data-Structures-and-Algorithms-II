#include "heap.h"

heap::heap(int capacity):str_map(capacity * 2) {
  this->capacity = capacity;
  // Not using 0th element so allocate capaity + 1
  // instead of just capacity
  data.resize(capacity + 1);
}

int heap::insert(const std::string& id, int key, void* pv) {
  // If will overflow if insertion takes place
  // don't allow it
  if (size + 1 > capacity) {
    return 1;
  }
  // If id is already in hash table and thus
  // is in the heap already
  if (str_map.contains(id)) {
    return 2;
  }
  // Set the new last node's fields
  data[++size].id = id;
  data[size].key = key;
  data[size].p_data = pv;
  // Insert unique id into hash table
  // and let it point to this new item
  str_map.insert(id, &(data[size]));
  // Percolate up to correct spot
  // considering the heap order property
  percolate_up(size);
  return 0;
}

int heap::setKey(const std::string& id, int key) {
  // Locate node with given id
  bool test = false;
  heap_node* phn = static_cast<heap_node*>(str_map.getPointer(id, &test));
  if (phn == nullptr) {
    return 1;
  }
  int pos = get_pos(phn);
  // Increase key
  if (key > data[pos].key) {
    data[pos].key = key;
    percolate_down(pos);
  }
  // Decrease key
  else if (key < data[pos].key) {
    data[pos].key = key;
    percolate_up(pos);
  }
  return 0;
}

int heap::deleteMin(std::string* p_id, int* p_key, void* pp_data) {
  // If heap is empty, can't delete
  if (size == 0) {
    return 1;
  }
  // Store the minimum node's data
  if (p_id != nullptr) {
    *p_id = data[1].id;
  }
  if (p_key != nullptr) {
    *p_key = data[1].key;
  }
  if (pp_data != nullptr) {
    *(static_cast<void **>(pp_data)) = data[1].p_data;
  }
  // Remove from hash table
  str_map.remove(data[1].id);
  // Overwrite root with last element
  data[1].id = data[size].id;
  data[1].key = data[size].key;
  data[1].p_data = data[size].p_data;
  // Tell hash item corresponding to previous last element
  // to now point to root
  str_map.setPointer(data[size].id, &(data[1]));
  --size;
  percolate_down(1);
  return 0;
}

int heap::remove(const std::string& id, int* p_key, void* pp_data) {
  // Locate node with given id
  bool test = false;
  heap_node* phn = static_cast<heap_node*>(str_map.getPointer(id, &test));
  if (phn == nullptr) {
    return 1;
  }
  // Store its values
  if (p_key != nullptr) {
    *p_key = phn->key;
  }
  if (pp_data != nullptr) {
    *(static_cast<void**>(pp_data)) = phn->p_data;
  }
  // Change to be less than the minimum and then call deleteMin
  setKey(id, data[1].key - 1);
  deleteMin();
  return 0;
}

void heap::percolate_up(int current_pos) {
  // Uses hole method so store original values
  std::string id_value = data[current_pos].id;
  int key_value = data[current_pos].key;
  void* p_data_value = data[current_pos].p_data;
  while (true) {
    // Current position is at the root
    if (current_pos == 1) {
      break;
    }
    // If not root, has a parent
    int parent_index = current_pos / 2;
    // If key belongs above parent
    if (key_value < data[parent_index].key) {
      // Move parent values down
      data[current_pos].id = data[parent_index].id;
      data[current_pos].key = data[parent_index].key;
      data[current_pos].p_data = data[parent_index].p_data;
      // Update pointer in hash table
      str_map.setPointer(data[parent_index].id, &(data[current_pos]));
      current_pos = parent_index;
    }
    // If key is in correct position
    else {
      break;
    }
  }
  // At this point the original node is in the correct place
  data[current_pos].id = id_value;
  data[current_pos].key = key_value;
  data[current_pos].p_data = p_data_value;
  str_map.setPointer(data[current_pos].id, &(data[current_pos]));
}

void heap::percolate_down(int current_pos) {
  // Uses the hole method
  std::string id_value = data[current_pos].id;
  int key_value = data[current_pos].key;
  void* p_data_value = data[current_pos].p_data;
  while (true) {
    // If node doesn't have any children
    // can't percolate down
    if (current_pos * 2 > size) {
      break;
    }
    // Has no right child (but has a left child)
    else if (current_pos * 2 + 1 > size) {
      // Left child index
      int child_index = current_pos * 2;
      // If belongs below its child
      if (key_value > data[child_index].key) {
	// Move up child values
	data[current_pos].id = data[child_index].id;
	data[current_pos].key = data[child_index].key;
	data[current_pos].p_data = data[child_index].p_data;
	// Change the hash table pointer to point to new spot
	str_map.setPointer(data[child_index].id, &(data[current_pos]));
	current_pos = child_index;
	// This case can only happen at the bottom, so must be done
	// percolating down
	break;
      }
    }
    // Has two children
    else {
      // Figure out which child is smaller
      int smaller_child_index = data[current_pos * 2].key < data[current_pos * 2 + 1].key ? current_pos * 2 : current_pos * 2 + 1;
      // If belongs below its smaller child
      if (key_value > data[smaller_child_index].key) {
	// Move up smaller child values
	data[current_pos].id = data[smaller_child_index].id;
	data[current_pos].key = data[smaller_child_index].key;
	data[current_pos].p_data = data[smaller_child_index].p_data;
	// Change the hash table pointer to point to new spot
	str_map.setPointer(data[smaller_child_index].id, &(data[current_pos]));
	current_pos = smaller_child_index;
      }
    }
  }
  data[current_pos].id = id_value;
  data[current_pos].key = key_value;
  data[current_pos].p_data = p_data_value;
  str_map.setPointer(data[current_pos].id, &(data[current_pos]));
}

int heap::get_pos(heap_node* phn) {
  // Okay because only will call this if phn is not nullptr
  return (phn - &(data[0]));
}
