#include "graph.hpp"

#include <ant_colony.hpp>
#include <cmath>
#include <limits>
#include <map>
#include <numeric>
#include <random>

static const Vertex DEPOT        = 0;
static const double DECAY        = 0.5;
static const double MAX_DISTANCE = 30;

Ant::Ant(Vertex start) : current_location(start), current_route({start}), total_distance(0.0) {}

AntColonyOpt::AntColonyOpt(Graph& g, unsigned int ant_count)
    : g(g)
    , trail_levels(g._adjMat.size(), std::vector<double>(g._adjMat.size(), 1.0))
    , alpha(1)
    , beta(2)
    , q(1)
    , decay(DECAY)
{

    for (int i = 0; i < ant_count; ++i)
    {
        ants.emplace_back(DEPOT);
    }
}

std::vector<double> AntColonyOpt::calculate_move_probabilities(const std::vector<double>& distances,
                                                               unsigned int               ant_nr,
                                                               const std::vector<bool>&   visited)
{

    std::vector<double> probabilities(distances.size(), 0.0);
    double              sum = 0.0;

    const auto& ant = ants[ant_nr];
    Vertex      i   = ant.current_location;

    for (int j = 0; j < distances.size(); ++j)
    {
        if (!visited[j] && j != i && distances[j] > 0)
        {
            double tau = std::pow(trail_levels[i][j], alpha);
            double eta = std::pow(1.0 / distances[j], beta);
            sum += tau * eta;
        }
    }

    if (sum == 0)
        return probabilities;

    for (int j = 0; j < distances.size(); ++j)
    {
        if (!visited[j] && j != i && distances[j] > 0)
        {
            double tau       = std::pow(trail_levels[i][j], alpha);
            double eta       = std::pow(1.0 / distances[j], beta);
            probabilities[j] = (tau * eta) / sum;
        }
    }

    return probabilities;
}

void AntColonyOpt::construct_solution(Ant& ant)
{

    std::vector<bool> visited(g._adjMat.size(), false);
    visited[DEPOT] = true;

    while (true)
    {
        bool any_unvisited = false;
        for (int i = 0; i < visited.size(); ++i)
        {
            if (!visited[i])
            {
                any_unvisited = true;
                break;
            }
        }
        if (!any_unvisited)
            break;

        // Start new route (new driver)
        ant.current_route     = {DEPOT};
        ant.current_location  = DEPOT;
        double route_distance = 0.0;

        while (true)
        {
            const auto distances = g.neighbours(ant.current_location);
            auto probabilities = calculate_move_probabilities(distances, &ant - &ants[0], visited);
            // std::cout << "calculated probabilities: \n";
            // for(const auto& item : probabilities){
            //   std::cout << item << ' ';
            // }
            // std::cout << '\n';
            if (std::accumulate(probabilities.begin(), probabilities.end(), 0.0) == 0.0)
                break;
            std::discrete_distribution<int> distrib(probabilities.begin(), probabilities.end());
            extern std::mt19937             gen;
            int                             next = distrib(gen);
            // No valid move → return to depot
            if (route_distance + g.get_edge(ant.current_location, next) > MAX_DISTANCE)
            {
                break;
            }

            // Move
            ant.current_route.push_back(next);
            route_distance += g.get_edge(ant.current_location, next);

            ant.current_location = next;
            visited[next]        = true;
        }

        // Return to depot
        ant.current_route.push_back(DEPOT);
        route_distance += g.get_edge(ant.current_location, DEPOT);

        ant.routes.push_back(ant.current_route);
        ant.route_distances.push_back(route_distance);
        ant.total_distance += route_distance;
    }
}

void AntColonyOpt::move_ants()
{
    for (auto& ant : ants)
    {
        ant.routes.clear();
        ant.route_distances.clear();
        ant.total_distance = 0.0;

        construct_solution(ant);
    }
}

void AntColonyOpt::update_trail_levels()
{

    // Evaporation
    for (int i = 0; i < trail_levels.size(); ++i)
    {
        for (int j = 0; j < trail_levels.size(); ++j)
        {
            trail_levels[i][j] *= (1 - decay);
        }
    }

    // Deposit
    for (const auto& ant : ants)
    {
        double contribution = q / ant.total_distance;

        for (const auto& route : ant.routes)
        {
            for (int i = 0; i < route.size() - 1; ++i)
            {
                int from = route[i];
                int to   = route[i + 1];

                trail_levels[from][to] += contribution;
                trail_levels[to][from] += contribution;
            }
        }
    }
}

std::vector<std::vector<Vertex>> AntColonyOpt::run()
{
    move_ants();
    update_trail_levels();

    // Return best solution
    const Ant* best      = nullptr;
    double     best_cost = std::numeric_limits<double>::max();

    for (const auto& ant : ants)
    {
        if (ant.total_distance < best_cost)
        {
            best_cost = ant.total_distance;
            best      = &ant;
        }
    }

    if (best)
        return best->routes;
    return {};
}