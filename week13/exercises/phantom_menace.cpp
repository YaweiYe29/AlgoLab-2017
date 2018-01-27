#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

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
/* Problem analysis:
 * We need to find minimal set of planets so if we block them princess can not go from start nodes to end ones (start and end nodes can be blocked).
 * In other words we are searching for min vertex cut. In lecture we had min edge cut. Transformation to is however simple.
 * We need to duplicate every vertex i.e. split him to in vertex and out vertex and connect them with cap of 1. Other connections can have cap 1 or infinite
 * it does not matter since these edge will become bottlenecks. Since we want to know just number of planets we output flow.
 */
// Main
void testcase() {
    int n, m, s, d;
    std::cin >> n >> m >> s >> d;
	Graph G(2*n + 2);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	Vertex src = 2*n;
	Vertex sink = 2*n+1;

    for(int i = 0; i < m; i++){
        int from, to;
        std::cin >> from >> to;
        eaG.addEdge(from + n, to, 1);
    }

    for(int i = 0; i < s; i++){
        int ss;
        std::cin >> ss;
        eaG.addEdge(src, ss, 1);
    }

    for(int i = 0; i < d; i++){
        int dd;
        std::cin >> dd;
        eaG.addEdge(dd + n, sink, 1);
    }

    for(int i = 0; i < n; i++){         // duplicate edges and connect them with cap of 1
        eaG.addEdge(i, i + n, 1);
    }

	// Find a min cut via maxflow
	int flow = boost::push_relabel_max_flow(G, src, sink);
    std::cout << flow << std::endl;
}

int main(){
    int t;
    std::cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}