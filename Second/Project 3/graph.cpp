#include "graph.h"
#include <deque>
#include <sstream>

graph::edge::edge(const std::string& destination, int cost) : destination(destination), cost(cost) {
  
}

graph::vertex::vertex(const std::string& name) : name(name) {

}

graph::~graph() {
  for (std::list<vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    for (std::list<edge*>::iterator it2 = ((*it)->adjacency_list).begin(); it2 != ((*it)->adjacency_list).end(); ++it2) {
      delete *it2;
    }
    delete *it;
  }
}

void graph::insert(std::string source, std::string destination, int cost) {
  bool s_in_graph = map_to_graph.contains(source);
  if (!s_in_graph) {
    vertex* new_vertex = new vertex(source);
    vertices.push_back(new_vertex);
    map_to_graph.insert(source, new_vertex);
  }
  bool d_in_graph = map_to_graph.contains(destination);
  if (!d_in_graph) {
    vertex* new_vertex = new vertex(destination);
    vertices.push_back(new_vertex);
    map_to_graph.insert(destination, new_vertex);
  }
  vertex* source_ptr = static_cast<vertex*>(map_to_graph.getPointer(source));
  edge* pe = new edge(destination, cost);
  source_ptr->adjacency_list.push_back(pe);
}

bool graph::check_start(const std::string& starting_v) {
  return map_to_graph.contains(starting_v) ? true : false;
}

void graph::run_dijkstra(const std::string& source) {
  heap minimum_dvs(vertices.size());
  for (auto elem : vertices) {
    minimum_dvs.insert(elem->name, elem->dv, elem);
  }
  minimum_dvs.setKey(source, 0);
  // Already set the previous vertex for each vertex to nullptr
  while (true) {
    std::string id;
    int key;
    vertex* p_data;
    bool status = minimum_dvs.deleteMin(&id, &key, &p_data);
    if (status == 1) {
      break;
    }
    p_data->known = true;
    for (std::list<edge*>::iterator it = (p_data->adjacency_list).begin(); it != (p_data->adjacency_list).end(); ++it) {
      vertex* w = static_cast<vertex*>(map_to_graph.getPointer((*it)->destination));
      if ((key + (*it)->cost) < (w->dv)) {
	(w->dv) = (key + (*it)->cost);
	w->pv = p_data;
      }
    }
  }
}

std::string graph::get_path(const std::string& v_str) {
  std::deque<std::string> path;
  vertex* v = static_cast<vertex*>(map_to_graph.getPointer(v_str));
  while (v) {
    path.push_front(v->name);
    v = v->pv;
  }
  int size = path.size();
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
    ofs << elem->name << ": " << elem->dv << " ";
    ofs << get_path(elem->name) << '\n';
  }
}
