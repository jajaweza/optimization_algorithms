#pragma once
#include "graph.hpp"
#include <memory>
#include <utils.hpp>
#include <vector>

struct Ant{
    Vertex current_location;
    std::vector<Vertex> path;
};

class AntColonyOpt{
    std::vector<std::vector<double>> get_possible_moves();
    std::vector<double> calculate_move_probabilities(std::vector<double> possible_moves);
    void move_ants();
    void update_trail_levels();

    std::vector<Ant> ants;
    std::vector<double> trail_levels; // each edge in graph has its trail level
    std::shared_ptr<Graph> g;
    double q; // constant used to calculate trail level update
    double alpha;
    double beta;
};