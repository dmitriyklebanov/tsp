#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#define MAKE_HISTORY
#include "annealing.hpp"
#include "clonal_selection.hpp"

Data input;
MetaheuristicAlgorithm* test_algorithm = nullptr;

void invalidParams(const std::string& msg) {
    std::cout << "Invalid arguments. " << msg << " Format:\n";
    std::cout << "[algo_name][input_file] [output_file] ([config_file])\n";
    std::cout << "\nAvailable algorithms : annealing, clonal_selection\n";
    exit(42);
}

int main(int argc, char* argv[]) {
    auto start = std::chrono::steady_clock().now();
    if ((argc != 4) && (argc != 5)) {
        invalidParams("Wrong number of arguments.");
    }

    std::string algorithm_name = argv[1];
    std::string input_file     = argv[2];
    std::string output_file    = argv[3];

    if (algorithm_name == "annealing") {
        test_algorithm = new Annealing;
    }

    if (algorithm_name == "clonal_selection") {
        test_algorithm = new ClonalSelection;
    }

    if (test_algorithm == nullptr) {
        invalidParams("Unknown test algorithm.");
    }


    std::ifstream in(input_file);
    if (!in.is_open()) {
        std::cout << "Can't open file input file" << std::endl;
        return 0;
    }
    in >> input;
    in.close();

    test_algorithm->setInput(input);

    if (argc == 5) {
        std::ifstream cfg(argv[4]);
        if (!cfg.is_open()) {
            std::cout << "Can't open file config file" << std::endl;
            return 0;
        }
        test_algorithm->loadConfig(cfg);
        cfg.close();
    }

    std::ofstream out(output_file);
    if (!out.is_open()) {
        std::cout << "Can't create file" << std::endl;
        return 0;
    }

    std::cout << "Test algorithm has been started" << std::endl;
    test_algorithm->start(out);
    out << "hist_end\n";
    std::cout << "Test algorithm has been finished successfully" << std::endl;

    out << test_algorithm->getSeed() << "\n";
    out << test_algorithm->getSolution().getAns() << "\n";
    out << test_algorithm->getSolution();
    out.close();

    std::cout << "ans = " << test_algorithm->getSolution().getAns() << "\n";

    auto finish = std::chrono::steady_clock().now();

    std::cout << "time = " << (std::chrono::duration_cast<std::chrono::milliseconds>(finish - start)).count() << " ms" << std::endl;

    return 0;
}
