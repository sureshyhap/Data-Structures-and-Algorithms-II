#include "graph.h"
#include <deque>
#include <sstream>
#include <iostream>

graph::edge::edge(const std::string& destination, int cost) : destination(destination), cost(cost) {
  
}

graph::vertex::vertex(const std::string& name) : name(name) {

}

graph::~graph() {
  for (std::list<vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    // Rename adjacency list for readability
    std::list<edge*>& edges = (*it)->adjacency_list;
    for (std::list<edge*>::iterator it2 = edges.begin(); it2 != edges.end(); ++it2) {
      delete *it2;
    }
    delete *it;
  }
}

void graph::insert(std::string source, std::string destination, int cost) {
  bool s_in_graph = map_to_graph.contains(source);
  // If source is not in graph, insert it into the graph and hash table
  if (!s_in_graph) {
    // Create a new vertex
    vertex* new_vertex = new vertex(source);
    // Add it to graph
    vertices.push_back(new_vertex);
    // Add it to hash table
    map_to_graph.insert(source, new_vertex);
  }
  bool d_in_graph = map_to_graph.contains(destination);
  // If destination is not in graph, insert it into the graph and hash table
  if (!d_in_graph) {
    // Create a new vertex
    vertex* new_vertex = new vertex(destination);
    // Add it to graph    
    vertices.push_back(new_vertex);
    // Add it to hash table    
    map_to_graph.insert(destination, new_vertex);
  }
  // Locate source vertex
  vertex* source_ptr = static_cast<vertex*>(map_to_graph.getPointer(source));
  // Create the edge corresponding to input parameters
  edge* pe = new edge(destination, cost);
  // Add edge to source vertex's adjacency list
  source_ptr->adjacency_list.push_back(pe);
}

bool graph::check_start(const std::string& starting_v) {
  return map_to_graph.contains(starting_v);
}

void graph::run_dijkstra(const std::string& source) {
  // Create priority queue to hold dvs of each vertex with respect to source
  heap minimum_dvs(vertices.size());
  // Insert each vertex's data into priority queue
  for (auto elem : vertices) {
    minimum_dvs.insert(elem->name, elem->dv, elem);
  }
  // dv to source is 0
  minimum_dvs.setKey(source, 0);
  // Already set the previous vertex for each vertex to nullptr
  while (true) {
    std::string id;
    int key;
    vertex* p_data;
    // Keep track of vertex with minimum dv
    bool status = minimum_dvs.deleteMin(&id, &key, &p_data);
    if (status == 1) {
      break;
    }
    if (key == INT_MAX) {
      continue;
    }
    // Vertex with minimum dv at this stage is certain to have minimum dv
    p_data->known = true;
    p_data->dv = key;
    // Rename adjacency list for readability
    std::list<edge*>& edges = p_data->adjacency_list;
    for (std::list<edge*>::iterator it = edges.begin(); it != edges.end(); ++it) {
      // Destination vertex
      vertex* w = static_cast<vertex*>(map_to_graph.getPointer((*it)->destination));
      if (w->known) {
	continue;
      }
      // If dv + c_vw < dw
      if ((key + (*it)->cost) < (w->dv)) {
	// dw = dv + c_vw
	(w->dv) = (key + (*it)->cost);
	minimum_dvs.setKey(w->name, w->dv);
	// pw = v
	w->pv = p_data;
      }
    }
  }
}

std::string graph::get_path(const std::string& v_str) {
  // Deque has efficient push_front implementation
  std::deque<std::string> path;
  // Get vertex corresponding to name v_str
  vertex* v = static_cast<vertex*>(map_to_graph.getPointer(v_str));
  // Trace back path through each pv
  while (v) {
    if (v->dv == INT_MAX) {
      break;
    }
    path.push_front(v->name);
    v = v->pv;
  }
  int size = path.size();
  // Path length = 0 means cannot be reached from source
  if (!size) {
    return "NO PATH";
  }
  // Display path
  std::ostringstream oss;
  oss << "[";
  for (int i = 0; i < size; ++i) {
    oss << path[i];
    if (i != size - 1) {
      oss << ", ";
    }
  }
  oss << "]";
  return oss.str();
}

void graph::display_results(std::ostream& ofs) {
  for (auto elem : vertices) {
    std::string path = get_path(elem->name);
    if (path == "NO PATH") {
      ofs << "NO PATH\n";
      continue;
    }
    ofs << elem->name << ": " << elem->dv << " ";
    ofs << path << '\n';
  }
}
