#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <string>
#include <list>
#include <climits>
#include "hash.h"
#include "heap.h"

class graph {
 private:
  class edge {
  public:
    std::string destination;
    int cost = {};
    edge() = default;
    edge(const std::string& destination, int cost);
  };
  class vertex {
  public:
    std::string name;
    int dv = INT_MAX;
    vertex* pv = nullptr;
    bool known = false;
    std::list<edge*> adjacency_list;
    vertex() = default;
    vertex(const std::string& name);
  };
  std::list<vertex*> vertices;
  hashTable map_to_graph;
 public:
  graph() = default;
  ~graph();
  void insert(std::string source, std::string destination, int cost);
  bool check_start(const std::string& starting_v);
  void run_dijkstra(const std::string& source);
  std::string get_path(const std::string& vertex);
  void display_results(std::ostream& ofs);
};

// _GRAPH_H_
#endif
