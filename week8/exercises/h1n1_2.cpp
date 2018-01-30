#include <iostream>
#include <vector>
#include <set>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;

typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Face_handle Face_handle;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property,
		boost::property<boost::edge_weight_t, double>>             Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		        Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor		    Vertex;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;

// whether h is in visited
bool contains(std::set<Face_handle> &visited, Face_handle h){
    return visited.find(h) != visited.end();
}

void addEdge(int from, int to, double w, Graph& G, WeightMap &wm){
    Edge e; bool success;
    std::cout << "Adding edge from " << from << " to " << to << " with weight " << w << std::endl;
    tie(e, success) = boost::add_edge(from, to, G);
    wm[e] = INT32_MAX - w;
}

// create graph from triangulation return set of infinite nodes
std::set<int> create_graph(Face_handle start, Triangulation &t, Graph &G, WeightMap &wm){
    std::set<int> infinite;
    std::set<Face_handle> visited;
    std::queue<Face_handle> queue;
    queue.push(start);
    visited.insert(start);
    while(!queue.empty()){
        Face_handle curr = queue.front();
        queue.pop();
        if(t.is_infinite(curr)){
            infinite.insert(curr->info());
            continue;
        }

        if(!contains(visited, curr->neighbor(2))){
            queue.push(curr->neighbor(2));
            visited.insert(curr->neighbor(2));
            double dist = CGAL::squared_distance(curr->vertex(0)->point(), curr->vertex(1)->point());
            addEdge(curr->info(), curr->neighbor(2)->info(), dist, G, wm);
        }
        if(!contains(visited, curr->neighbor(0))){
            queue.push(curr->neighbor(0));
            visited.insert(curr->neighbor(0));
            double dist = CGAL::squared_distance(curr->vertex(2)->point(), curr->vertex(1)->point());
            addEdge(curr->info(), curr->neighbor(0)->info(), dist, G, wm);
        }
        if(!contains(visited, curr->neighbor(1))){
            queue.push(curr->neighbor(1));
            visited.insert(curr->neighbor(1));
            double dist = CGAL::squared_distance(curr->vertex(0)->point(), curr->vertex(2)->point());
            addEdge(curr->info(), curr->neighbor(1)->info(), dist, G, wm);
        }
    }
    return infinite;
}

void testcase(int n)
{
    // read points
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    for (int i = 0; i < n; i++) {
        K::Point_2 p;
        std::cin >> p;
        pts.push_back(p);
    }

    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    int counter = 0;
    for(auto it = t.all_faces_begin(); it != t.all_faces_end(); ++it){
        it->info() = counter++;
    }
    std::cerr << "faces marked. count: " << counter << std::endl;
    std::vector<double> max_es_face_rad(counter, 0);
    Graph G(counter);
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    std::set<int> infinite = create_graph(t.finite_faces_begin(), t, G, weightmap);
    std::cerr << "infinite: ";
    for(auto it = infinite.begin(); it != infinite.end(); ++it){
        std::cerr << *it << " ";
    }
    std::cerr << std::endl;
    for(auto it = infinite.begin(); it != infinite.end(); ++it){
	    std::vector<Vertex> predmap(counter);
	    std::vector<double> distmap(counter);
	    boost::dijkstra_shortest_paths(G, *it,                                                                              // We MUST provide at least one of the two maps
		    boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		    distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	        // distance map as Named Parameter
        for(int i = 0; i < counter; i++){
            std::cout << predmap[i] << " ";
        }
        std::cout << std::endl;
    }
    int m;
    std::cin >> m;

    for(int i = 0; i < m; i++){
        K::Point_2 pt;
        double d;
        std::cin >> pt >> d;
        int face = t.locate(pt)->info();
        K::Point_2 pt2 = t.nearest_vertex(pt)->point();
        //if we get infected at the beginning output n. Otherwise try to escape...
        if(CGAL::squared_distance(pt, pt2) < d)
            std::cout << "n";
        else if(max_es_face_rad[face] >= d)
            std::cout << "y";
        else
            std::cout << "n";
    }
    std::cout << std::endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    std::cin >> n;
    while(n != 0){
        testcase(n);
        std::cin >> n;
    }
    return 0;
}