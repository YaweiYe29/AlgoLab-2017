#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};


// Main
void testcase() {
    int buyers, sites, states;
    std::cin >> buyers >> sites >> states;

	// Create Graph and Maps
    Graph G(2 + buyers + sites + states);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

	Vertex src = buyers + sites + states;
	Vertex sink = buyers + sites + states + 1;

    // from src to all buyers
    for (int i = 0; i < buyers; i++){
        eaG.addEdge(src, i, 1, 0);
    }

    // from every state to sink (can be n-edges)
    for (int i = 0; i < states; i++){
        int count;
        std::cin >> count;
        eaG.addEdge(buyers + sites + i, sink, count, 0);
    }

    // from every site to state 
    for (int i = 0; i < sites; i++){
        int site;
        std::cin >> site;
        eaG.addEdge(buyers + i, buyers + sites + site - 1, 1, 0);
    }

    // from every buyer to every state with capacity 1 and cost 100 - bin
    // because 100 is max and we cannot have negative edges in faster algo 
    // so we need to revert them and than revert back.
    for (int i = 0; i < buyers; i++){
        for (int j = 0; j < sites; j++){
            int bid;
            std::cin >> bid;
            eaG.addEdge(i, buyers + j, 1, 100 - bid); // for 80 pts -> (-1) * bid
        }
    }

	// we should use this one if we wanna have 100p
    int flow = boost::push_relabel_max_flow(G, src, sink);
    //boost::cycle_canceling(G); // got just 80 points -> slow
    boost::successive_shortest_path_nonnegative_weights(G, src, sink);
    int cost = boost::find_flow_cost(G);

	std::cout << flow << " " << flow * 100 - cost << std::endl; // for 80pts -> (-1) * cost
}

int main(){
    int n;
    std::cin >> n;
    while(n--){
        testcase();
    }
}