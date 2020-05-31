#include <fstream>
#include <cstring>

#include "graph.h"

#ifndef M_FLOW_NETWORK_H
#define M_FLOW_NETWORK_H

class FlowNetwork {
public:
    class NodeData;
    class EdgeData;
    typedef Graph<NodeData, EdgeData>::Vertex Vertex;
    typedef Graph<NodeData, EdgeData>::Edge Edge;

    class NodeData {
    public:
        std::string name;
        bool visit_flag = false;

        NodeData();
        NodeData(std::string const& name);

        bool operator==(NodeData const& other);
        bool operator==(std::string const& str);
    };

    class EdgeData {
    public:
        int max_flow = 0;
        int flow = 0;
        Edge* opposite = nullptr;

        EdgeData();
        EdgeData(int max_flow);
        int remaining_flow();
    };

    Graph<NodeData, EdgeData> graph;
    Vertex* source = nullptr;
    Vertex* target = nullptr;

    void clear();
    void reset_vertices();
    void reset_edges();
    Vertex& vertex_by_name(std::string const &name);

    bool is_valid(Vertex* vertex = nullptr, Vertex* last = nullptr);
    bool search_path(Vertex* vertex,  Array<Edge*>& path);
    void _add_temporary_edges();
    void _remove_temporary_edges();
    void build_max_flow();
    int vertex_flow(Vertex &vertex);

    bool parse_edge(std::string line);
    bool from_file(std::string filename);
    void print();

};



#endif
