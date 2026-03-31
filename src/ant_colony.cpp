#include <ant_colony.hpp>
#include <cmath>
#include <random>


    std::vector<std::vector<double>> AntColonyOpt::get_possible_moves(){
        std::vector<std::vector<double>> r;
        for(const auto& ant : ants){
            const auto neighbours = g->neighbours(ant.current_location);
            r.emplace_back(neighbours);
            // for(const auto& neighbour : r){
            //     r.emplace_back(g->get_edge(ant.current_location, neighbour));
            // }
        }
        return r;
    }
    std::vector<double> AntColonyOpt::calculate_move_probabilities(std::vector<double> possible_moves){
        std::vector<double> r;
        double sum = 0;
        for(int i = 0; i < possible_moves.size(); ++i){
            double tau = std::pow(trail_levels[i], alpha);
            double eta = std::pow(1/possible_moves[i], beta);
            sum += tau*eta;
        }
        for(int i = 0; i < possible_moves.size(); ++i){
            double tau = std::pow(trail_levels[i], alpha);
            double eta = std::pow(1/possible_moves[i], beta);
            r.emplace_back(tau*eta/sum);
        }
        return r;
    }
    void AntColonyOpt::move_ants(){
        const auto
        for(int i = 0; i < ants.size(); ++i){
            std::discrete_distribution<double> distrib(calculate_move_probabilities());
            extern std::mt19937 gen;
            double roll = distrib(gen);
        }
        return;
    }
    void AntColonyOpt::update_trail_levels(){
        for(int i = 0; i < ants.size(); ++i){
            const auto last =  std::prev(ants[i].path.end(), 1);
            const auto second_to_last =  std::prev(ants[i].path.end(), 2);
            trail_levels[i] = q/g->get_edge(*second_to_last, *last);
        }
        return;
    }