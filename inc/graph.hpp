#pragma once
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>

struct Vertex{
    unsigned int key;
};
bool operator<(Vertex first, Vertex second);

class Graph{
    public:
    Graph() = delete;
    Graph(Vertex v);
    ~Graph();
    bool adjacent(Vertex, Vertex);
    std::vector<Vertex> neighbours(Vertex)const;
    bool add_vertex(Vertex);
    bool remove_vertex(Vertex);
    bool add_edge(Vertex, Vertex, double);
    bool remove_edge(Vertex, Vertex);
    Vertex get_vertex(Vertex) const;
    void set_vertex(Vertex, unsigned int);
    double get_edge(Vertex, Vertex) const;
    void set_edge(Vertex, Vertex, double);
    std::vector<std::vector<double>> _adjMat;
    std::vector<Vertex> _vertices;
};

std::ostream& operator<<(std::ostream& os, Graph g);