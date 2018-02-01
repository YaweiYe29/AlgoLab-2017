// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

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
        //std::cout << "adding edge from " << from << " to " << to << " with cap " << capacity << std::endl;
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
    int Z, J;
    std::cin >> Z >> J;
	// Create Graph and Maps
	Graph G(Z + J + 2);
    Vertex source = Z + J;
	Vertex target = Z + J + 1;
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

    std::vector<int> jobs(J), zones(Z);
    for(int i = 0; i < Z; i++){
        std::cin >> zones[i];
        eaG.addEdge(source, i, zones[i]);
    }
    int profit = 0;
    for(int i = 0; i < J; i++){
        std::cin >> jobs[i];
        eaG.addEdge(Z+i, target, jobs[i]);
        profit += jobs[i];
    }
    for(int i = 0; i < J; i++){
        int n;
        std::cin >> n;
        for(int j = 0; j < n; j++){
            int zone;
            std::cin >> zone;
            eaG.addEdge(zone, Z+i, zones[zone]);
        }
    }

	// Calculate flow
	//long flow1 = boost::push_relabel_max_flow(G, source, target);
	long flow = boost::edmonds_karp_max_flow(G, source, target);
    std::cout << profit - flow << std::endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	
    std::cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}