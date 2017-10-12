//STLincludes
#include<iostream>
#include<vector>
#include<algorithm>
#include<climits>

//BGLincludes
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/strong_components.hpp>
#include<boost/graph/dijkstra_shortest_paths.hpp> 
#include<boost/graph/kruskal_min_spanning_tree.hpp>

using namespace std; 
using namespace boost;

//Undirected graph with integer weights on edges. 
typedef adjacency_list<vecS, vecS, undirectedS,
no_property,
property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;      //Vertex type 
typedef graph_traits<Graph>::edge_descriptor Edge;          //Edgetype
typedef graph_traits<Graph>::edge_iterator EdgeIt;          // Edge iterator 
typedef property_map<Graph,edge_weight_t>::type WeightMap;  //Property map edge->weight

void testcase(){
    int V, E; 
    cin >> V >> E;

    Graph GT(V);
    WeightMap weightmap = get(edge_weight, GT);

    for (int i = 0; i < E; ++i) {
        int u, v, w; 
        cin >> u >> v >> w;
        Edge e; 
        bool success; 

        tie(e,success) = add_edge(v,u,GT);
        weightmap[e] = w;
    }

    //first make kruskal to find MST
    vector<Edge> mst;                                      
    kruskal_minimum_spanning_tree(GT, back_inserter(mst)); 
    vector<Edge>::iterator ebeg, eend = mst.end();

    //then sum weights
    int weights = 0;
    for(ebeg=mst.begin(); ebeg != eend; ++ebeg){
        int w = boost::get(boost::edge_weight_t(), GT, *ebeg);
        weights += w;
    }

    vector<int> distances(V);
    vector<Vertex> predecessors(V);

    //and find shortest path to the furthest vertex from vertex 0
    Vertex v = 0;
    dijkstra_shortest_paths(GT, 0,
        predecessor_map(make_iterator_property_map(predecessors.begin(), get(vertex_index, GT))).
        distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, GT))));
        int max = INT_MIN;
        for(int i = 0; i < V; i++){
            if(distances[i] > max && distances[i] < INT_MAX)
                max = distances[i];
        }

    //output solution
    cout << weights << " " << max << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    for(int i = 0; i < n; i++){
        testcase();
    }
    return 0;
}