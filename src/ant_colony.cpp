#include "graph.hpp"

#include <ant_colony.hpp>
#include <cmath>
#include <iomanip>
#include <limits>
#include <map>
#include <numeric>
#include <random>

static const int DEPOT = 0;
// static const double DECAY        = 0.5;
static const double MAX_DISTANCE = 100;
static const double ALPHA        = 1;
static const double BETA         = 2;
static const double max_decay = 0.9;
static const double min_decay = 0.1;

Ant::Ant(Vertex start) : current_location(start), current_route({start}), total_distance(0.0) {}

AntColonyOpt::AntColonyOpt(Graph& g, unsigned int ant_count)
    : g(g)
    , trail_levels(g._adjMat.size(), std::vector<double>(g._adjMat.size(), 1.0))
    , alpha(ALPHA)
    , beta(BETA)
    , decay(ant_count)
{
    for (int i = 0; i < ant_count; ++i)
    {
        ants.emplace_back(g.get_vertex(DEPOT));
        decay[i] =  min_decay + (max_decay - min_decay)*i/ant_count;
    }
}

AntColonyOpt::AntColonyOpt(Graph& g, unsigned int ant_count, double alpha, double beta)
    : g(g)
    , trail_levels(g._adjMat.size(), std::vector<double>(g._adjMat.size(), 1.0))
    , alpha(alpha)
    , beta(beta)
    , decay(ant_count)
{
    for (int i = 0; i < ant_count; ++i)
    {
        ants.emplace_back(g.get_vertex(DEPOT));
        decay[i] = 0.9 - 0.05 * i;
    }
}

double how_late(unsigned int time, const Vertex& place)
{
    if(time < place.open) {
        return place.open - time;
    }
    else if(time > place.closed){
        return time - place.closed;
    }
    else {
        return 0;
    }
}
std::vector<double> AntColonyOpt::calculate_move_probabilities(const Vertex&            loc,
                                                               const unsigned int       step,
                                                               unsigned int             ant_nr,
                                                               const std::vector<bool>& visited)
{
    const auto distances = g.neighbours(loc);
    std::vector<double> probabilities(distances.size(), 0.0);
    double              sum = 0.0;

    const Ant&   ant = ants[ant_nr];
    unsigned int i   = ant.current_location.id;

    for (int j = 0; j < distances.size(); ++j)
    {
        if (!visited[j] && j != i && distances[j] > 0)
        {
            double cost = distances[j] + how_late(step, g._vertices[j]);
            double tau  = std::pow(trail_levels[i][j], alpha);
            double eta  = std::pow(1.0 / cost, beta);
            sum += tau * eta;
        }
    }

    if (sum == 0)
    {
        // std::cout << "Probabilities1: " << probabilities;
        return probabilities;
    }

    for (int j = 0; j < distances.size(); ++j)
    {
        if (!visited[j] && j != i && distances[j] > 0)
        {
            double cost = distances[j] + how_late(step, g._vertices[j]);
            double tau       = std::pow(trail_levels[i][j], alpha);
            double eta       = std::pow(1.0 / cost, beta);
            probabilities[j] = (tau * eta) / sum;
        }
    }
    // std::cout << std::setprecision(2) << "Probabilities2: " << probabilities;
    return probabilities;
}

void AntColonyOpt::construct_solution(Ant& ant)
{

    std::vector<bool> visited(g._adjMat.size(), false);
    // std::cout << "visited.size() = " << visited.size() << '\n';
    visited[DEPOT] = true;

    while (true)
    {
        bool any_unvisited = false;
        for (int i = 0; i < visited.size(); ++i)
        {
            if (!visited[i])
            {
                // std::cout << "the " << i << "th node unvisited, go back!\n";
                any_unvisited = true;
                break;
            }
        }
        if (!any_unvisited){
            break;
        }

        ant.current_route     = {g.get_vertex(DEPOT)};
        ant.current_location  = g.get_vertex(DEPOT);
        double route_distance = 0.0;

        while (true)
        {
            const auto distances = g.neighbours(ant.current_location);
            auto       probabilities =
                calculate_move_probabilities(ant.current_location, ant.current_route.size(),
                                             &ant - &ants[0], visited);
            // std::cout << "calculated probabilities: \n";
            // for(const auto& item : probabilities){
            //   std::cout << item << ' ';
            // }
            // std::cout << '\n';
            if (std::accumulate(probabilities.begin(), probabilities.end(), 0.0) == 0.0){
                break;
            }
            std::discrete_distribution<int> distrib(probabilities.begin(), probabilities.end());
            extern std::mt19937             gen;
            int                             next          = distrib(gen);
            Vertex                          next_location = g.get_vertex(next);
            // std::cout << "still in the while true loop\n";
            if (route_distance + g.get_edge(ant.current_location, next_location) >= MAX_DISTANCE)
            {
                // std::cout << "breaking out of the while true loop!\n";
                break;
            }

            ant.current_route.push_back(next_location);
            route_distance += g.get_edge(ant.current_location, next_location);

            ant.current_location = next_location;
            visited[next]        = true;
            // std::cout << "visited[" << next
            //           << "] = " << visited[next] << '\n';
        }

        ant.current_route.push_back(g.get_vertex(DEPOT));
        route_distance += g.get_edge(ant.current_location, g.get_vertex(DEPOT));

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
    for (const auto& ant_decay : decay)
    {
        for (int i = 0; i < trail_levels.size(); ++i)
        {
            for (int j = 0; j < trail_levels.size(); ++j)
            {
                trail_levels[i][j] *= (1 - ant_decay);
            }
        }
    }

    for (const auto& ant : ants)
    {
        double contribution = 1 / ant.total_distance;

        for (const auto& route : ant.routes)
        {
            for (int i = 0; i < route.size() - 1; ++i)
            {
                int from = route[i].id;
                int to   = route[i + 1].id;

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