// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
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
        std::cout << "Adding edge from " << u << " to " << v << " with cap " << c << " and cost " << w << std::endl;
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

void testcase() {
    /*
    – n, the number of relevant positions p0 , . . . , pn−1 around Schilthorn (1 ≤ n ≤ 103 ).
    – m, the number of slopes and ski lifts around Schilthorn (0 ≤ m ≤ 5 · 103).
    – a, the number of secret agents (1 ≤ a ≤ 102).
    – s, the number of shelters (1 ≤ s ≤ 102).
    – c, the capacity of each shelter (1 ≤ c ≤ 2).
    – d, the time (in seconds) an agent needs to perform the entering protocol at a shelter (1 ≤ d ≤ 103).
    */  
    int n, m, a, s, c, d;
    std::cin >> n >> m >> a >> s >> c >> d;

    int N = n + 3;
    // Create Graph and Maps
    Graph G(N);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    int source = n;
    int gate = n + 1;
    int sink = n + 2;

    for (int i = 0; i < m; i++){
        int x, y, z;
        char w;
        std::cin >> w >> x >> y >> z;
        eaG.addEdge(x, y, INT32_MAX, -z);
        if(w == 'L')
            eaG.addEdge(y, x, INT32_MAX, -z);
    }
    
    for(int i = 0; i < a; i++){
        int start;
        std::cin >> start;
        eaG.addEdge(source, start, 1, 0);
    }

    for(int i = 0; i < s; i++){
        int shelter;
        std::cin >> shelter;
        eaG.addEdge(shelter, gate, c, -d);
    }

    eaG.addEdge(gate, sink, 1, 0);

    int flow = boost::push_relabel_max_flow(G, source, sink);
     boost::cycle_canceling(G);
    int cost = boost::find_flow_cost(G);
    std::cout << cost << std::endl;
}

int main(){
    int t;
    std::cin >> t;
    while(t--){
        testcase();
    }
}