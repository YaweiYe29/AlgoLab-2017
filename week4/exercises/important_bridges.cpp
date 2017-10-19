//STLincludes
#include<iostream>
#include<vector>
#include<algorithm>
#include<climits>
#include<utility>

//BGLincludes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace std; 
using namespace boost;

struct edge_component_t
{
  enum
  { num = 555 };
  typedef edge_property_tag kind;
}
edge_component;

//Undirected graph with integer weights on edges. 
typedef adjacency_list<vecS, vecS, undirectedS,
no_property,
property<edge_component_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;      //Vertex type 
typedef graph_traits<Graph>::edge_descriptor Edge;          //Edgetype
typedef graph_traits<Graph>::edge_iterator EdgeIt;          // Edge iterator 
typedef property_map<Graph,edge_component_t>::type ComponentMap;  //Property map edge->weight

int cmp(pair<int, int> p1, pair<int, int> p2){
    if(p1.first == p2.first)
        return p1.second < p2.second;
    else
        return p1.first < p2.first;
}

pair<int, int> correctPair(pair<int, int> p){
    pair<int, int> res = p;
    if(p.first > p.second) {
        res.first = p.second;
        res.second = p.first;
    }
    return res;
}

void Bridges(){
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int> > > edges(n);
    vector<pair<int, int> > res;

    Graph g(n);

    for(int i = 0; i < m; i++){
        int x0, x1;
        cin >> x0 >> x1;
        add_edge(x0, x1, g);
    }

    ComponentMap components = get(edge_component, g);
    int nc = biconnected_components(g, components);
    EdgeIt it, itEnd;
    for(tie(it, itEnd) = boost::edges(g); it != itEnd; ++it){
        pair<int, int> p;
        p.first = source(*it, g);
        p.second = target(*it, g);

        edges[components[*it]].push_back(p);
    }

    for(int i = 0; i < nc; i++){
        if(edges[i].size() == 1){
            res.push_back(correctPair(edges[i][0]));
        }
    }

    // commented solution is valid but I scored just 40/100 because of time limit.
    /*for(int i = 0; i < m; i++){
        remove_edge(edges[i].first, edges[i].second, g);
	    vector<int> componentmap(n);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	    int ncc = biconnected_components(g, make_iterator_property_map(componentmap.begin(), get(vertex_index, g))); 
        if(ncc > 1){
            pair<int, int> pp;
            //cout << edges[i].first << " " << edges[i].second << endl;
            if(edges[i].first > edges[i].second){
                pp.first = edges[i].second;
                pp.second = edges[i].first;
            }
            else
                pp = edges[i];

            res.push_back(pp);
        }
        Edge e; 
        bool success; 

        tie(e,success) = add_edge(edges[i].first, edges[i].second, g);
        weightmap[e] = 1;
    }*/

    sort(res.begin(), res.end(), cmp);

    cout << res.size() << endl;
    for(int i = 0; i < res.size(); i++){
        cout << res[i].first << " " << res[i].second << endl;
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n--){
        Bridges();
    }
    return 0;
}