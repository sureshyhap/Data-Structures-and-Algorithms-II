#include "graph.h"

Graph::~Graph() {
  for (std::list<Vertex*>::iterator vit = vertices.begin(); vit != vertices.end(); ++vit) {
    for (std::list<Edge*>::iterator eit = (*vit)->adjacency_list.begin();
	 eit != (*vit)->adjacency_list.end(); ++eit) {
      delete *eit;
    }
    delete *vit;
  }
}

void Graph::insert(const std::string& start, const std::string& destination, int cost) {
  // Check if vertex is in the graph yet
  bool start_is_in = find_vertex.contains(start);
  // If it's not in the graph, add it to the list of vertices
  if (!start_is_in) {
    Vertex* vs = new Vertex;
    vs->name = start;
    vertices.push_back(vs);
    std::list<Vertex*>::iterator new_vertex = vertices.end();
    --new_vertex;
    // Telling the hash table to point to Vertex*
    find_vertex.insert(start, *new_vertex);
  }
  // Same as above but checking if the destination vertex is in the graph yet
  bool destination_is_in = find_vertex.contains(destination);
  if (!destination_is_in) {
    Vertex* vd = new Vertex;
    vd->name = destination;
    vertices.push_back(vd);
    std::list<Vertex*>::iterator new_vertex = vertices.end();
    --new_vertex;
    find_vertex.insert(destination, *new_vertex);
  }
  // At this point both vertices are in the graph
  Vertex* start_vertex = static_cast<Vertex*>(find_vertex.getPointer(start));
  // Setting up an edge to add to the start vertex's adjacency list
  Edge* e = new Edge;
  e->v_name = destination;
  e->cost = cost;
  start_vertex->adjacency_list.push_back(e);
}

bool Graph::check_start(const std::string& start_vertex) {
  // If start vertex is in hash table, it is in the graph
  if (find_vertex.contains(start_vertex)) {
    return true;
  }
  else {
    return false;
  }
}

void Graph::dijkstra(const std::string start_vertex) {
  // Will keep track of the distances from the source.
  // Make the heap large enough to contain info about all vertices
  heap distances_from_source(vertices.size());
  // Add vertices to priority queue
  for (std::list<Vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    distances_from_source.insert((*it)->name, (*it)->dv, *it);
  }
  // At this point the priority is full with all vertices and the distances are infinity
  // Decrease key for source to 0
  distances_from_source.setKey(start_vertex, 0);
  while (true) {
    std::string id;
    int key;
    void* p_data;
    // Save information about deleted node
    int status = distances_from_source.deleteMin(&id, &key, &p_data);
    // If key == INT_MAX then there are no more paths to the rest of the nodes.
    // If status == 1 then there are no more nodes
    if (key == INT_MAX || status == 1) {
      break;
    }
    // The node with the smallest distance from the source
    Vertex* min = static_cast<Vertex*>(p_data);
    min->dv = key;
    min->known = true;
    // Traverse outgoing edges to the removed vertex
    for (std::list<Edge*>::iterator it = min->adjacency_list.begin();
	 it != min->adjacency_list.end(); ++it) {
      // Pointer to vertex that is adjacent to min
      Vertex* destination = static_cast<Vertex*>(find_vertex.getPointer((*it)->v_name));
      int& dw = destination->dv;
      Vertex*& pw = destination->pv;
      // Update rule
      if (min->dv + ((*it)->cost) < dw) {
	// If new path to destination is better, record it
	dw = min->dv + ((*it)->cost);
	pw = min;
	distances_from_source.setKey(((*it)->v_name), dw);
      }
    }
  }
}

std::vector<std::string> Graph::get_path(Vertex* v) {
  std::vector<std::string> reverse_path{v->name};
  while (v->pv != nullptr) {
    v = v->pv;
    reverse_path.push_back(v->name);
  }
  std::vector<std::string> path;
  for (std::vector<std::string>::reverse_iterator rit = reverse_path.rbegin();
       rit != reverse_path.rend(); ++rit) {
    path.push_back(*rit);
  }
  // Path from source to v
  return path;
}

void Graph::display_results(std::ostream& os) {
  for (std::list<Vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    os << ((*it)->name) << ": ";
    // No path was found to this vertex
    if ((*it)->dv == INT_MAX) {
      os << "NO PATH\n";
      continue;
    }
    os << ((*it)->dv) << " [";
    std::vector<std::string> path = get_path(*it);
    int size = path.size();
    for (int i = 0; i < size - 1; ++i) {
      os << path[i] << ", ";
    }
    os << path[size - 1] << "]" << "\n";
  }
}
