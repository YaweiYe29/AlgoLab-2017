#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;

typedef std::pair<K::Point_2, int> IPoint;

bool check_interference(Triangulation &t, long long rr)
{ // go throught all edges and check that their endpoints have different colors
    for (auto it = t.finite_edges_begin(); it != t.finite_edges_end(); ++it)
    {
        if (t.segment(*it).squared_length() <= rr)
            return false;
    }
    return true;
}

bool can_color(Graph &G, int n, long long rr, std::vector<IPoint> pts)
{
    std::vector<int> colored(n, 2); // 2 -> unvisited, 0 -> red, 1 -> black
    std::vector<K::Point_2> ff, ss;
    Triangulation t1, t2;
    std::queue<int> stack;
    for (int i = 0; i < n; i++)
    { // check all componenets
        if (colored[i] == 2)
        { // if component of vertex already checked than continue
            stack.push(i);  // do dfs from ith vertex
            colored[i] = 0; // mark him as red
            while (!stack.empty())
            {
                int curr = stack.front();
                assert(curr != 2);
                stack.pop();
                OutEdgeIt ebeg, eend;
                for (boost::tie(ebeg, eend) = boost::out_edges(curr, G); ebeg != eend; ++ebeg)
                {
                    const int v = boost::target(*ebeg, G); // get target vertex
                    assert(v != curr);
                    if (colored[v] != 2)
                    {                                     // if already visited
                        if (colored[v] == colored[curr]) {// if we reached the same color for him than its wrong
                            return false;                 // return false (graph can not be colored in 2 colors)
                        }
                        else
                            continue;                     // else its OK continue
                    }
                    colored[v] = (colored[curr] + 1) % 2; // color vertex with current color
                    stack.push(v);
                }
            }
        }
        if (colored[i] == 0) ff.push_back(pts[i].first);
        else ss.push_back(pts[i].first);
    }
    t1.insert(ss.begin(), ss.end());
    t2.insert(ss.begin(), ss.end());
    return check_interference(t1, rr) && check_interference(t2, rr);
}

void testcase()
{
    int n, m, r; //n -> number of stations, m -> number of clues, r -> radius
    std::cin >> n >> m >> r;
    Graph G(n);
    long long rr = r * r;
    std::vector<IPoint> pts;
    pts.reserve(n);
    for (int i = 0; i < n; i++)
    {
        K::Point_2 pt;
        std::cin >> pt;
        pts.push_back(IPoint(pt, i));
    }

    Triangulation t;
    t.insert(pts.begin(), pts.end());

    for (auto it = t.finite_edges_begin(); it != t.finite_edges_end(); ++it)
    {
        double dist = t.segment(*it).squared_length();
        int from = it->first->vertex((it->second + 1) % 3)->info();
        int to = it->first->vertex((it->second + 2) % 3)->info();
        if (dist <= rr)
            boost::add_edge(from, to, G);
    }

    bool valid_graph = can_color(G, n, rr, pts);

    std::vector<int> componentmap(n); // We MUST use such a vector as an Exterior Property Map: Vertex -> Component

    if(valid_graph)
        boost::connected_components(G, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, G)));

    for (int i = 0; i < m; i++)
    {
        K::Point_2 pt1, pt2;
        std::cin >> pt1 >> pt2;
        auto from_handle = t.nearest_vertex(pt1);
        auto to_handle = t.nearest_vertex(pt2);
        if (valid_graph)
        {
            if ((CGAL::squared_distance(from_handle->point(), pt1) <= rr && CGAL::squared_distance(to_handle->point(), pt2) <= rr &&
                 componentmap[from_handle->info()] == componentmap[to_handle->info()]) ||
                 CGAL::squared_distance(pt1, pt2) <= rr)
            {
                std::cout << "y";
            }
            else
            {
                std::cout << "n";
            }
        }
        else
        {
            std::cout << "n";
        }
    }
    std::cout << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--)
    {
        testcase();
    }
    return 0;
}