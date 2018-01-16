// STL includes
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp> 
#include <boost/graph/max_cardinality_matching.hpp>

// BGL Graph definitions
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::allow_parallel_edge_tag, boost::property<boost::edge_weight_t, int> > Graph;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::edge_weight_t, int> > UGraph;

typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t
typedef boost::graph_traits<UGraph>::vertex_descriptor		Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;

void addEdge(int from, int to, int w, WeightMap &weightmap, Graph &G){
    Edge e;	bool success;
	boost::tie(e, success) = boost::add_edge(from, to, G);
	weightmap[e] = w;
}

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

    // Create Graph and Maps
    Graph G(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);

    for (int i = 0; i < m; i++){
        int x, y, z;
        char w;
        std::cin >> w >> x >> y >> z;
        addEdge(x, y, z, weightmap, G);
        if(w == 'L')
            addEdge(y, x, z, weightmap, G);
    }

    std::vector<std::vector<int>> dist(a, std::vector<int>(n));
    for(int i = 0; i < a; i++){
        int start;
        std::cin >> start;
        boost::dijkstra_shortest_paths(G, start, boost::distance_map(boost::make_iterator_property_map(dist[i].begin(), boost::get(boost::vertex_index, G))));
    }

    std::vector<std::vector<int>> graph(a, std::vector<int>(s));
    for(int i = 0; i < s; i++){
        int shelter;
        std::cin >> shelter;
        for(int j = 0; j < a; j++){
            //std::cout << "a: " << j << " s: " << i << " dist: " << dist[j][shelter] << std::endl;
            graph[j][i] = dist[j][shelter];
        }
    }


    int left = 0, right = INT32_MAX, middle;
    while(left < right){
        middle = left + (right - left) / 2;
        UGraph uG(a + c * s);
        for(int i = 0; i < a; i++){
            for(int j = 0; j < s; j++){
                if(graph[i][j] == INT32_MAX)
                    continue;
                for(int k = 1; k <= c; k++){
                    if(graph[i][j] + k * d <= middle){
                        boost::add_edge(i, a + ((k-1) * s) + j, uG);
                    }
                }
            }
        }
        std::vector<Vertex> matemap(a + c * s);
	    boost::edmonds_maximum_cardinality_matching(uG, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, uG)));
	    int agents = 0;
        const Vertex NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();
        for (int i = 0; i < a; i++) { // Count the number of matched agents.
            agents += (matemap[i] != NULL_VERTEX);
        }
        if(agents == a){
            right = middle;
        } else {
            left =  middle + 1;
        }
    }
    std::cout << left << std::endl;
}

int main(){
    int t;
    std::cin >> t;
    while(t--){
        testcase();
    }
}