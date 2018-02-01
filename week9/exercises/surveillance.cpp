    // STL includes
    #include <iostream>
    #include <vector>
    #include <algorithm>
    

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
            std::cout << "adding edge from: " << from << " to: " << to << " capacity: " << capacity << std::endl;
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

        std::vector<int> police;
        std::vector<int> photo;

        for(int i = 0; i < nPolice; i++){
            int x;
            std::cin >> x;
            police.push_back(x);
        }

        for(int i = 0; i < nPhoto; i++){
            int x;
            std::cin >> x;
            photo.push_back(x);
        }

        // Create Graph and Maps
        Graph G(nIntersections);
        EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
        ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
        ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, revedgemap);

        // loop through all cities and substract actual and needed state
        // if state < 0 then we need soldiers
        for(int i = 0; i < nEdges; i++){
            int from, to;
            std::cin >> from >> to;
            eaG.addEdge(from, to, 1);
        }

        // start and end vertexes
        Vertex source = boost::add_vertex(G);
        Vertex target = boost::add_vertex(G);

        for(int i = 0; i < nPolice; i++){
            eaG.addEdge(police[i], target, 1);
            eaG.addEdge(source, police[i] + nIntersections + nPhoto, 1);
        }

        for(int i = 0; i < nPhoto; i++){
            eaG.addEdge(photo[i], photo[i] + nIntersections, 1);
            //eaG.addEdge(source, photo[i], 1);
        }

        // Calculate flow
        // If not called otherwise, the flow algorithm uses the interior properties
        // - edge_capacity, edge_reverse (read access),
        // - edge_residual_capacity (read and write access).
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
