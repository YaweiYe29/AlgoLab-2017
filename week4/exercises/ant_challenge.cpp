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

//Undirected graph with integer weights on edges and parallel edges. 
typedef adjacency_list<vecS, vecS, undirectedS,
allow_parallel_edge_tag,
property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;      //Vertex type 
typedef graph_traits<Graph>::edge_descriptor Edge;          //Edgetype
typedef graph_traits<Graph>::edge_iterator EdgeIt;          // Edge iterator 
typedef property_map<Graph,edge_weight_t>::type WeightMap;  //Property map edge->weight

void testcase(){
    int V, E, insectTypesCount, startV, endV; 
    cin >> V >> E >> insectTypesCount >> startV >> endV;

    // graph for each insect type + final graph
    Graph graphs[insectTypesCount + 1];
    WeightMap weightmaps[insectTypesCount + 1];

    //initialize graphs
    for(int i = 0; i < insectTypesCount; i++){
        graphs[i] = Graph(V);
        weightmaps[i] = get(edge_weight, graphs[i]);
    }

    //load each graph(all graphs have same edges and vertices but different weights)
    int k = 0;
    for (int i = 0; i < E; ++i) {
        int u, v, w; 
        cin >> u >> v;
        k += 2;
        Edge e; 
        bool success; 

        for(int j = 0; j < insectTypesCount; j++){
            cin >> w;
            tie(e,success) = add_edge(v, u, graphs[j]);
            weightmaps[j][e] = w;
            k++;
        }
    }

    //read unused hives
    int tmp;
    for(int i = 0; i < insectTypesCount; i++){
        cin >> tmp;
        k++;
    }

    //calc MST for every graph and create final graph with all edges from all MSTs
    for(int i = 0; i < insectTypesCount; i++){
        vector<Edge> mst;                                                 
        kruskal_minimum_spanning_tree(graphs[i], back_inserter(mst)); 
        vector<Edge>::iterator ebeg, eend = mst.end();

        int weights = 0;
        for(ebeg=mst.begin(); ebeg != eend; ++ebeg){
            Edge e;
            bool success;

            tie(e,success) = add_edge(source(*ebeg, graphs[i]), target(*ebeg, graphs[i]), graphs[insectTypesCount]);
            weightmaps[insectTypesCount][e] = get(boost::edge_weight_t(), graphs[i], *ebeg);
        }
    }

    vector<int> distances(V);
    vector<Vertex> predecessors(V);

    //find shortest path
    Vertex v = 0;
    dijkstra_shortest_paths(graphs[insectTypesCount], startV,
        predecessor_map(make_iterator_property_map(predecessors.begin(), get(vertex_index, graphs[insectTypesCount]))).
        distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, graphs[insectTypesCount]))));

    cout << distances[endV] << endl;
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