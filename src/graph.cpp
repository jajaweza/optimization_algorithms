#include <graph.hpp>
#include <iterator>
#include <stdexcept>

std::ostream &operator<<(std::ostream &os, const Graph &g) {
  for (const auto &col : g._adjMat) {
    for (const auto &elem : col) {
      os << elem << ' ';
    }
    os << '\n';
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Vertex> &v) {
  for (const auto &elem : v) {
    os << elem << " ";
  }
  os << '\n';
  return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<double> &v) {
  for (const auto &elem : v) {
    os << elem << " ";
  }
  os << '\n';
  return os;
}

// bool operator<(Vertex first, Vertex second){ return first.key < second.key; }
// bool operator==(Vertex first, Vertex second){ return first.key == second.key;
// }

template <typename T> unsigned int find(std::vector<T> v, const T &val) {
  for (int i = 0; i < v.size(); ++i) {
    if (v[i] == val) {
      return i;
    }
  }
  return v.size();
}
unsigned int Vertex::count = 0;

  Vertex::Vertex(){
    id = count++;
    if(id%3){
      open = 9;
      closed = 17;
    }
    else{
      open = 2;
      closed = 18;
    }
  }
  bool Vertex::operator==(const Vertex& other){
    return (id == other.id);
  }

  bool Vertex::operator==(const unsigned int& id){
    return (this->id == id);
  }


Graph::Graph() : _vertices(1, Vertex()) { _adjMat.emplace_back(1, 0); };

Graph::Graph(Vertex v) : _vertices(1, v) { _adjMat.emplace_back(1, 0); };

Graph::~Graph() {};

bool Graph::adjacent(Vertex v1, Vertex v2) {
  const auto idx1 = find(_vertices, v1);
  const auto idx2 = find(_vertices, v2);
  if ((idx1 == _vertices.size()) ||
      (idx2 == _vertices.size())) { // check if this should be OR or AND
    return false;
  }
  return ((_adjMat[idx1][idx2] != 0) || (_adjMat[idx2][idx1]) != 0);
};

std::vector<double> Graph::neighbours(const Vertex v) const {
  const auto idx = find(_vertices, v);
  if (idx != _vertices.size()) {
    return _adjMat[idx];
    // std::vector<Vertex> r;
    // if(idx == _vertices.size()){
    //     return r;
    // }

    // for(unsigned int i = 0; i < col.size(); ++i){
    //     if(col[i] != 0){
    //             r.emplace_back(_vertices[idx]);
    //     }
    // }
    // return r;
  }
  throw std::runtime_error{"Vertex doesnt exist!"};
};

void Graph::add_vertex(Vertex v) {
  if (_vertices.end() == std::find(_vertices.begin(), _vertices.end(), v)) {
    _vertices.emplace_back(v);
    _adjMat.emplace_back(std::vector<double>(_adjMat[0].size(), 0));
    for (auto &item : _adjMat) {
      item.emplace_back(0);
    }
    // std::cout << "Added vertex with key: " << v << '\n';
  }
};

bool Graph::remove_vertex(Vertex v) {
  const auto loc = find(_vertices, v);
  auto it = _adjMat.begin();
  std::advance(it, loc);
  _adjMat.erase(it);
  for (auto &item : _adjMat) {
    auto it2 = item.begin();
    std::advance(it, loc);
    item.erase(it2);
  }
  // std::vector<std::vector<double>> updatedAdjMat;
  // for(int i = 0; i < _adjMat.size()-1; ++i){
  //     updatedAdjMat.emplace_back(_adjMat.size()-1, 0);
  // }
  // const auto idx = find(_vertices, v);
  // if(idx == _vertices.size()) return false;
  // for(unsigned int i = 0, m = 0; i < _adjMat.size(); ++i){
  //     if(i != idx){
  //         for(unsigned int j = 0, n = 0; j < _adjMat.size(); ++j){
  //             if(j != idx){
  //                 updatedAdjMat[m][n] = _adjMat[i][j];
  //                 ++n;
  //             }
  //         }
  //     ++m;
  //     }
  // }
  // _adjMat = updatedAdjMat;
  // const auto it = std::find(_vertices.begin(), _vertices.end(), v);
  // if(it != _vertices.end()){
  //     _vertices.erase(it);
  // }
  return true;
};

bool Graph::add_edge(Vertex v1, Vertex v2, double val) {
  const auto it1 = std::find(_vertices.begin(), _vertices.end(), v1);
  const auto it2 = std::find(_vertices.begin(), _vertices.end(), v2);

  if ((it1 != _vertices.end()) && (it2 != _vertices.end())) {
    set_edge(v1, v2, val);
    return true;
  }
  return false;
};

bool Graph::remove_edge(Vertex v1, Vertex v2) {
  const auto it1 = std::find(_vertices.begin(), _vertices.end(), v1);
  const auto it2 = std::find(_vertices.begin(), _vertices.end(), v2);

  if ((it1 != _vertices.end()) && (it2 != _vertices.end())) {
    set_edge(v1, v2, 0);
    return true;
  }
  return false;
};

Vertex Graph::get_vertex(unsigned int id) const {
  const auto it = find_if(_vertices.begin(), _vertices.end(),
                          [id](Vertex v) { return v == id; });
  if (it == _vertices.end()) {
    throw std::runtime_error{"Requested vertex doesnt exist!"};
  }
  return *it;
};

void Graph::set_vertex(Vertex v, unsigned int val) {
  const auto id = find(_vertices, v);
  if (id < _vertices.size()) {
    _vertices[id] = v;
  }
};

double Graph::get_edge(Vertex v1, Vertex v2) const {
  const auto idx1 = find(_vertices, v1);
  const auto idx2 = find(_vertices, v2);
  if ((idx1 == _vertices.size()) || (idx2 == _vertices.size())) {
    return 0;
  }
  return _adjMat[idx1][idx2];
};

void Graph::set_edge(Vertex v1, Vertex v2, double val) {
  const auto idx1 = find(_vertices, v1);
  const auto idx2 = find(_vertices, v2);
  if ((idx1 == _vertices.size()) || (idx2 == _vertices.size())) {
    return;
  }
  _adjMat[idx1][idx2] = val;
  // _adjMat[idx2][idx1] = val;
};