#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

std::set<int> bfs(Graph &G, int start, ResidualCapacityMap &rescapacitymap, EdgeCapacityMap &capacitymap, int n, int * cutPrice) {
    // BFS to find vertex set S
    std::vector<int> vis(n, false); // visited flags
    std::set<int> visited;
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[start] = true; // Mark the source as visited
    visited.insert(start);
	Q.push(start);
    *cutPrice = 0;
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			if (vis[v]) continue;
            if (rescapacitymap[*ebeg] == 0){
                *cutPrice += capacitymap[*ebeg];
                continue;
            }
			vis[v] = true;
            visited.insert(v);
			Q.push(v);
		}
	}
    return visited;
}

// Main
void testcase() {
    int n, m;
    std::cin >> n >> m;

	// Create Graph and Maps
    Graph G(n);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

    // from src to all buyers
    for (int i = 0; i < m; i++){
        int from, to, cap;
        std::cin >> from >> to >> cap;
        eaG.addEdge(from, to, cap);
    }

    std::set<int> finalStatues;
    int cutPrice = 0;
    int minCutPrice = 123456;

    for (int i = 1; i < n; i++){
        int flow = boost::push_relabel_max_flow(G, 0, i);
        auto set = bfs(G, i, rescapacitymap, capacitymap, n, &cutPrice);

        if(minCutPrice > cutPrice) {
            minCutPrice = cutPrice;
            finalStatues = set;
            for(auto it = finalStatues.begin(); it != finalStatues.end(); ++it){
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        }
    }

    for (int i = 1; i < n; i++){
        int flow = boost::push_relabel_max_flow(G, i, 0);
        auto set = bfs(G, i, rescapacitymap, capacitymap, n, &cutPrice);

        if(minCutPrice > cutPrice) {
            minCutPrice = cutPrice;
            finalStatues = set;
            for(auto it = finalStatues.begin(); it != finalStatues.end(); ++it){
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        }
    }

	// we should use this one if we wanna have 100p
    std::cout << cutPrice << std::endl << finalStatues.size() << " ";
    for(auto it = finalStatues.begin(); it != finalStatues.end(); ++it){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int main(){
    int n;
    std::cin >> n;
    while(n--){
        testcase();
    }
}