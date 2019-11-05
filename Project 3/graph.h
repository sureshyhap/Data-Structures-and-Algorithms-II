#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <climits>
#include <string>
#include "hash.h"
#include "heap.h"
#include <vector>

class Graph {
 public:
  Graph() = default;
  // Inserts edges (and possibly vertices) into graph
  void insert(const std::string& start, const std::string& destination, int cost);
  // Check if a start_vertex is in the graph
  bool check_start(const std::string& start_vertex);
  // Run Dijkstra's algorithm
  void dijkstra(const std::string start_vertex);
  // Retrieve the path to a given vertex
  std::vector<std::string> get_path(Vertex* v);
  // Output the shortest path and its cost for each vertex
  void display_results(std::ostream& os);
 private:
  struct Edge;
  struct Vertex {
    // List of edges directly connected to this vertex
    std::list<Edge*> adjacency_list;
    std::string name;
    // distance from source using known vertices (initially infinity)
    int dv = INT_MAX;
    // Previous node
    Vertex* pv = nullptr;
    // If it is certain that the distance from the source, dv, is optimal
    bool known = false;
  };
  struct Edge {
    // Name of a destination vertex
    std::string v_name;
    int cost;
  };
  // List of vertices that will have list of edges
  // which together define the graph
  std::list<Vertex*> vertices;
  // Will be used to efficiently find a vertex or
  // test whether a vertex is in the graph
  hashTable find_vertex;
};

#endif // GRAPH_H
