// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
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
    int n, m, a, b, c, game, player, sum = 0;
    std::cin >> n >> m;
    game = n + 2;
	// Create Graph and Maps
	Graph G(n);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

    // start and end vertexes
    Vertex source = boost::add_vertex(G);
    Vertex target = boost::add_vertex(G);
    
    for(int i = 0; i < m; i++){
        std::cin >> a >> b >> c;
        // connect start and each game vertex
        eaG.addEdge(source, game, 1);

        //if game won first or not known add vertex from game to a
        if(c < 2)
            eaG.addEdge(game, a, 1);

        //if game won second or not known add vertex from game to b
        if(c == 2 || c == 0)
            eaG.addEdge(game, b, 1);

        game++;
    }

    for(int i = 0; i < n; i++){
        std::cin >> player;
        sum += player;
        eaG.addEdge(i, target, player);
    }

	// Calculate flow
	// If not called otherwise, the flow algorithm uses the interior properties
	// - edge_capacity, edge_reverse (read access),
	// - edge_residual_capacity (read and write access).
    long flow = boost::push_relabel_max_flow(G, source, target);
    
    // they can give you final result that is wrong because it is not equal m
    if(flow == std::max(sum, m))
        std::cout << "yes";
    else
        std::cout << "no";

    std::cout << std::endl;
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
