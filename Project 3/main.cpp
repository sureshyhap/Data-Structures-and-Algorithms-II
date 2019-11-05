#include <iostream>
#include "graph.h"
#include <fstream>
#include <string>
#include <sstream>

int main(int argc, char* argv[]) {
  Graph g;
  std::string file_name;
  std::cout << "Enter name of graph file: ";
  std::cin >> file_name;
  std::ifstream infile(file_name);
  std::string edge;
  while (std::getline(infile, edge)) {
    std::istringstream iss(edge);
    std::string vertex1, vertex2;
    int cost;
    iss >> vertex1 >> vertex2 >> cost;
    g.insert(vertex1, vertex2, cost);
  }
  std::string start_vertex_dijkstra;
  do {
    std::cout << "Enter a valid vertex id for the starting vertex: ";
    std::cin >> start_vertex_dijkstra;
  } while(!g.check_start(start_vertex_dijkstra));
  clock_t time1 = clock();
  g.dijkstra(start_vertex_dijkstra);
  clock_t time2 = clock();
  double time_diff = (double(time2 - time1)) / CLOCKS_PER_SEC;
  std::cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << time_diff << "\n";
  std::string out;
  std::cout << "Enter name of output file: ";
  std::cin >> out;
  std::ofstream outfile(out);
  g.display_results(outfile);
  return 0;
}
