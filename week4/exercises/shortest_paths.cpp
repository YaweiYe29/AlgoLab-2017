//STLincludes
#include<iostream>
#include<vector>
#include<climits>

//BGLincludes
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/strong_components.hpp>
#include<boost/graph/dijkstra_shortest_paths.hpp> 
#include<boost/graph/kruskal_min_spanning_tree.hpp>

using namespace std; 
using namespace boost;

//Undirected graph with integer weights on edges. 
typedef adjacency_list<vecS, vecS, directedS,
no_property,
property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;      //Vertex type 
typedef graph_traits<Graph>::edge_descriptor Edge;          //Edgetype
typedef graph_traits<Graph>::edge_iterator EdgeIt;          // Edge iterator 
typedef property_map<Graph,edge_weight_t>::type WeightMap;  //Property map edge->weight

void FindPaths(){
    int n, m, q, ignore, from, to, w;
    cin >> n >> m >> q;

    Graph g(n);
    WeightMap weightmap = get(edge_weight, g);

    string line;
    for(int i = 0; i < n; i++){
        cin >> ignore >> ignore;
    }

    for(int i = 0; i < m; i++){
        cin >> from >> to >> w;
        Edge e;
        bool success;

        tie(e, success) = add_edge(from, to, g);
        weightmap[e] = w;
    }

    for(int i = 0; i < q; i++){
        int start, end;
        cin >> start >> end;

        vector<int> distances(n);

        dijkstra_shortest_paths(g, start,
            distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, g))));

        int distance = distances[end];
        if(distance == INT_MAX)
            cout << "unreachable\n";
        else
            cout << distance << endl;
    }

}

int main(){ 
    ios_base::sync_with_stdio(false);
    //int n;
    //cin >> n;
    //while(n--){
        FindPaths();
    //}
    return 0;
}