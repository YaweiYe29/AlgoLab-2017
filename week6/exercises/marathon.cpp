// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <algorithm>
#include <climits>
#include <cassert>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph2;
typedef boost::graph_traits<Graph2>::edge_descriptor		Edge2;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph2>::vertex_descriptor		Vertex2;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph2>::edge_iterator		EdgeIt2;		// to iterate over all edges
typedef boost::graph_traits<Graph2>::out_edge_iterator		OutEdgeIt2;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph2, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t


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
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;

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
        //std::cout << "adding edge from: " << from << " to: " << to << " capacity: " << capacity << std::endl;
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void testcase() {
    int n, m, start, end, from, to, length, capacity;
    std::cin >> n >> m >> start >> end;

    Graph Gfinal;
    Graph2 G(n);
    WeightMap weightmap = boost::get(boost::edge_weight, G);

	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, Gfinal);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, Gfinal);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, Gfinal);
	EdgeAdder eaG(Gfinal, capacitymap, revedgemap);

    std::map<Edge2, int> capacities;
    for(int i = 0; i < m; i++){
		std::cin >> from >> to >> capacity >> length;
		bool success; Edge2 e;
		boost::tie(e, success) = boost::add_edge(from, to, G);	
        //std::cout << "Adding edge from " << from << " to " << to << " with length " << length << std::endl;
		weightmap[e] = length;
        capacities[e] = capacity;
	}

   	// Dijkstra shortest paths
	// =======================
	std::vector<int> distmap(n);		
	boost::dijkstra_shortest_paths(G, start, 
		boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));

	std::vector<int> distmap2(n);
	dijkstra_shortest_paths(G, end,
		boost::distance_map(boost::make_iterator_property_map(distmap2.begin(), boost::get(boost::vertex_index, G))));

	assert(distmap[end] == distmap2[start]);
    if(distmap[end] == 0 || distmap[end] == INT32_MAX){
        std::cout << 0 << std::endl;
        return;
    }

	EdgeIt2 ebeg, eend;	
	for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
		Vertex u = source(*ebeg, G);
		Vertex v = target(*ebeg, G);

		int cap = capacities[*ebeg];
		int len = weightmap[*ebeg];
		if(len + distmap[u] + distmap2[v] == distmap[end]){
		    eaG.addEdge(u, v, cap);
		} else if (len + distmap2[u] + distmap[v] == distmap[end]){
            eaG.addEdge(v, u, cap);
        }
	}

	// Calculate flow
    long flow = boost::push_relabel_max_flow(Gfinal, start, end);
    std::cout << flow << std::endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
    int n;	
    std::cin >> n;
	while(n--){
        testcase();
    }
	return 0;
}
