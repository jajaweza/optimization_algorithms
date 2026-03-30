#include <iostream>
#include <graph.hpp>

int main(int argc, char** argv){
    Vertex v1{1}, v2{2}, v3{3};
    Graph g(v1);

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
    for(const auto& elem : v){
        std::cout << elem.key << " ";
    }
    std::cout << std::endl;

    const auto p = g.adjacent(v1, v2);
    std::cout << "g.adjacent(v1, v2)\n" << p << std::endl;

    const auto q = g.adjacent(v1, v3);
    std::cout << "g.adjacent(v1, v3)\n" << q << std::endl;

    g.set_edge(v1, v3, 4);
    std::cout << "g.set_edge(v1, v3, 4)\n" << g << std::endl;

    g.set_vertex(v3, 4);
    std::cout << "g.set_vertex(v3, 4)\n" << g << std::endl;

    const auto c = g.get_vertex(v3);
    std::cout << "g.get_vertex(v3) \n" << c.key << std::endl; 

    const auto e = g.get_edge(v1, v3);
    std::cout << "g.get_edge(v1, v3); \n" << e << std::endl; 

    g.remove_edge(v1, v2);
    std::cout << "g.remove_edge(v1, v2)\n" << g << std::endl;

    g.remove_vertex(v1);
    std::cout << "g.remove_vertex(v1)\n" << g << std::endl;

    std::cout << "Hello world!" << std::endl;
    return 0;
}