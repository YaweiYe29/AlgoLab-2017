/* Master solution */
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <vector>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;
typedef Delaunay::Finite_edges_iterator EI;
typedef std::pair<K::Point_2, int> IPoint;
typedef boost::disjoint_sets_with_storage<> Uf;

struct Edge
{
    Edge(int u_, int v_, K::FT sql_) : u(u_), v(v_), sql(sql_) {}
    int u, v;  // endpoints
    K::FT sql; // squared length
};
bool cmp(const Edge &e, const Edge &f) { return e.sql < f.sql; }

void handle_missions()
{
    std::size_t n, m;
    double p;
    std::cin >> n >> m >> p;
    // read jammers and build Delaunay
    std::vector<IPoint> jammers;
    jammers.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
    {
        int x, y;
        std::cin >> x >> y;
        jammers.push_back(IPoint(K::Point_2(x, y), i));
    }
    Delaunay t;
    t.insert(jammers.begin(), jammers.end());
    // extract edges and sort by length

    std::vector<Edge> edges;
    edges.reserve(3 * n);
    for (EI e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
        edges.push_back(Edge(e->first->vertex((e->second + 1) % 3)->info(), e->first->vertex((e->second + 2) % 3)->info(),
                             t.segment(e).squared_length()));
    std::sort(edges.begin(), edges.end(), cmp);
    // compute components with power consumption p
    Uf ufp(n);
    typedef std::vector<Edge>::const_iterator ECI;
    for (ECI e = edges.begin(); e != edges.end() && e->sql <= p; ++e)
        ufp.union_set(e->u, e->v);
    // handle missions
    K::FT a = 0;
    K::FT b = 0;
    Uf ufa(n);
    Uf ufb(n);
    ECI ai = edges.begin();
    ECI bi = edges.begin();
    for (std::size_t i = 0; i < m; ++i)
    {
        int x0, y0, x1, y1;
        std::cin >> x0 >> y0 >> x1 >> y1;
        K::Point_2 p0(x0, y0), p1(x1, y1);
        Delaunay::Vertex_handle v0 = t.nearest_vertex(p0);
        Delaunay::Vertex_handle v1 = t.nearest_vertex(p1);
        K::FT d = 4 * std::max(CGAL::squared_distance(p0, v0->point()),
                               CGAL::squared_distance(p1, v1->point()));
        if (d <= p && ufp.find_set(v0->info()) == ufp.find_set(v1->info()))
        {
            // mission possible with power p -> also with b
            std::cout << "y";
            if (d > b)
                b = d;
            for (; bi != edges.end() &&
                   ufb.find_set(v0->info()) != ufb.find_set(v1->info());
                 ++bi)
                ufb.union_set(bi->u, bi->v);
        }
        else
            std::cout << "n";
        // ensure it is possible at power a
        if (d > a)
            a = d;
        for (; ai != edges.end() &&
               ufa.find_set(v0->info()) != ufa.find_set(v1->info());
             ++ai)
            ufa.union_set(ai->u, ai->v);
    }
    if (ai != edges.begin() && (ai - 1)->sql > a)
        a = (ai - 1)->sql;
    if (bi != edges.begin() && (bi - 1)->sql > b)
        b = (bi - 1)->sql;
    std::cout << "\n"
              << a << "\n"
              << b << "\n";
}
int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    std::size_t t;
    for (std::cin >> t; t > 0; --t)
        handle_missions();
    return 0;
}