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
typedef boost::graph_traits<Graph>::edge_iterator              EdgeIt;

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
        //std::cout << "adding edge from " << u << " to " << v << " cap " << c << " and cost " << w << std::endl;
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
    const int MAX_PRIORITY = 128;
    int n, m, l;
    std::cin >> n >> m >> l;
    // Create Graph and Maps
    Graph G(n + 2);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    Vertex src = n;
    Vertex sink = n + 1;
    eaG.addEdge(src, 0, l, 0);
    eaG.addEdge(n - 1, sink, l, 0);

    for(int i = 1; i < n; i++){
        eaG.addEdge(i - 1, i, l, MAX_PRIORITY);
    }

    for(int i = 0; i < m; i++){
        int from, to, priority;
        std::cin >> from >> to >> priority;
        int dist = to - from;
        eaG.addEdge(from, to, 1, dist * MAX_PRIORITY - priority);
    }

    // Option 1: Min Cost Max Flow with cycle_canceling
    int flow = boost::push_relabel_max_flow(G, src, sink);
    boost::successive_shortest_path_nonnegative_weights(G, src, sink);
    int missions = 0;
    EdgeIt e, eend;
    for(boost::tie(e, eend) = boost::edges(G); e != eend; ++e) {
        int flow = capacitymap[*e] - rescapacitymap[*e];
        int weight = weightmap[*e];
        int s = boost::source(*e,G);
        int t = boost::target(*e,G);
        if(flow == 1 && weight != MAX_PRIORITY && s != src && t != sink){
            int priority = (t - s) * MAX_PRIORITY - weightmap[*e];
            missions += priority;
            //std::cout << "from: " << s << " to: " << t << " priority " << priority << std::endl;
        }
    }
    std::cout << missions << std::endl;
}

int main(){
    int t;
    std::cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}