#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

struct Options {
    std::string input_path;
    std::string output_path = "pagerank_scores.csv";
    double damping = 0.85;
    double tolerance = 1e-10;
    int max_iterations = 200;
};

struct Graph {
    std::vector<std::string> labels;
    std::vector<std::vector<int>> incoming;
    std::vector<int> out_degree;
    long long edges = 0;
};

static void print_usage() {
    std::cerr
        << "Usage: pagerank --input edges.csv [--output scores.csv] "
        << "[--damping 0.85] [--tol 1e-10] [--max-iter 200]\n";
}

static Options parse_args(int argc, char* argv[]) {
    Options options;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto require_value = [&](const std::string& name) -> std::string {
            if (i + 1 >= argc) {
                throw std::invalid_argument("Missing value for " + name);
            }
            return argv[++i];
        };

        if (arg == "--input") {
            options.input_path = require_value(arg);
        } else if (arg == "--output") {
            options.output_path = require_value(arg);
        } else if (arg == "--damping") {
            options.damping = std::stod(require_value(arg));
        } else if (arg == "--tol") {
            options.tolerance = std::stod(require_value(arg));
        } else if (arg == "--max-iter") {
            options.max_iterations = std::stoi(require_value(arg));
        } else if (arg == "--help" || arg == "-h") {
            print_usage();
            std::exit(0);
        } else {
            throw std::invalid_argument("Unknown argument: " + arg);
        }
    }

    if (options.input_path.empty()) {
        throw std::invalid_argument("--input is required");
    }
    if (!(options.damping > 0.0 && options.damping < 1.0)) {
        throw std::invalid_argument("--damping must satisfy 0 < d < 1");
    }
    if (options.tolerance <= 0.0) {
        throw std::invalid_argument("--tol must be positive");
    }
    if (options.max_iterations <= 0) {
        throw std::invalid_argument("--max-iter must be positive");
    }
    return options;
}

static std::string trim(const std::string& s) {
    const auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

static int id_for_label(
    const std::string& label,
    std::unordered_map<std::string, int>& id,
    std::vector<std::string>& labels
) {
    auto it = id.find(label);
    if (it != id.end()) {
        return it->second;
    }
    const int next = static_cast<int>(labels.size());
    id[label] = next;
    labels.push_back(label);
    return next;
}

static Graph read_edge_list(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Cannot open input file: " + path);
    }

    std::unordered_map<std::string, int> id;
    std::vector<std::string> labels;
    std::vector<std::pair<int, int>> edges;
    std::string line;
    bool first_line = true;

    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::replace(line.begin(), line.end(), ',', ' ');
        std::istringstream iss(line);
        std::string source;
        std::string target;
        if (!(iss >> source >> target)) {
            continue;
        }

        if (first_line && (source == "source" || source == "from") &&
            (target == "target" || target == "to")) {
            first_line = false;
            continue;
        }
        first_line = false;

        const int u = id_for_label(source, id, labels);
        const int v = id_for_label(target, id, labels);
        if (u != v) {
            edges.push_back({u, v});
        }
    }

    Graph graph;
    graph.labels = std::move(labels);
    const int n = static_cast<int>(graph.labels.size());
    graph.incoming.assign(n, {});
    graph.out_degree.assign(n, 0);

    std::sort(edges.begin(), edges.end());
    edges.erase(std::unique(edges.begin(), edges.end()), edges.end());
    graph.edges = static_cast<long long>(edges.size());

    for (const auto& edge : edges) {
        graph.out_degree[edge.first]++;
        graph.incoming[edge.second].push_back(edge.first);
    }

    return graph;
}

struct PageRankResult {
    std::vector<double> scores;
    int iterations = 0;
    bool converged = false;
    double final_error = 0.0;
    double seconds = 0.0;
};

static PageRankResult pagerank(const Graph& graph, double damping, double tolerance, int max_iterations) {
    const int n = static_cast<int>(graph.labels.size());
    if (n == 0) {
        throw std::runtime_error("The graph has no nodes");
    }

    std::vector<double> rank(n, 1.0 / n);
    std::vector<double> next(n, 0.0);
    const double teleport = (1.0 - damping) / n;
    PageRankResult result;

    const auto start = std::chrono::high_resolution_clock::now();
    for (int iter = 1; iter <= max_iterations; ++iter) {
        double dangling_mass = 0.0;
        for (int u = 0; u < n; ++u) {
            if (graph.out_degree[u] == 0) {
                dangling_mass += rank[u];
            }
        }

        const double dangling_contribution = damping * dangling_mass / n;
        std::fill(next.begin(), next.end(), teleport + dangling_contribution);

        for (int v = 0; v < n; ++v) {
            double link_sum = 0.0;
            for (int u : graph.incoming[v]) {
                link_sum += rank[u] / graph.out_degree[u];
            }
            next[v] += damping * link_sum;
        }

        double error = 0.0;
        for (int i = 0; i < n; ++i) {
            error += std::abs(next[i] - rank[i]);
        }

        rank.swap(next);
        result.iterations = iter;
        result.final_error = error;
        if (error < tolerance) {
            result.converged = true;
            break;
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    result.seconds = std::chrono::duration<double>(end - start).count();
    result.scores = std::move(rank);
    return result;
}

static void write_scores(const Graph& graph, const PageRankResult& result, const std::string& path) {
    std::vector<int> order(graph.labels.size());
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        if (result.scores[a] == result.scores[b]) {
            return graph.labels[a] < graph.labels[b];
        }
        return result.scores[a] > result.scores[b];
    });

    std::ofstream out(path);
    if (!out) {
        throw std::runtime_error("Cannot open output file: " + path);
    }

    out << "rank,node,score,out_degree,in_degree\n";
    out << std::setprecision(15);
    for (std::size_t i = 0; i < order.size(); ++i) {
        const int node = order[i];
        out << (i + 1) << ','
            << graph.labels[node] << ','
            << result.scores[node] << ','
            << graph.out_degree[node] << ','
            << graph.incoming[node].size() << '\n';
    }
}

int main(int argc, char* argv[]) {
    try {
        const Options options = parse_args(argc, argv);
        const Graph graph = read_edge_list(options.input_path);
        const PageRankResult result = pagerank(
            graph,
            options.damping,
            options.tolerance,
            options.max_iterations
        );
        write_scores(graph, result, options.output_path);

        std::cout << "nodes=" << graph.labels.size() << '\n';
        std::cout << "edges=" << graph.edges << '\n';
        std::cout << "iterations=" << result.iterations << '\n';
        std::cout << "converged=" << (result.converged ? "true" : "false") << '\n';
        std::cout << "final_l1_error=" << std::setprecision(12) << result.final_error << '\n';
        std::cout << "time_seconds=" << std::setprecision(12) << result.seconds << '\n';
        std::cout << "output=" << options.output_path << '\n';
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n\n";
        print_usage();
        return 1;
    }
}
