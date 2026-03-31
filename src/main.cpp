#include <iostream>
#include <graph.hpp>
#include <memory>
#include <utils.hpp>

int main(int argc, char** argv){
    Vertex v1{1}, v2{2}, v3{3};
    Graph g(v1);

    std::cout << "############ Begining of Graph ADT Implementation tests ############\n";
    g.add_vertex(v2);
    std::cout << "g.add_vertex(v2) \n" << g << std::endl;

    g.add_edge(v1, v2, 10);
    std::cout << "g.add_edge(v1, v2, 10) \n" << g << std::endl;

    g.add_vertex(v3);
    std::cout <<"g.add_vertex(v3) \n" << g << std::endl;

    g.add_edge(v2, v3, 20);
    std::cout <<"g.add_edge(v2, v3, 20) \n" << g << std::endl;

    const auto v = g.neighbours(v2);
    std::cout << "g.neighbours(v2)\n";
    std::cout << v;
    std::cout << std::endl;

    const auto p = g.adjacent(v1, v2);
    std::cout << "g.adjacent(v1, v2)\n" << p << std::endl;

    const auto q = g.adjacent(v1, v3);
    std::cout << "g.adjacent(v1, v3)\n" << q << std::endl;

    g.set_edge(v1, v3, 4);
    std::cout << "g.set_edge(v1, v3, 4)\n" << g << std::endl;

    g.set_vertex(v3, 4);
    std::cout << "g.set_vertex(v3, 4)\n" << g << std::endl;

    const auto c = g.get_vertex(3);
    std::cout << "g.get_vertex(v3) \n" << c << std::endl; 

    const auto e = g.get_edge(v1, v3);
    std::cout << "g.get_edge(v1, v3); \n" << e << std::endl; 

    g.remove_edge(v1, v2);
    std::cout << "g.remove_edge(v1, v2)\n" << g << std::endl;

    g.remove_vertex(v1);
    std::cout << "g.remove_vertex(v1)\n" << g << std::endl;

    std::cout << "############ End of Graph ADT Implementation tests ############\n";
    std::cout << "\n\n";
    std::cout << "############ Beginning of Random Graph Population tests ############\n";
    std::unique_ptr<Graph> g_ptr = std::make_unique<Graph>();
    populate_graph(g_ptr, 5);
    std::cout << "populate_graph(g_ptr, 5): \n";
    std::cout << *g_ptr;
    std::cout << "############ End of Random Graph Population tests ############\n";
    return 0;
}