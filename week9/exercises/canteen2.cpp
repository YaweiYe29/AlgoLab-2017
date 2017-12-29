#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <utility>
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
        //std::cout << "adding edge from " << u << " to " << v << " cap: " << c << " cost: " << w << std::endl;
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
    int n, students = 0;
    std::cin >> n;

	// Create Graph and Maps
    Graph G(2 * n + 1);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

	Vertex src = 2 * n;
	Vertex sink = 2 * n + 1;

    // from src to all buyers
    for (int i = 0; i < n; i++){
        int day, cost;
        std::cin >> day >> cost;
        eaG.addEdge(src, i, day, cost);
    }

    // from every state to sink (can be n-edges)
    for (int i = 0; i < n; i++){
        int stud, cost;
        std::cin >> stud >> cost;
        students += stud;
        eaG.addEdge(i, sink, stud, 20 - cost);
    }

    // from every site to state 
    for (int i = 0; i < n - 1; i++){
        int volume, cost;
        std::cin >> volume >> cost;
        eaG.addEdge(i, i + 1, volume, cost);
    }

	// we should use this one if we wanna have 100p
    int flow = boost::push_relabel_max_flow(G, src, sink);
    //boost::cycle_canceling(G);
    boost::successive_shortest_path_nonnegative_weights(G, src, sink);
    int cost = boost::find_flow_cost(G);

    OutEdgeIt e, eend;
    int income = 0;
    int oldIncome = 0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(sink,G), G); e != eend; ++e) {
        int tmp_i = rescapacitymap[*e] - capacitymap[*e];
        income += tmp_i * (20 + weightmap[*e]);
        oldIncome += tmp_i * (-1) * weightmap[*e];
    }

    if(flow < students)
        std::cout << "impossible ";
    else
        std::cout << "possible ";
	std::cout << flow << " " << income - (cost - oldIncome) << std::endl; 
}

int main(){
    int n;
    std::cin >> n;
    while(n--){
        testcase();
    }
}