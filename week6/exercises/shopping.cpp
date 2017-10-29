// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

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

bool isShop(int v, std::set<int> shops){
    if(shops.find(v) == shops.end())
        return false;
    return true;
}

// Functions
// =========
// Function for an individual testcase
void testcases() {
    int n, m, s, shop, a, b;
    std::cin >> n >> m >> s;
	// Create Graph and Maps
	Graph G(n);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
    
    std::set<int> shops;
    std::map<int, bool> added_shops;

    for(int i = 0; i < s; i++){
        std::cin >> shop;
        shops.insert(shop);
        added_shops[shop] = false;
    }

    assert(!isShop(0, shops));

    // the easy solution is to duplicate vertexes and edges(one is for going to the shop and second one is for going from the shop).
    // all edges have capacity of 1
    // the only connections between those two graphs are between mirrored vertexes of shops
    // max flow than indicates n of possibilities to go to the shop and back.
    // if max flow is greater or equal n of shops that there exists such a scenario, otherwise not.
    for(int i = 0; i < m; i++){
        std::cin >> a >> b;

        if(isShop(a, shops) && !added_shops[a]){
            eaG.addEdge(a, a+n, 1);
            eaG.addEdge(a+n, a, 1);
            added_shops[a] = true;
        }
        if(isShop(b, shops) && !added_shops[b]){
            eaG.addEdge(b, b+n, 1);
            eaG.addEdge(b+n, b, 1);
            added_shops[b] = true;
        }
        eaG.addEdge(a, b, 1);
        eaG.addEdge(b+n, a+n, 1);
        eaG.addEdge(b, a, 1);
        eaG.addEdge(a+n, b+n, 1);
    }

	// Calculate flow
	// If not called otherwise, the flow algorithm uses the interior properties
	// - edge_capacity, edge_reverse (read access),
	// - edge_residual_capacity (read and write access).
    long flow = boost::push_relabel_max_flow(G, 0, n);

    // if there are more ways it is still fine
    if(flow >= s)
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
