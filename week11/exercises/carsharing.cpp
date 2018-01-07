#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
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
        //std::cout << "adding edge from " << u << " to " << v << " with cap " << c << " and cost " << w << std::endl;
        assert(w >= 0);
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
    // n -> number of car requests, s -> car rentals
    int n, s, idx = 0;
    const int max_p = 100;
    const int MAXF = 10101;
    std::cin >> n >> s;

	// Create Graph and Maps
    Graph G;
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

	Vertex src =  idx++;    // source 0
	Vertex sink =  idx++;   // sink 1

    // load initial car distribution
    std::vector<int> sn(s);
    for (int i = 0; i < s; i++){
        std::cin >> sn[i];
    }


    int maxEnd = 0;                                             // maxEnd is maximum time from all car requests. It is used to balance paths in graph
    std::vector<std::map<int,int>> ids(s);                      // mapping from (rental station, time) -> id
    std::map<std::pair<int,int>, int> dur_map;                  // mapping from (id, id) -> duration, where ids are from ids structure
    std::vector<std::set<int> > intervals(s, std::set<int>());  // time points for each rental station
    for(int i = 0; i < n; i++){                                 // go for each car request
        int from, to, start, end, profit;                       // load data
        std::cin >> from >> to >> start >> end >> profit;
        from--; to--;                                           // we are 0 based so lower rental station id by one
        if(ids[from][start] == 0)                               // create alias for pair of reantal station and time (vertex id) if does not exist yet
            ids[from][start] = idx++;
        int u = ids[from][start];
        if(ids[to][end] == 0)
            ids[to][end] = idx++;
        int v = ids[to][end];
        int duration = end-start;                               // compute duration between two times
        eaG.addEdge(u, v, 1, max_p*duration-profit);            // add edge between two aliases with capacity one and cost maximum profit * duration - profit
        dur_map[std::make_pair(u, v)] = duration;               // add edge to map
        intervals[from].insert(start);                          // add both times to appropriate intervals
        intervals[to].insert(end);
        maxEnd = std::max(maxEnd, std::max(start, end));        // find max time
    }

    int prev;
    for(int i = 0; i < s; i++){
        prev = -1;
        eaG.addEdge(src, ids[i][*intervals[i].begin()], sn[i], 0);  //add edge from src to lowest time with capacity of according rental station and cost 0
        eaG.addEdge(ids[i][*intervals[i].rbegin()], sink, MAXF, (maxEnd - *intervals[i].rbegin()) * max_p);
                                                                    //add edge from biggest time of rental station to sink with infinite cap and duration from
                                                                    // time to biggest time so all rentals end in the same biggest time
        for(auto it = intervals[i].begin(); it != intervals[i].end(); ++it){
            if(prev != -1){
                int duration = *it-prev;;
                eaG.addEdge(ids[i][prev], ids[i][*it], MAXF, max_p*duration);
                                                                    // connect two following times with max time so they will not be prioritized and save duration
                dur_map[std::make_pair(ids[i][prev], ids[i][*it])] = duration;
            }
            prev = *it;
        }
    }

    int flow = boost::push_relabel_max_flow(G, src, sink);          // find flow || flow == sum(sn)
    boost::successive_shortest_path_nonnegative_weights(G, src, sink); // compute costs
    long cost = 0;
	EdgeIt e, e_end;
    // we need to find cost by our selves because we need to ignore costs of edges added by us and revert cost back 
	for(tie(e, e_end) = boost::edges(G); e != e_end; ++e) {
		long flow = capacitymap[*e] - rescapacitymap[*e]; 
        int s = boost::source(*e,G);
        int t = boost::target(*e,G);
        int duration = dur_map[std::make_pair(s, t)];
		if(flow == 1 && s != src && t != sink) {
			cost += weightmap[*e] - max_p*duration;
		}
	}

	std::cout << -cost << std::endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    std::cin >> n;
    while(n--){
        testcase();
    }
}