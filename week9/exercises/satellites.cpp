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


// Main
int testcase() {
    int ground, satellites, links;
    std::cin >> ground >> satellites >> links;

	// build Graph
	Graph G(ground + satellites);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	Vertex src = ground + satellites;
	Vertex sink = ground + satellites + 1;

    // from src to ground
    for (int i = 0; i < ground; i++){
        eaG.addEdge(src, i, 1);
    }

    // from satellites to sink
    for (int i = 0; i < satellites; i++){
        eaG.addEdge(i + ground, sink, 1);
    }

    // from ground to satellites
    for (int i = 0; i < links; i++){
        int from, to;
        std::cin >> from >> to;
        eaG.addEdge(from, to + ground, 1);
    }

	// Find a min cut via maxflow
	int flow = boost::push_relabel_max_flow(G, src, sink);

	// BFS to find vertex set S
	std::vector<int> vis(ground + satellites + 2, false); // visited flags
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[src] = true; // Mark the source as visited
	Q.push(src);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}

    std::vector<int> left;
    std::vector<int> right;

    // from ground unvisited
    for (int i = 0; i < ground; i++){
        if (!vis[i])
            left.push_back(i);
    }

    // from satellites visited
    for (int i = 0; i < satellites; i++){
       if(vis[i + ground])
            right.push_back(i);
    }

	std::cout << left.size() <<  " " << right.size() << std::endl;
    for (int i = 0; i < left.size(); i++)
        std::cout << left[i] << " ";
    for (int i = 0; i < right.size(); i++)
        std::cout << right[i] << " ";

    if(!left.empty() || !right.empty())
	    std::cout << std::endl;

	return 0;
}

int main(){
    int n;
    std::cin >> n;
    while(n--){
        testcase();
    }
}