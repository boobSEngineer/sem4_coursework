#include "flow_network.h"

FlowNetwork::NodeData::NodeData() {}

FlowNetwork::NodeData::NodeData(std::string const &name) : name(name) {}

bool FlowNetwork::NodeData::operator==(const FlowNetwork::NodeData &other) {
    return name == other.name;
}

bool FlowNetwork::NodeData::operator==(std::string const &str) {
    return name == str;
}

FlowNetwork::EdgeData::EdgeData() {}

FlowNetwork::EdgeData::EdgeData(int max_flow) : max_flow(max_flow) {}

int FlowNetwork::EdgeData::remaining_flow() {
    return max_flow - flow;
}



void FlowNetwork::clear() {
    graph.clear();
    source = target = nullptr;
}

void FlowNetwork::reset_vertices() {
    graph.for_each([] (Vertex& vertex) -> void {
        vertex.data.visit_flag = false;
    });
}

void FlowNetwork::reset_edges() {
    graph.for_each([] (Vertex& vertex) -> void {
        vertex.for_each([] (Edge& edge) -> void {
            edge.data.flow = 0;
        });
    });
}



bool FlowNetwork::is_valid(FlowNetwork::Vertex *vertex, FlowNetwork::Vertex *last) {
    if (vertex == nullptr) {
        if (source == nullptr || target == nullptr) {
            return false;
        }
        vertex = source;
        reset_vertices();
    }
    if (vertex->data.visit_flag) {
        return false;
    }
    vertex->data.visit_flag = true;
    bool any = false;
    for (auto it = vertex->edges.begin(); it != vertex->edges.end(); it++) {
        if (it->connected != last && it->data.max_flow > 0) {
            any = true;
            bool result = is_valid(it->connected, vertex);
            if (!result) {
                return false;
            }
        }
    }
    if (!any && target != vertex) {
        return false;
    }
    vertex->data.visit_flag = false;
    return true;
}

bool FlowNetwork::search_path(FlowNetwork::Vertex* vertex, Array<FlowNetwork::Edge*>& path) {
    if (vertex == target) {
        return true;
    }
    if (vertex->data.visit_flag) {
        return false;
    }

    vertex->data.visit_flag = true;
    for (auto it = vertex->edges.begin(); it != vertex->edges.end(); it++) {
        if (it->data.remaining_flow() > 0) {
            path.add(&*it);
            if (search_path(it->connected, path)) {
                return true;
            }
            path.resize(path.length() - 1);
        }
    }
    return false;
}


void FlowNetwork::_add_temporary_edges() {
    auto edges = graph.get_all_edges();
    for (int i = 0; i < edges.length(); i++) {
        edges[i]->data.opposite = &graph.connect(*edges[i]->connected, *edges[i]->from, EdgeData(0));
        edges[i]->data.opposite->data.opposite = edges[i];
    }
}

void FlowNetwork::_remove_temporary_edges() {
    auto edges = graph.get_all_edges();
    for (int i = 0; i < edges.length(); i++) {
        if (edges[i]->data.max_flow == 0) {
            graph.disconnect(*edges[i]->from, *edges[i]->connected);
            edges[i]->data.opposite = nullptr;
        }
    }
}

void FlowNetwork::build_max_flow() {
    reset_edges();

    _add_temporary_edges();
    while (true) {
        Array<Edge*> path;
        reset_vertices();
        if (!search_path(source, path)) {
            break;
        }

        int min_remaining_flow = path[0]->data.remaining_flow();
        for (int i = 1; i < path.length(); i++) {
            int remaining_flow = path[i]->data.remaining_flow();
            if (remaining_flow < min_remaining_flow) {
                min_remaining_flow = remaining_flow;
            }
        }

        for (int i = 0; i < path.length(); i++) {
            path[i]->data.flow += min_remaining_flow;
            path[i]->data.opposite->data.flow -= min_remaining_flow;
        }
    }
    _remove_temporary_edges();
}

int FlowNetwork::vertex_flow(Vertex &vertex) {
    int flow = 0;
    graph.for_each([&] (Vertex& v) -> void {
        v.for_each([&] (Edge& edge) -> void {
            if (edge.connected == &vertex) {
                flow += edge.data.flow;
            }
            else if (edge.from == &vertex) {
                flow -= edge.data.flow;
            }
        });
    });
    return flow;
}

FlowNetwork::Vertex &FlowNetwork::vertex_by_name(std::string const &name) {
    Vertex* vertex = graph.get_vertex(name);
    if (vertex == nullptr) {
        return graph.add_vertex(NodeData(name));
    }
    return *vertex;
}

bool FlowNetwork::parse_edge(std::string line) {
    Array<std::string> tokens;

    char* token = strtok(line.data(), " ");
    while (token != nullptr) {
        tokens.add(std::string(token));
        token = strtok(nullptr, " ");
    }

    if (tokens.length() == 3) {
        int max_flow = (int) std::strtol(tokens[2].data(), nullptr, 10);
        if (max_flow > 0) {
            Vertex& vertex1 = vertex_by_name(tokens[0]);
            Vertex& vertex2 = vertex_by_name(tokens[1]);

            graph.connect(vertex1, vertex2, EdgeData(max_flow));
            if (vertex1.data.name == "S") {
                source = &vertex1;
            }
            if (vertex1.data.name == "T") {
                target = &vertex1;
            }
            if (vertex2.data.name == "S") {
                source = &vertex2;
            }
            if (vertex2.data.name == "T") {
                target = &vertex2;
            }
        } else {
            Vertex* vertex1 = graph.get_vertex(tokens[0]);
            Vertex* vertex2 = graph.get_vertex(tokens[1]);
            if (vertex1 != nullptr && vertex2 != nullptr) {
                graph.disconnect(*vertex1, *vertex2);
            }
        }
        return true;
    }
    return false;
}

bool FlowNetwork::from_file(std::string filename) {
    std::ifstream stream(filename);
    if (stream) {
        graph.clear();
        std::string line;
        while (std::getline(stream, line)) {
            if (!line.empty() && !parse_edge(line)) {
                std::cout << "invalid formatted edge: " << line << "\n";
            }
        }
        return true;
    } else {
        return false;
    }
}

void FlowNetwork::print() {
    graph.print(10,
                 [] (NodeData& data) -> std::string {
                     return data.name;
                 },
                 [] (EdgeData& data) -> std::string {
                     char line[64];
                     sprintf(line, "%2i/%-2i", data.flow, data.max_flow);
                     return line;
                 });
}
