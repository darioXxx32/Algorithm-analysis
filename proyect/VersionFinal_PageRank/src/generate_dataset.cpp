#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>

static void usage() {
    std::cerr << "Usage: generate_dataset --output edges.csv --nodes N --avg-degree K [--seed S]\n";
}

int main(int argc, char* argv[]) {
    std::string output;
    int nodes = 0;
    int avg_degree = 6;
    unsigned int seed = 2026;

    try {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            auto value = [&](const std::string& name) -> std::string {
                if (i + 1 >= argc) {
                    throw std::invalid_argument("Missing value for " + name);
                }
                return argv[++i];
            };

            if (arg == "--output") {
                output = value(arg);
            } else if (arg == "--nodes") {
                nodes = std::stoi(value(arg));
            } else if (arg == "--avg-degree") {
                avg_degree = std::stoi(value(arg));
            } else if (arg == "--seed") {
                seed = static_cast<unsigned int>(std::stoul(value(arg)));
            } else {
                throw std::invalid_argument("Unknown argument: " + arg);
            }
        }

        if (output.empty() || nodes <= 1 || avg_degree <= 0) {
            throw std::invalid_argument("Invalid arguments");
        }

        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> dist(0, nodes - 1);
        std::set<std::pair<int, int>> edges;
        const long long target_edges = static_cast<long long>(nodes) * avg_degree;

        for (int i = 0; i + 1 < nodes; ++i) {
            edges.insert({i, i + 1});
        }

        while (static_cast<long long>(edges.size()) < target_edges) {
            int u = dist(rng);
            int v = dist(rng);
            if (u != v) {
                edges.insert({u, v});
            }
        }

        std::ofstream out(output);
        if (!out) {
            throw std::runtime_error("Cannot open output file");
        }
        out << "source,target\n";
        for (const auto& edge : edges) {
            out << "N" << edge.first << ",N" << edge.second << "\n";
        }

        std::cout << "output=" << output << "\n";
        std::cout << "nodes=" << nodes << "\n";
        std::cout << "edges=" << edges.size() << "\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        usage();
        return 1;
    }
}
