#include <ant_colony.hpp>
#include <graph.hpp>
#include <iostream>
#include <memory>
#include <utils.hpp>
#include <fstream>
#include <chrono>
#include <vector>
#include <limits>

#define ANT_COUNT 50
#define TUNING_GRAPH_SIZE 100
#define PARAMETERS_COUNT 20
#define STEPS (TUNING_GRAPH_SIZE - 1)
#define TUNING 
#define COMPLEXITY_TIMING 
unsigned int Vertex::count = 0;

int main(int argc, char **argv) {
#ifdef TUNING
    std::vector<double> alpha_values;
    std::vector<double> beta_values;
    for(int i = 0; i < PARAMETERS_COUNT; ++i){
      alpha_values.emplace_back(i*1./5);
      beta_values.emplace_back(1+i*1./5);
    }
    
    double best_alpha = alpha_values[0];
    double best_beta = beta_values[0];
    double min_exec_time = std::numeric_limits<double>::max();
    Graph g1;
    populate_graph(g1, TUNING_GRAPH_SIZE);

    std::cout << "############ Beginning of Ant Colony Optimization Tuning ############\n";
    
    std::ofstream tuning_csv("param_tuning.csv");
    if (!tuning_csv.is_open()) {
        std::cerr << "Error: Could not open param_tuning.csv for writing.\n";
        return 1;
    }
    tuning_csv << "Alpha,Beta,ExecutionTime_ms\n";

    for (double alpha : alpha_values) {
        for (double beta : beta_values) {
            AntColonyOpt aco(g1, ANT_COUNT, alpha, beta);

            auto start_time = std::chrono::high_resolution_clock::now();
            const auto result = aco.run();
            auto end_time = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
            double current_time_ms = elapsed.count();

            tuning_csv << alpha << "," << beta << "," << current_time_ms << "\n";

            std::cout << "[Tuning Sweep] Alpha: " << alpha << " | Beta: " << beta 
                      << " -> Time: " << current_time_ms << " ms\n";

            if (current_time_ms < min_exec_time) {
                min_exec_time = current_time_ms;
                best_alpha = alpha;
                best_beta = beta;
            }
        }
    }
    tuning_csv.close();

    std::cout << "\n>>> Best Hyperparameters Selected: Alpha = " << best_alpha 
              << ", Beta = " << best_beta << " (Lowest Time: " << min_exec_time << " ms)\n\n";
#else
    double best_alpha = 1;
    double best_beta = 2;
#endif
#ifdef COMPLEXITY_TIMING
    std::cout << "############ Beginning of Algorithm Complexity Analysis ############\n";
    
    std::vector<int> graph_sizes = {50, 100, 150, 200, 250, 300, 400, 500, 1000, 2000, 3000};
    
    std::ofstream complexity_csv("complexity_analysis.csv");
    if (!complexity_csv.is_open()) {
        std::cerr << "Error: Could not open complexity_analysis.csv for writing.\n";
        return 1;
    }
    complexity_csv << "GraphSize,ExecutionTime_ms\n";
    Graph g_scale;

    for (int size : graph_sizes) {
        populate_graph(g_scale, size);

        AntColonyOpt aco(g_scale, ANT_COUNT, best_alpha, best_beta);

        auto start_time = std::chrono::high_resolution_clock::now();
        const auto result = aco.run();
        auto end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
        double current_time_ms = elapsed.count();

        complexity_csv << size << "," << current_time_ms << "\n";

        std::cout << "[Complexity Sweep] Graph Size: " << size 
                  << " -> Time: " << current_time_ms << " ms\n";
    }
    complexity_csv.close();

    std::cout << "\nBenchmarking operations successfully completed!\n";
    std::cout << "Results saved to: 'param_tuning.csv' and 'complexity_analysis.csv'.\n";
#endif
    
    return 0;
}