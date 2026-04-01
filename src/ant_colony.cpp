#include "graph.hpp"
#include <ant_colony.hpp>
#include <cmath>
#include <complex>
#include <map>
#include <random>

Ant::Ant(Vertex start) : current_location(start), path(1, start) {
  std::cout << "Created an ant with starting location: " << start << '\n';
}

AntColonyOpt::AntColonyOpt(Graph &g, unsigned int ant_count)
    : g(g), trail_levels(g._adjMat.size(), 1), alpha(1), beta(1), q(1),
      decay(DECAY) {
  for (int i = 0; i < ant_count; ++i) {
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
  //   std::cout << "found possible moves: " << '\n';
  //   for(const auto& item : r){
  //     std::cout << item << ' ';
  //   }
  //   std::cout << '\n';

  return ants_neighbours;
}
std::vector<double>
AntColonyOpt::calculate_move_probabilities(std::vector<double> possible_moves,
                                           unsigned int ant_nr) {
  std::vector<double> probabilities;
  double sum = 0;

  // for testing purposes
  //   std::cout << "calculating prob for following moves: " << '\n';
  //   for(const auto& item : possible_moves){
  //     std::cout << item << " ";
  //   }
  //   std::cout << '\n';
  double tau = 0, eta = 0, p = 0;
  const auto current_ant = ants.at(ant_nr);
  for (int i = 0; i < possible_moves.size(); ++i) {
    if (current_ant.path.end() ==
        std::find(current_ant.path.begin(), current_ant.path.end(), i)) {
      tau = std::pow(trail_levels[i], alpha);
      eta = std::pow(1.0 / possible_moves[i], beta);
      sum += tau * eta;
    }
  }

  for (int i = 0; i < possible_moves.size(); ++i) {
    if (current_ant.path.end() ==
        std::find(current_ant.path.begin(), current_ant.path.end(), i)) {
      tau = std::pow(trail_levels[i], alpha);
      eta = std::pow(possible_moves[i], -1 * beta);
      p = tau * eta / sum;
      probabilities.emplace_back(p);

      // for testing purposes
      // std::cout << "calculated " << i << "th probability: " << p <<
      // '\n'; std::cout << "calculated tau: " << tau << ", calculated eta:
      // " << eta << '\n'; std::cout << "sum = " << sum << '\n';
    } else {
      probabilities.emplace_back(0);
    }
  }
  // for testing purposes
  //   double prob_sum{0};
  //   std::cout << "calculated probabilities: " << '\n';
  //   for (int i = 0; i < probabilities.size(); ++i) {
  //     std::cout << "probability to go to " << i << " vertex is "
  //               << probabilities.at(i) << '\n';
  //     prob_sum += probabilities.at(i);
  //   }
  //   std::cout << "sum of calculated probabilities: " << prob_sum << '\n';
  return probabilities;
}
void AntColonyOpt::move_ants() {
  const std::vector<std::vector<double>> possibilities = get_possible_moves();
  for (int i = 0; i < ants.size(); ++i) {
    const auto probabilities =
        calculate_move_probabilities(possibilities[i], i);
    std::discrete_distribution<int> distrib(probabilities.begin(),
                                            probabilities.end());
    // for testing purposes
    // for(const auto& item : probabilities){
    //     std::cout << item << " ";
    // }
    // std::cout << '\n';

    extern std::mt19937 gen;
    unsigned int roll = distrib(gen);
    ants[i].path.emplace_back(roll);
    ants[i].current_location = roll;
  }
}
void AntColonyOpt::update_trail_levels() {
  for (int i = 0; i < ants.size(); ++i) {
    const auto last = std::prev(ants[i].path.end(), 1);
    const auto second_to_last = std::prev(ants[i].path.end(), 2);
    trail_levels.at(ants[i].current_location) =
        ((1 - decay) * trail_levels.at(ants[i].current_location)) +
        (q / g.get_edge(*second_to_last, *last));
  }
}

std::vector<Vertex> AntColonyOpt::run() {
  move_ants();
  update_trail_levels();
  return std::vector<Vertex>();
}

std::vector<unsigned int> AntColonyOpt::choose_best_path() {
  std::map<std::vector<unsigned int>, unsigned int> map;
  for (const auto &ant : ants) {
    auto pair = map.emplace(ant.path, 1);
    if (!pair.second) {
      ++map.at(ant.path);
    }
  }
  auto it = map.begin();
  auto loc = map.begin();
  unsigned int max_value{0};
  while (it != map.end()) {
    // for testing purposes
    // std::cout << "score for path: " << '\n';
    // for(const auto& item : it->first){
    //     std::cout << item << " -> ";
    // }
    // std::cout << '0' << '\n';
    // std::cout << "is " << it->second << '\n';
    if (it->second > max_value) {
      max_value = it->second;
      loc = it;
    }
    std::advance(it, 1);
  }
  return loc->first;
}