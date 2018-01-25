#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

int getIdx(int i, int j, int k){
    int res = i*(i+1) + j * 2;    
    return res;
}

void addEdge(int from, int to, int w, Graph &G, WeightMap & wm){
    //std::cerr << "Adding edge from " << from << " to " << to << " with weight " << w << std::endl;
    bool success; Edge e;
    tie(e, success) = boost::add_edge(from, to, G);
    wm[e] = w;
}

bool canAdd(int i, int j, int k){
    return i >= 0 && j >= 0 && i < k && j <= i ;
}

void tryAdd(int out, int i, int j, int k, Graph &G, WeightMap & wm){
    if(canAdd(i, j, k)){
        int target = getIdx(i, j, k);
        addEdge(out, target, 0, G, wm);
    }
}

void testcase() {
    int k;
    std::cin >> k;
    int V = k*(k+1);
    //std::cerr << "vertices: " << V << std::endl;
	Graph G(V);
	WeightMap weightmap = boost::get(boost::edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38

    std::vector<std::vector<int>> presents(k);
    std::vector<int> iterables;
    for(int i = 0; i < k; i++){
        for(int j = 0; j < i + 1; j++){
            int p;
            std::cin >> p;
            presents[i].push_back(p);
        }
    }

    int idx = 3;
    for(int i = 0; i < k; i++){
        for(int j = 0; j < i + 1; j++){
            int p = presents[i][j];
            int in = getIdx(i, j, k);
            int out = in + 1;
            iterables.push_back(in);
            addEdge(in, out, p, G, weightmap);
            tryAdd(out, i, j - 1, k, G, weightmap);
            tryAdd(out, i, j + 1, k, G, weightmap);
            tryAdd(out, i - 1, j - 1, k, G, weightmap);
            tryAdd(out, i - 1, j, k, G, weightmap);
            tryAdd(out, i + 1, j + 1, k, G, weightmap);
            tryAdd(out, i + 1, j, k, G, weightmap);
        }
    }
	std::vector<std::vector<int>> distmap(3, std::vector<int>(V));
	boost::dijkstra_shortest_paths(G, getIdx(0,0,k),
		distance_map(boost::make_iterator_property_map(distmap[0].begin(), boost::get(boost::vertex_index, G))));
	boost::dijkstra_shortest_paths(G, getIdx(k-1,0,k),
		distance_map(boost::make_iterator_property_map(distmap[1].begin(), boost::get(boost::vertex_index, G))));
	boost::dijkstra_shortest_paths(G, getIdx(k-1,k-1,k),
		distance_map(boost::make_iterator_property_map(distmap[2].begin(), boost::get(boost::vertex_index, G))));
    int minDistance = INT32_MAX;
    for(auto it = iterables.begin(); it != iterables.end(); ++it){
        //std::cout << *it << " -> " << distmap[0][*it] << " " << distmap[1][*it] << " " << distmap[2][*it+1] << std::endl;
        int dist = distmap[0][*it] + distmap[1][*it] + distmap[2][*it+1];
        minDistance = std::min(dist, minDistance);
    }
    std::cout << minDistance << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t;
    std::cin >> t;
	while(t--) {
        testcase();
    }
	return 0;
}
