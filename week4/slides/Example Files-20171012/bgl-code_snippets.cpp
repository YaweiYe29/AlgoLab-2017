// ALGOLAB BGL Tutorial 1
// Code snippets demonstrating 
// - graph definitions
// - several algorithms (components, distance-based algorithms, maximum matching)
// - how to pass exterior property maps
// - use of iterators

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets

// Includes
// ========
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
// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		no_property,				// interior properties of vertices	
		property<edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t


// Functions
// ========= 
void testcases() {
	// Create Graph, Vertices and Edges
	// ================================
	int V = 7;
	Graph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = get(edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38
	vector<vector<int> > graphedges = {{0,2,14},{0,3,12},{1,2,19},{1,3,15},{2,3,13},{4,5,17},{4,6,16},{5,6,18}};
	int E = graphedges.size();
	for (int i = 0; i < E; ++i) {
		Edge e;	bool success;
		Vertex u = graphedges[i][0];
		Vertex v = graphedges[i][1];
		int w = graphedges[i][2];
		tie(e, success) = add_edge(u, v, G);	// Adds edge from u to v. If parallel edges are allowed, success is always true.
							// Caveat: if u or v don't exist in G, G is automatically extended!
		weightmap[e] = w;			// Otherwise it is false in case of failure when the edge is a duplicate
		assert(source(e, G) == u && target(e, G) == v);	// This shows how to get the vertices of an edge
	}
	cout << endl;


	// Connected components
	// ====================
	vector<int> componentmap(V);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	int ncc = connected_components(G, make_iterator_property_map(componentmap.begin(), get(vertex_index, G))); 
	cout << "Connected components: " << ncc << "\n";
	vector<int> componentsize(ncc);
	// Iterate over all vertices
	for (int i = 0; i < V; ++i)
		++componentsize[componentmap[i]];
	for (int i = 0; i < ncc; ++i)
		cout << "Component " << i << " has size " << componentsize[i] << ".\n";
	cout << endl;


	// Dijkstra shortest paths
	// =======================
	vector<Vertex> predmap(V);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	vector<int> distmap(V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	Vertex start = 0;
	dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
		predecessor_map(make_iterator_property_map(predmap.begin(), get(vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));	// distance map as Named Parameter
	int maxdist = 0;
	Vertex furthest = start;
	int reachable = 0;
	for (int i = 0; i < V; ++i) {
		if (distmap[i] < INT_MAX) {	// check whether vertex i is reachable, i.e. has "finite" distance from the start
			++reachable;		// INT_MAX comes from climits and the type of distmap (vector<int>).
			if (distmap[i] > maxdist) {
				maxdist = distmap[i];
				furthest = i;
			}
		}
	}
	cout << "Dijkstra starting in node " << start << " reaches " << reachable << " out of " << V << " vertices.\n";
	cout << "The furthest reachable vertex is " << furthest << " with distance " << maxdist << ".\n";
	cout << "Edges in Dijkstra's shortest paths tree:\n";
	// EdgeIterators
	// =============
	EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {	// edges(G) returns a pair of iterators which define a range of all edges. 
		// For undirected graphs, each edge is visited once, with some orientation.
		// ebeg is EdgeIterator, *ebeg is EdgeDescriptor
		Vertex u = source(*ebeg, G);
		Vertex v = target(*ebeg, G);
		if (predmap[u] == v || predmap[v] == u)
			cout << u << " -- " << v << " (weight " << weightmap[*ebeg] << ")\n";

	}
	cout << "List of disconnected single nodes: ";
	for (int i = 0; i < V; ++i)
		if (predmap[i] == i && i != start)
			cout << i << " ";
	cout << "\n" << endl;
	

	
	// Prim minimum spanning tree
	// ==========================
	vector<Vertex> primpredmap(V);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
	start = 6;
	prim_minimum_spanning_tree(G, make_iterator_property_map(primpredmap.begin(), get(vertex_index, G)));	// Prim from *vertices(G).first (usually 0)
	prim_minimum_spanning_tree(G, make_iterator_property_map(primpredmap.begin(), get(vertex_index, G)),	// Prim from user-defined start,
			root_vertex(start));									// old primpredmap gets reset first!	
	cout << "Prim builds a minimum spanning tree (of node " << start << "'s component) of total weight: ";
	int totalweight = 0;
	for (int i = 0; i < V; ++i) {
		if (primpredmap[i] != i) {
			Edge e; bool success;
			tie(e, success) = edge(i, primpredmap[i], G);
			totalweight += weightmap[e];
		}
	}
	cout << totalweight << "\n";
	cout << "Edges in Prim's minimum spanning:\n";	
	// iterate over all vertices
	for (int i = 0; i < V; ++i) {
		// OutEdgeIterators
		// ================
		Vertex u = i;
		OutEdgeIt oebeg, oeend;
		for (tie(oebeg, oeend) = out_edges(u, G); oebeg != oeend; ++oebeg) {
			Vertex v = target(*oebeg, G);	// source(*oebeg, G) is guaranteed to be u, even in an undirected graph.
			if (primpredmap[u] == v) {
				cout << u << " -- " << v << " (weight " << weightmap[*oebeg] << ")\n";
			}
		}
	}
	cout << endl;


	// Kruskal minimum spanning tree
	// =============================
	vector<Edge>	mst; // We must use this vector to store the MST edges (not as a property map!)
	// We can use the following vectors as Exterior Property Maps if we want to access additional information computed by Union-Find:	
	vector<Vertex>	kruskalpredmap(V);	// Stores predecessors needed for Union-Find (NOT the MST!)
	vector<int>	rankmap(V);		// Stores ranks needed for Union-Find
	kruskal_minimum_spanning_tree(G, back_inserter(mst),	// kruskal_minimum_spanning_tree(G, back_inserter(mst)); would be fine as well 
			rank_map(make_iterator_property_map(rankmap.begin(), get(vertex_index, G))).
			predecessor_map(make_iterator_property_map(kruskalpredmap.begin(), get(vertex_index, G))));			
	cout << "Kruskal builds a minimum spanning tree of total weight: ";
	totalweight = 0;
	// go through the minimum spanning tree with an iterator
	vector<Edge>::iterator	mstbeg, mstend = mst.end();
	for (mstbeg = mst.begin(); mstbeg != mstend; ++mstbeg) {
		totalweight += weightmap[*mstbeg];
	}
	cout << totalweight << "\n";
	cout << "Edges in Kruskal's minimum spanning tree:\n";
	// go through the minimum spanning tree 
	for (size_t i = 0; i < mst.size(); ++i) {
		Edge e = mst[i];
		Vertex u = source(e, G);
		Vertex v = target(e, G);
		cout << u << " -- " << v << " (weight " << weightmap[e] << ")\n";
	}
	cout << endl;


	// Edmonds' maximum cardinality matching
	// =====================================
	vector<Vertex> matemap(V);		// We MUST use this vector as an Exterior Property Map: Vertex -> Mate in the matching
	edmonds_maximum_cardinality_matching(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
	// Using the matemap 
	// =================
	const Vertex NULL_VERTEX = graph_traits<Graph>::null_vertex();	// unmatched vertices get the NULL_VERTEX as mate.
	int matchingsize = matching_size(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
	cout << "A maximum matching has " << matchingsize << " edges " << " and " << (V-2*matchingsize) << " unmatched vertices.\n"; 
	cout << "Edges in the maximum matching:\n";
	for (int i = 0; i < V; ++i) 
		if (matemap[i] != NULL_VERTEX && i < matemap[i])	// i is matched && we only print the edge once
			cout << i << " -- " << matemap[i] << "\n";
	cout << "List of unmatched vertices: ";
	for (int i = 0; i < V; ++i) 
		if (matemap[i] == NULL_VERTEX)	// i is not matched
			cout << i << " ";
	cout << "\n" << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;	T = 1;
	while(T--)	testcases();
	return 0;
}

