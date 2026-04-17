#pragma once

#include "graph.hpp"
#include <vector>

// using Vertex = int;

struct Ant {
  Vertex current_location;

  // Current route being built
  std::vector<Vertex> current_route;

  // All routes (each = one driver)
  std::vector<std::vector<Vertex>> routes;

  // Distance per driver
  std::vector<double> route_distances;

  // Total solution cost
  double total_distance;

  Ant(Vertex start);
};

class AntColonyOpt {
private:
  Graph &g;

  // Pheromone on EDGES (important change)
  std::vector<std::vector<double>> trail_levels;

  std::vector<Ant> ants;

  double alpha;
  double beta;
  double q;
  double decay;

  std::vector<double> calculate_move_probabilities(
      const std::vector<double> &distances,
      unsigned int ant_nr,
      const std::vector<bool> &visited);

  void construct_solution(Ant &ant);

public:
  AntColonyOpt(Graph &g, unsigned int ant_count);

  void move_ants();
  void update_trail_levels();

  // Now returns multiple routes (VRP solution)
  std::vector<std::vector<Vertex>> run();
};