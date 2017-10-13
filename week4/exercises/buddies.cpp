//STLincludes
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <climits>
#include <algorithm>

//BGLincludes
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/strong_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

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

struct Friend{
    set<int> hobbies;
};

void buddy(){
    //n-buddies, c - characteristics, f - min. comm. char,
    int n, c, f, hobbyId = 0;
    cin >> n >> c >> f;
    map<string, int> hashedNames;
    map<pair<int, int>, int> edgesCount;
    Friend friends[n];

    Graph graph(n);
    Graph graph2(n);
    WeightMap weightmap = get(edge_weight, graph);
    WeightMap weightmap2 = get(edge_weight, graph2);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < c; j++){
            string hobby;
            cin >> hobby;
            if(hashedNames.find(hobby) == hashedNames.end()){
                hashedNames[hobby] = hobbyId++;
            }
            friends[i].hobbies.insert(hashedNames[hobby]);
        }
    }

    for(int i = 0; i < n - 1; i++){
        for(int j = i + 1; j < n; j++){
            vector<int> inter(c);
            auto it = set_intersection(friends[i].hobbies.begin(), friends[i].hobbies.end(),
                             friends[j].hobbies.begin(), friends[j].hobbies.end(),
                              inter.begin());
            pair<int, int> pair(i, j);
            inter.resize(it - inter.begin());
            // assign number of parallel edges.
            edgesCount[pair] = inter.size();
        }
    }
    //construct second graph where are just edges than are duplicated more than f
    for(auto it = edgesCount.begin(); it != edgesCount.end(); ++it){
        if(it->second > f){
            Edge e; 
            bool success; 
    
            tie(e,success) = add_edge(it->first.first, it->first.second, graph2);
            weightmap2[e] = 1;
        }
    }

    vector<Vertex> matemap(n);		// We MUST use this vector as an Exterior Property Map: Vertex -> Mate in the matching
	edmonds_maximum_cardinality_matching(graph2, make_iterator_property_map(matemap.begin(), get(vertex_index, graph2)));
	const Vertex NULL_VERTEX = graph_traits<Graph>::null_vertex();	// unmatched vertices get the NULL_VERTEX as mate.
    int matchingsize = matching_size(graph, make_iterator_property_map(matemap.begin(), get(vertex_index, graph)));
    
    if(n - 2 * matchingsize == 0)
        cout << "not optimal\n";
    else
        cout << "optimal\n";
}

int main(){
    int n;
    cin >> n;
    while(n--){
        buddy();
    }
    return 0;
}