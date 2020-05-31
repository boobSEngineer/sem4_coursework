#include <functional>
#include <string>
#include <string.h>
#include <sstream>

#include "array.h"
#include "list.h"


#ifndef M_GRAPH_H
#define M_GRAPH_H


template <typename V, typename E>

class Graph {
public:
    class Vertex;
    class Edge {
    public:
        friend class Graph;

        Vertex* connected = nullptr;
        Vertex* from = nullptr;
        E data;

    private:
        bool print_flag = false;
    public:

        Edge() = default;
        Edge(Vertex* from, Vertex* connected, E const& data) : from(from), connected(connected), data(data) {}
    };

    class Vertex {
    public:
        friend class Graph;

        List<Edge> edges;
        V data;

    private:
        int print_position = 0;
        std::string print_str = "";

    public:
        Vertex() = default;
        Vertex(V const& data) : data(data) {};

        void for_each(std::function<void(Edge&)> func) {
            for (auto it = edges.begin(); it != edges.end(); it++) {
                func(*it);
            }
        }
    };

private:
    Array<Vertex> vertices;

public:
    Array<Vertex>& get_vertices() {
        return vertices;
    }

    int size() {
        return vertices.length();
    }

    Edge& connect(Vertex& v1, Vertex& v2, E const& data) {
        for (auto i = v1.edges.begin(); i != v1.edges.end(); i++) {
            if (i->connected == &v2) {
                i->data = data;
                return *i;
            }
        }
        return *v1.edges.add(Edge(&v1, &v2, data));
    }

    void disconnect(Vertex& v1, Vertex& v2) {
        for (auto i = v1.edges.begin(); i != v1.edges.end(); i++) {
            if (i->connected == &v2) {
                v1.edges.erase(i);
                break;
            }
        }
    }

    Array<Edge*> get_all_edges() {
        Array<Edge*> result;
        for (int i = 0; i < vertices.length(); i++) {
            Vertex& vertex = vertices[i];
            for (auto i = vertex.edges.begin(); i != vertex.edges.end(); i++) {
                result.add(&*i);
            }
        }
        return result;
    }

    Edge* get_edge(Vertex& v1, Vertex& v2) const {
        for (auto i = v1.edges.begin(); i != v1.edges.end(); i++) {
            if (i->connected == &v2) {
                return &*i;
            }
        }
        return nullptr;
    }

    template <typename T>
    Vertex* get_vertex(T const& data) {
        for (int i = 0; i < vertices.length(); i++) {
            if (vertices[i].data == data) {
                return &vertices[i];
            }
        }
        return nullptr;
    }

    Vertex& add_vertex(V const& data) {
        return vertices.add(Vertex(data));
    }

    void for_each(std::function<void(Vertex&)> func) {
        for (int i = 0; i < vertices.length(); i++) {
            func(vertices[i]);
        }
    }

    void clear() {
        vertices.clear();
    }

    void print(int arrow_length, std::function<std::string(V&)> vertex_to_string, std::function<std::string(E&)> edge_to_string) {
        if (get_vertices().length() == 0) {
            std::cout << "empty\n";
            return;
        }

        for_each([] (Vertex& vertex) -> void {
            vertex.for_each([] (Edge& edge) -> void {
                edge.print_flag = false;
            });
        });

        int string_length = 1;
        for (int i = 0; i < vertices.length(); i++) {
            vertices[i].print_position = i;
            vertices[i].print_str = vertex_to_string(vertices[i].data);
            if (vertices[i].print_str.length() > string_length) {
                string_length = (int) vertices[i].print_str.length();
            }
        }

        for (int i = 0; i < vertices.length(); i++) {
            Vertex& vertex = vertices[i];
            std::cout << vertex.print_str;
            int len = string_length + arrow_length - (int) vertex.print_str.length();
            for (int j = 0; j < len; j++) {
                std::cout << " ";
            }
        }
        std::cout << "\n";

        int counter = 0;
        bool all_printed = false;
        while (!all_printed) {
            int line_length = (string_length + arrow_length) * vertices.length();
            char* line = new char[line_length + 1];
            for (int i = 0; i < line_length; i++) {
                line[i] = i % (string_length + arrow_length) == 0 ? '|' : ' ';
            }
            line[line_length] = 0;

            int end_position = -1;
            while (!all_printed) {
                Edge *min_edge = nullptr;
                int cur_min_edge_end = -1;
                all_printed = true;

                for_each([&] (Vertex& vertex) -> void {
                    vertex.for_each([&] (Edge& edge) -> void {
                        if (!edge.print_flag) {
                            all_printed = false;
                            int start_pos = std::min(edge.from->print_position, edge.connected->print_position);
                            int end_pos = std::max(edge.from->print_position, edge.connected->print_position);
                            if (std::min(start_pos, end_pos) >= end_position) {
                                if (min_edge == nullptr || end_pos < cur_min_edge_end) {
                                    min_edge = &edge;
                                    cur_min_edge_end = end_pos;
                                }
                            }
                        }
                    });
                });

                if (min_edge == nullptr) {
                    break;
                }
                min_edge->print_flag = true;
                end_position = cur_min_edge_end;

                int edge_start = 1 + std::min(min_edge->from->print_position, min_edge->connected->print_position) * (string_length + arrow_length);
                int edge_end = std::max(min_edge->from->print_position, min_edge->connected->print_position) * (string_length + arrow_length);
                for (int i = edge_start; i < edge_end; i++) {
                    line[i] = '-';
                }

                std::string edge_str = edge_to_string(min_edge->data);
                int mid = (edge_end + edge_start - (int) edge_str.length()) / 2;
                for (char c : edge_str) {
                    line[mid++] = c;
                }

                if (min_edge->from->print_position < min_edge->connected->print_position) {
                    line[edge_end - 1] = '>';
                } else {
                    line[edge_start] = '<';
                }
            }

            std::cout << line << "\n";
            delete[] line;
        }
    };
};

#endif