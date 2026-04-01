#include "graph.hpp"
#include <ant_colony.hpp>
#include <cmath>
#include <complex>
#include <random>

Ant::Ant(Vertex start) : current_location(start), path(1, start) {
    std::cout << "Created an ant with starting location: " << start << std::endl;
}

AntColonyOpt::AntColonyOpt(Graph& g, unsigned int ant_count)
    : g(g), trail_levels(g._adjMat.size(), 1), alpha(1), beta(1), q(1), decay(0.2) {
        for(int i = 0; i < ant_count; ++i){
            ants.emplace_back(0);
        }
    }

AntColonyOpt::AntColonyOpt() {}

std::vector<std::vector<double>> AntColonyOpt::get_possible_moves() {
  std::vector<std::vector<double>> ants_neighbours;
  for (const auto &ant : ants) {
    const auto neighbours = g.neighbours(ant.current_location);
    ants_neighbours.emplace_back(neighbours.begin(), neighbours.end());
    // const auto loc =
    //     find(r.back().begin(), r.back().end(), ant.current_location);
    // r.back().erase(loc);
    // for(const auto& neighbour : r){
    //     r.emplace_back(g->get_edge(ant.current_location, neighbour));
    // }
  }

// for testing purposes
//   std::cout << "found possible moves: " << std::endl;
//   for(const auto& item : r){
//     std::cout << item << ' ';
//   }
//   std::cout << std::endl;

  return ants_neighbours;
}
std::vector<double>
AntColonyOpt::calculate_move_probabilities(std::vector<double> possible_moves) {
  std::vector<double> probabilities;
  double sum = 0;

// for testing purposes
//   std::cout << "calculating prob for following moves: " << std::endl;
//   for(const auto& item : possible_moves){
//     std::cout << item << " ";
//   }
//   std::cout << std::endl;

  for (int i = 0; i < possible_moves.size(); ++i) {
    if(possible_moves[i] != 0){
        double tau = std::pow(trail_levels[i], alpha);
        double eta = std::pow(1.0 / possible_moves[i], beta);
        sum += tau * eta;
    }
  }
  double tau = 0, eta = 0, p = 0;

  for (int i = 0; i < possible_moves.size(); ++i) {
    if(possible_moves[i] == 0) {
        probabilities.emplace_back(0);
    }
    else{
        tau = std::pow(trail_levels[i], alpha);
        eta = std::pow(possible_moves[i], -1*beta);
        p = tau * eta / sum;
        probabilities.emplace_back(p);

        // for testing purposes
        // std::cout << "calculated " << i << "th probability: " << p << std::endl;
        // std::cout << "calculated tau: " << tau << ", calculated eta: " << eta << std::endl;
        // std::cout << "sum = " << sum << std::endl;
    }
  }
  return probabilities;
}
void AntColonyOpt::move_ants() {
  const std::vector<std::vector<double>> possibilities = get_possible_moves();
  for (int i = 0; i < ants.size(); ++i) {
    const auto probabilities = calculate_move_probabilities(possibilities[i]);
    std::discrete_distribution<int> distrib(probabilities.begin(),
                                            probabilities.end());
    // for testing purposes
    // for(const auto& item : probabilities){
    //     std::cout << item << " ";
    // }
    // std::cout << std::endl;

    extern std::mt19937 gen;
    unsigned int roll = distrib(gen);
    ants[i].path.emplace_back(roll);
    ants[i].current_location = roll;
  }
  return;
}
void AntColonyOpt::update_trail_levels() {
  for (int i = 0; i < ants.size(); ++i) {
    const auto last = std::prev(ants[i].path.end(), 1);
    const auto second_to_last = std::prev(ants[i].path.end(), 2);
    trail_levels[i] =
        (1 - decay) * trail_levels[i] + q / g.get_edge(*second_to_last, *last);
  }
  return;
}

std::vector<Vertex> AntColonyOpt::run() {
  move_ants();
  update_trail_levels();
  return std::vector<Vertex>();
}