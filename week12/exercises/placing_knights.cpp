// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

void addEdge(int x1, int y1, int x2, int y2, int n, WeightMap &weightmap, Graph &G, std::vector<std::vector<bool>> &chessboard){
    if(x1 >= 0 && y1 >= 0 && x2 >= 0 && y2 >= 0 && x1 < n && x2 < n && y1 < n && y2 < n && chessboard[x1][y1] && chessboard[x2][y2]){
        Edge e;	bool success;
		boost::tie(e, success) = boost::add_edge(x1*n + y1, x2*n + y2, G);
		weightmap[e] = 1;
    }
}

void testcase() {
	int n;
    std::cin >> n;
    std::vector<std::vector<bool>> chessboard(n, std::vector<bool>(n));
    int validPlaces = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int in;
            std::cin >> in;
            chessboard[i][j] = in == 1 ? true : false;
            validPlaces += chessboard[i][j] ? 1 : 0;
        }
    }

	Graph G(n*n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            //[i − 1, j − 2], [i − 1, j + 2], [i + 1, j − 2], [i + 1, j + 2], [i − 2, j − 1], [i − 2, j + 1], [i + 2, j − 1], [i + 2, j + 1]
            addEdge(i, j, i - 1, j - 2, n, weightmap, G, chessboard);
            addEdge(i, j, i - 1, j + 2, n, weightmap, G, chessboard);
            addEdge(i, j, i + 1, j - 2, n, weightmap, G, chessboard);
            addEdge(i, j, i + 1, j + 2, n, weightmap, G, chessboard);
            addEdge(i, j, i - 2, j - 1, n, weightmap, G, chessboard);
            addEdge(i, j, i - 2, j + 1, n, weightmap, G, chessboard);
            addEdge(i, j, i + 2, j - 1, n, weightmap, G, chessboard);
            addEdge(i, j, i + 2, j + 1, n, weightmap, G, chessboard);
        }
	}

	// Edmonds' maximum cardinality matching
	std::vector<Vertex> matemap(n*n);
	boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));
	int matchingsize = matching_size(G, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));
    std::cout << validPlaces - matchingsize << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t;	
    std::cin >> t;
	while(t--)	
        testcase();
	return 0;
}
