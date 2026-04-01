#include "graph.hpp"

#include <memory>
#include <utils.hpp>

std::random_device rd;
std::mt19937       gen(rd());

void populate_graph(Graph& g, unsigned int n)
{
    std::uniform_real_distribution<double> distrib(LOWER_DISTNACE_LIMIT, UPPER_DISTNACE_LIMIT);
    for (int i = 1; i < n; ++i)
    {
        g.add_vertex(i);
    }
    std::cout << "Graph after adding vertices: \n" << g << '\n';
    std::cout << "_vertices after adding vertices: \n";
    std::cout << g._vertices;
    for (unsigned int i = 0; i < n; ++i)
    {
        for (unsigned int j = 0; j < n; ++j)
        {
            if (i != j)
            {
                g.add_edge(g.get_vertex(i), g.get_vertex(j), distrib(gen));
            }
        }
    }
}