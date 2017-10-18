//STLincludes
#include<iostream>
#include<vector>
#include<climits>
#include<algorithm>
#include<map>

//BGLincludes
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/dijkstra_shortest_paths.hpp> 

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

// we need to project vertex to the appropriate level
// nOfLevels is number of levels, it is k + 1
// k means number of rivers we need to pass, so we create k + 1 levels in graph and add the river edges as bridges between those levels
int Project(int vertex, int level, int nOfLevels){
    return vertex * nOfLevels + level;
}

// We are adding edge kind of strangely. We need to project vertexes to levels and then add connection in appropriate level. 
// Function Project projects vertice to the level. If from_level == to_level then we are adding edge in the same level.
// If from_level != to_level than we are adding connection between levels "bridge".
void AddEdge(Graph& g, WeightMap& map, int from, int from_level, int to, int to_level, int time, int k){
    int f = Project(from, from_level, k + 1);
    int t = Project(to, to_level, k + 1);
    //cout << "(" << f << ";" << t << ") : " << time << endl;
    Edge e;
    bool success;
    tie(e, success) = add_edge(f, t, g);
    map[e] = time;
}

void Track(){
    int n, m, k, x, y, from, to, time, river;
    cin >> n >> m >> k >> x >> y;

    // graph will have k + 1 levels and every level will have n vertices
    Graph g(n * (k + 1));
    WeightMap weightmap = get(edge_weight, g);
    
    for(int i = 0; i < m; i++){
        cin >> from >> to >> time >> river;
        if(river){
            //cout << "river\n";
            for(int i = 0; i < k; i++){
                // for every level i and i + 1 we add river edge as a bridge.
                AddEdge(g, weightmap, from, i, to, i + 1, time, k);
                AddEdge(g, weightmap, from, i + 1, to, i, time, k);
            }
            // for the k-th level we add river edge not like a bridge but to the level because it can cause faster way (there can be more than k river paths).
            AddEdge(g, weightmap, from, k, to, k, time, k);
            //cout << "----------------\n";
        } else {
            // if edge is not river edge we just add it to every level.
            for(int i = 0; i <= k; i++){
                //cout << "level" << i << ": ";
                AddEdge(g, weightmap, from, i, to, i, time, k);
            }
        }
    }

    vector<int> distances(n * (k + 1));
    int from2 = Project(x, 0, k + 1);
    int to2 = Project(y, k, k + 1);
    //cout << "from: " << from2 << " to: " << to2 << endl;
    dijkstra_shortest_paths(g, from2,
        distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, g))));

    cout << distances[to2] << endl;
}

int main(){
    int n;
    cin >> n;
    while(n--){
        Track();
    }
    return 0;
}