// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
// Namespaces
// using namespace std;
using namespace boost;

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

//Undirected graph with integer weights on edges and parallel edges. 
typedef adjacency_list<vecS, vecS, directedS,
allow_parallel_edge_tag,
property<edge_weight_t, int> > Graph2;
typedef graph_traits<Graph2>::vertex_descriptor Vertex2;      //Vertex type 
typedef graph_traits<Graph2>::edge_descriptor Edge2;          //Edgetype
typedef graph_traits<Graph2>::edge_iterator EdgeIt;          // Edge iterator 
typedef property_map<Graph2,edge_weight_t>::type WeightMap;  //Property map edge->weight


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

// Functions
// =========
// Function for an individual testcase
void testcases() {
    int n, m, start, end, from, to, length, capacity;
    std::cin >> n >> m >> start >> end;
	// Create Graph and Maps
    Graph Gfinal;

	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, Gfinal);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, Gfinal);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, Gfinal);
	EdgeAdder eaG(Gfinal, capacitymap, revedgemap);
    
    Graph2 Gtmp(n);
	Graph2 Gtmp2(n);
    WeightMap weightmap = boost::get(edge_weight, Gtmp);
	WeightMap weightmap2 = boost::get(edge_weight, Gtmp2);
	std::map<Edge2, int> capMap;

    for(int i = 0; i < m; i++){
		std::cin >> from >> to >> capacity >> length;
		std::cout << "adding edge from " << from << " to " << to << std::endl;
		bool success;
		Edge2 e1, e2;
		boost::tie(e1, success) = boost::add_edge(from, to, Gtmp);	
		boost::tie(e2, success) = boost::add_edge(to, from, Gtmp2);
		weightmap[e1] = length;	
		weightmap2[e2] = length;	
		capMap[e1] = capacity;
	}

   	// Dijkstra shortest paths
	// =======================
	std::vector<int> distmap(n);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	dijkstra_shortest_paths(Gtmp, start, // We MUST provide at least one of the two maps
		distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, Gtmp))));	// distance map as Named Parameter

	std::vector<int> distmap2(n);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	dijkstra_shortest_paths(Gtmp2, end, // We MUST provide at least one of the two maps
		distance_map(make_iterator_property_map(distmap2.begin(), get(vertex_index, Gtmp2))));	// distance map as Named Parameter

	assert(distmap[end] == distmap2[start]);
	std::cout << "min distance: " << distmap[end] << std::endl;
	EdgeIt ebeg, eend;	
	for (tie(ebeg, eend) = edges(Gtmp); ebeg != eend; ++ebeg) {	// edges(G) returns a pair of iterators which define a range of all edges. 
		// For undirected graphs, each edge is visited once, with some orientation.
		// ebeg is EdgeIterator, *ebeg is EdgeDescriptor
		Vertex2 u = source(*ebeg, Gtmp);
		Vertex2 v = target(*ebeg, Gtmp);

		int cap = capMap[*ebeg];
		int len = weightmap[*ebeg];
		if(len + distmap[u] + distmap2[v] == distmap[end] || len + distmap[u] + distmap2[v] == distmap[end]){
			std::cout << "adding edge from " << u << " to " << v << std::endl;
			eaG.addEdge(u, v, cap);
		}
	}

	// Calculate flow
	// If not called otherwise, the flow algorithm uses the interior properties
	// - edge_capacity, edge_reverse (read access),
	// - edge_residual_capacity (read and write access).
    long flow = boost::push_relabel_max_flow(Gfinal, start, end);

    std::cout << flow << std::endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
    int n;	
    std::cin >> n;
	while(n--){
        testcases();
    }
	return 0;
}
