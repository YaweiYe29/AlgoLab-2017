    // STL includes
    #include <iostream>
    #include <vector>
    #include <algorithm>
    #include <set>
    #include <climits>

    // BGL includes
    #include <boost/graph/adjacency_list.hpp>
    #include <boost/graph/push_relabel_max_flow.hpp>
    #include <boost/graph/edmonds_karp_max_flow.hpp>
    // Namespaces
    // using namespace std;
    using namespace boost;


    // BGL Graph definitions
    // =====================
    // Graph Type with nested interior edge properties for Flow Algorithms
    typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
            boost::property<boost::edge_residual_capacity_t, long,
                boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
    // Interior Property Maps
    typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
    typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
    typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
    typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
    typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;

    // Custom Edge Adder Class, that holds the references
    // to the graph, capacity map and reverse edge map
    // ===================================================
    class EdgeAdder {
        Graph &G;
        EdgeCapacityMap	&capacitymap;
        ReverseEdgeMap	&revedgemap;

    public:
        // to initialize the Object
        EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
            G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

        // to use the Function (add an edge)
        void addEdge(int from, int to, long capacity) {
            //std::cout << "adding edge from: " << from << " to: " << to << " capacity: " << capacity << std::endl;
            Edge e, rev_e;
            bool success;
            boost::tie(e, success) = boost::add_edge(from, to, G);
            boost::tie(rev_e, success) = boost::add_edge(to, from, G);
            capacitymap[e] = capacity;
            capacitymap[rev_e] = 0; // reverse edge has no capacity!
            revedgemap[e] = rev_e;
            revedgemap[rev_e] = e;
        }
    };

    // Functions
    // =========
    // Function for an individual testcase
    void testcases() {
        int nIntersections, nEdges, nPolice, nPhoto;
        std::cin >> nIntersections >> nEdges >> nPolice >> nPhoto;

        // Create Graph and Maps
        Graph G;
        EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
        ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
        ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, revedgemap);

        // start and end vertexes
        Vertex source = 2 * nIntersections;
        Vertex target = 2 * nIntersections + 1;

        for(int i = 0; i < nPolice; i++){
            int x;
            std::cin >> x;
            eaG.addEdge(source, x, 1);
            eaG.addEdge(x + nIntersections, target, 1);
        }

        for(int i = 0; i < nPhoto; i++){
            int x;
            std::cin >> x;
            eaG.addEdge(x, x + nIntersections, 1);
        }

        // loop through all cities and substract actual and needed state
        // if state < 0 then we need soldiers
        for(int i = 0; i < nEdges; i++){
            int from, to;
            std::cin >> from >> to;
            int ffrom = from;
            
            eaG.addEdge(ffrom, to, INT_MAX);
            eaG.addEdge(ffrom + nIntersections, to + nIntersections, 1);
        }

        long flow = boost::push_relabel_max_flow(G, source, target);

        std::cout << flow << std::endl;
    }

    // Main function to loop over the testcases
    int main() {
        std::ios_base::sync_with_stdio(false);
        int n;	
        std::cin >> n;
        while(n--){
            testcases();
        }
        return 0;
    }
