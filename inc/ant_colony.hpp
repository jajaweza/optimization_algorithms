#pragma once

#include "graph.hpp"

#include <vector>

struct Ant
{
    Vertex                           current_location;
    std::vector<Vertex>              current_route;
    std::vector<std::vector<Vertex>> routes;
    std::vector<double>              route_distances;
    double                           total_distance;
    Ant(Vertex start);
};

class AntColonyOpt
{
  private:
    Graph&                           g;
    std::vector<std::vector<double>> trail_levels;
    std::vector<Ant>                 ants;
    double                           alpha;
    double                           beta;
    std::vector<double>              decay;

    std::vector<double> calculate_move_probabilities(const Vertex& loc, const unsigned int step,
                                                     unsigned int             ant_nr,
                                                     const std::vector<bool>& visited);

    void construct_solution(Ant& ant);

  public:
    AntColonyOpt(Graph& g, unsigned int ant_count);

    void                             move_ants();
    void                             update_trail_levels();
    std::vector<std::vector<Vertex>> run();
};