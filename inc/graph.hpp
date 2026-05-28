#pragma once
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

// using Vertex = unsigned int;
struct Vertex{
  static unsigned int count;
  Vertex();
  bool operator==(const Vertex& other);
  bool operator==(const unsigned int& id);
  unsigned int id;
  unsigned int open;
  unsigned int closed;
};
// bool operator<(Vertex first, Vertex second);

class Graph {
public:
  Graph();
  Graph(Vertex v);
  ~Graph();
  bool adjacent(Vertex, Vertex);
  std::vector<double> neighbours(Vertex) const;
  void add_vertex(Vertex);
  bool remove_vertex(Vertex);
  bool add_edge(Vertex, Vertex, double);
  bool remove_edge(Vertex, Vertex);
  Vertex get_vertex(unsigned int) const;
  void set_vertex(Vertex, unsigned int);
  double get_edge(Vertex, Vertex) const;
  void set_edge(Vertex, Vertex, double);
  std::vector<std::vector<double>> _adjMat;
  std::vector<Vertex> _vertices;
};

std::ostream &operator<<(std::ostream &os, const Graph &g);
std::ostream &operator<<(std::ostream &os, const std::vector<Vertex> &v);
std::ostream &operator<<(std::ostream &os, const std::vector<double> &v);