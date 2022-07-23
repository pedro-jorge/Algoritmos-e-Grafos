#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stdexcept>

class Vertex {
private:
    unsigned int id;

public:
    explicit Vertex(unsigned int id_) {
        id = id_;
    }

    auto operator==(const Vertex& other) const { return id == other.id; }
    [[nodiscard]] auto getId() const { return id; }
};

namespace std {
    template <>
    struct hash<Vertex> {
        auto operator()(const Vertex &vertex) const -> size_t {
            return hash<unsigned int>()(vertex.getId());
        }
    };
}

class Graph {
private:
    bool isDirected = false;
    std::unordered_map<Vertex, std::unordered_map<Vertex, int>> adjList;
    static auto emptyNeighborhood() {
        return std::unordered_map<Vertex, int>();
    }

public:

    explicit Graph(const std::string& s = "undirected") {
        if(s == "directed")
            isDirected = true;
        else if(s != "undirected")
            throw std::invalid_argument("Graph was type not specified.");
    }

    auto addVertex(unsigned int id) {
        Vertex v(id);

        if(adjList.count(v)) { return false; }

        adjList.insert({v, emptyNeighborhood()});
        return true;
    }

    auto addEdge(unsigned int id1, unsigned int id2, const int& w = 0) {
        Vertex v(id1), u(id2);

        if(!adjList.count(v) || !adjList.count(u)) { return false; }

        adjList.at(v).insert({u, w});
        if(!isDirected)
            adjList.at(u).insert({v, w});
        return true;
    }

    auto BFS(unsigned int source) {
        Vertex current(source);

        if(!adjList.count(current)) return;

        std::queue<Vertex> q;
        std::unordered_set<Vertex> visited;

        q.push(current);
        visited.insert(current);
        while(!q.empty()) {
            auto const& neighbors = adjList.at(current);
            for(auto const &n : neighbors) {
                if(!visited.count(n.first)) {
                    q.push(n.first);
                    visited.insert(n.first);
                }
            }
            // visiting
            std::cout << current.getId() << std::endl;

            q.pop();
            current = q.front();
        }
    }

    auto DFS(unsigned int source) {
        static std::unordered_set<Vertex> visited;
        Vertex current(source);

        if(!adjList.count(current)) return;

        // visiting
        std::cout << current.getId() << std::endl;
        visited.insert(current);

        auto neighbors = adjList.at(current);
        for(const auto& n : neighbors) {
            if(!visited.count(n.first))
                DFS(n.first.getId());
        }
    }

    auto printVertices() {
        for(auto const& adj : adjList)
            std::cout << adj.first.getId() << std::endl;
    }

    auto printEdges() {
        for(auto const& adj : adjList) {
            std::cout << adj.first.getId() << ": ";
            for (auto const &n: adj.second)
                std::cout << n.first.getId() << " ";
            std::cout << "\n";
        }
    }
};

int main() {
    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);

    g.addEdge(0, 1);
    g.addEdge(0,2);
    g.addEdge(1,2);
    g.addEdge(2,0);
    g.addEdge(2,3);
    g.addEdge(3,3);

    g.BFS(2);
    std::cout << "\n----\n";
    g.DFS(0);

    return 0;
}