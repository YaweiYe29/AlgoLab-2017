//STLincludes
#include<iostream>
#include<vector>
#include<algorithm>
#include<climits>

//BGLincludes
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/strong_components.hpp>
#include<boost/graph/dijkstra_shortest_paths.hpp> //Namespaces

using namespace std; 
using namespace boost;

//Directed graph with integer weights on edges. 
typedef adjacency_list<vecS, vecS, directedS,
no_property,
property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex; //Vertex type 
typedef graph_traits<Graph>::edge_descriptor Edge; //Edgetype
typedef graph_traits<Graph>::edge_iterator EdgeIt;     // Edge iterator 
//Property map edge->weight
typedef property_map<Graph,edge_weight_t>::type WeightMap;

void testcase(){
    // Read and build graph
    int V, E, t; // 1st line: <vertex_no> <edge_no> <target>
    cin >> V >> E >> t;
    Graph GT(V); // Creates an empty graph on V vertices
    WeightMap weightmap = get(edge_weight, GT);
    for (int i = 0; i < E; ++i) {
        int u, v, w; // Each edge: <from> <to> <weight>
        cin >> u >> v >> w;
        Edge e; 
        bool success; // *** We swap u and v to create ***

        tie(e,success) = add_edge(v,u,GT);//***thereversedgraphGT! ***
        weightmap[e] = w;
    }
}

int main(){
    int n;
    cin >> n;
    for(int i = 0; i < n; i++){
        testcase();
    }
    return 0;
}