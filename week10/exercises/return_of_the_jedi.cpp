/* Solution worth just 40 points because its slow. Check return_of_the_jedi2.cpp for full points*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <stack>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

typedef struct MyEdge{
    int from, to, weight;
} MyEdge;

// finds max edge between from and to vertices of G that is MST of some other graph
int findMaxWeightInCycle(int from, int to, Graph & G, int n, WeightMap& weightmap){
    std::vector<int> predecessors(n);
    std::vector<bool> visited(n, false);
    std::stack<int> dfs;
    dfs.push(from);
    predecessors[from] = -1;
    visited[from] = true;
    // classical dfs
    while(!dfs.empty()){
        int curr = dfs.top();
        dfs.pop();
        OutEdgeIt oebeg, oeend;
        for (boost::tie(oebeg, oeend) = boost::out_edges(curr, G); oebeg != oeend; ++oebeg) {
			Vertex v = boost::target(*oebeg, G);
            if(visited[v])
                continue;
            predecessors[v] = curr;
            visited[v] = true;
            dfs.push(v);
            // if we reached "to" vertex than find max of edges that led to it from "from" vertex
            if(v == to){
                int max = 0;
                int i = 0;
                while(predecessors[v] != -1){
                    Edge e; bool success;
			        boost::tie(e, success) = boost::edge(predecessors[v], v, G);
                    int w = weightmap[e];
                    if(w > max)
                        max = w;
                    v = predecessors[v];
                    if(i++ > 10)
                        break;
                }
                return max;
            }
        }
    }
    return -1;
}

void testcase() {
    int n, tatooine, weight = 0;
    std::cin >> n >> tatooine;
    Graph G(n);
    WeightMap weightmap = boost::get(boost::edge_weight, G);

    // create graph from input
    for (int i = 0; i < n - 1; ++i) {
        for(int j = i + 1; j < n; ++j){
            int w;
            std::cin >> w;
            Edge e;	bool success;
            boost::tie(e, success) = boost::add_edge(i, j, G);
            weightmap[e] = w;
        }
    }

    // Prim minimum spanning tree starting from tatooine
	std::vector<Vertex> primpredmap(n);
	boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)),
			boost::root_vertex(tatooine - 1));

	// iterate over all vertices and create new graph from the ones that form MST
    // and push other to the nonMST vector
    Graph G2(n);
    WeightMap weightmap2 = boost::get(boost::edge_weight, G2);
    std::vector<std::vector<bool> > added = std::vector<std::vector<bool> >(n, std::vector<bool>(n, false));
    std::vector<std::pair<int,int> > nonMST;
	for (int i = 0; i < n; ++i) {
		Vertex u = i;
		OutEdgeIt oebeg, oeend;
		for (boost::tie(oebeg, oeend) = boost::out_edges(u, G); oebeg != oeend; ++oebeg) {
			Vertex v = boost::target(*oebeg, G);
			if (primpredmap[u] == v) {
                weight += weightmap[*oebeg];
                Edge e;	bool success;
                boost::tie(e, success) = boost::add_edge(u, v, G2);
                weightmap2[e] = weightmap[*oebeg];
			} else if (primpredmap[v] != u){
                if(!added[u][v] && !added[v][u]){
                    added[u][v] = true;
                    nonMST.push_back(std::pair<int,int>(u, v));
                }
            }
		}
	}
	// find replacemenet of min cost
    int min = INT32_MAX;
    for (auto it = nonMST.begin(); it != nonMST.end(); ++it){
        Edge e;	bool success;
        boost::tie(e, success) = boost::edge(it->first, it->second, G);
        int w = weightmap[e];
        int w2 = w - findMaxWeightInCycle(it->first, it->second, G2, n, weightmap2);
        if(w2 < min)
            min = w2;
    }
    // result is weight of MST plus min cost
    std::cout << weight + min << std::endl;
}

int main(){
    int t;
    std::cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}