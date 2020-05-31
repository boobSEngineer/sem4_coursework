#include "gtest/gtest.h"
#include "../array.h"
#include "../flow_network.h"


// test basic graph functionality
TEST (graph, build_test) {
    Graph<std::string, std::string> graph;
    Graph<std::string, std::string>::Vertex& v1 = graph.add_vertex("A");
    ASSERT_EQ(graph.size(), 1);
    Graph<std::string, std::string>::Vertex& v2 = graph.add_vertex("B");
    ASSERT_EQ(graph.size(), 2);
    ASSERT_EQ(&v1, graph.get_vertex("A"));
    ASSERT_EQ(&v2, graph.get_vertex("B"));
    graph.connect(v1, v2, "Edge");
    ASSERT_EQ(graph.get_edge(v1, v2)->data, "Edge");
    graph.clear();
    ASSERT_EQ(graph.size(), 0);
}

TEST (graph, iteration_test) {
    Graph<int, int> graph;
    for (int i = 0; i < 10; i++) {
        Graph<int, int>::Vertex& v1 = graph.add_vertex(i);
        for (int j = 0; j < i; j++) {
            auto v2 = graph.get_vertex(j);
            ASSERT_NE(v2, nullptr);
            graph.connect(*v2, v1, i * j);
        }
    }

    ASSERT_EQ(graph.size(), 10);

    graph.for_each([&] (Graph<int, int>::Vertex& v) -> void {
        v.for_each([&](Graph<int, int>::Edge &edge) -> void {
            ASSERT_LE(edge.from->data, edge.connected->data);
            ASSERT_EQ(edge.data, edge.from->data * edge.connected->data);
        });
    });
}


// flow networks test
TEST(flow_network, build) {
    FlowNetwork network;
    ASSERT_TRUE(network.parse_edge("S T 10"));
    ASSERT_FALSE(network.parse_edge("A 10"));
    ASSERT_EQ(network.graph.size(), 2);
    FlowNetwork::Vertex* v1 = network.graph.get_vertex("S");
    FlowNetwork::Vertex* v2 = network.graph.get_vertex("T");
    ASSERT_NE(v1, nullptr);
    ASSERT_NE(v2, nullptr);
    ASSERT_EQ(v1, network.source);
    ASSERT_EQ(v2, network.target);
    FlowNetwork::Edge* edge1 = network.graph.get_edge(*v1, *v2);;
    FlowNetwork::Edge* edge2 = network.graph.get_edge(*v2, *v1);
    ASSERT_NE(edge1, nullptr);
    ASSERT_EQ(edge2, nullptr);
    ASSERT_EQ(edge1->data.max_flow, 10);
}

TEST(flow_network, validate_ok) {
    FlowNetwork network;
    ASSERT_TRUE(network.parse_edge("S A 1"));
    ASSERT_TRUE(network.parse_edge("S B 2"));
    ASSERT_TRUE(network.parse_edge("A B 3"));
    ASSERT_TRUE(network.parse_edge("B T 4"));
    ASSERT_EQ(network.is_valid(), true);
}

TEST(flow_network, validate_invalid_1) {
    FlowNetwork network;
    ASSERT_TRUE(network.parse_edge("S A 1"));
    ASSERT_TRUE(network.parse_edge("S B 2"));
    ASSERT_TRUE(network.parse_edge("A B 3"));
    ASSERT_EQ(network.is_valid(), false);

    network.clear();
    ASSERT_TRUE(network.parse_edge("A T 1"));
    ASSERT_TRUE(network.parse_edge("T B 2"));
    ASSERT_TRUE(network.parse_edge("A B 3"));
    ASSERT_EQ(network.is_valid(), false);
}

TEST(flow_network, validate_invalid_2) {
    FlowNetwork network;
    ASSERT_TRUE(network.parse_edge("S A 1"));
    ASSERT_TRUE(network.parse_edge("T S 2"));
    ASSERT_TRUE(network.parse_edge("A T 3"));
    ASSERT_EQ(network.is_valid(), false);
}

TEST(flow_network, validate_invalid_3) {
    FlowNetwork network;
    ASSERT_TRUE(network.parse_edge("S A 1"));
    ASSERT_TRUE(network.parse_edge("T B 3"));
    ASSERT_TRUE(network.parse_edge("A B 2"));
    ASSERT_EQ(network.is_valid(), false);
}

TEST(flow_network, dfs_success) {
    FlowNetwork network;
    ASSERT_TRUE(network.parse_edge("S A 1"));
    ASSERT_TRUE(network.parse_edge("A B 1"));
    ASSERT_TRUE(network.parse_edge("B C 1"));
    ASSERT_TRUE(network.parse_edge("C T 1"));
    Array<FlowNetwork::Edge*> path;
    ASSERT_EQ(network.search_path(network.source, path), true);
    ASSERT_EQ(path.length(), 4);
}

TEST(flow_network, dfs_fail) {
    FlowNetwork network;
    ASSERT_TRUE(network.parse_edge("S A 1"));
    ASSERT_TRUE(network.parse_edge("A B 1"));
    ASSERT_TRUE(network.parse_edge("C T 1"));
    Array<FlowNetwork::Edge*> path;
    ASSERT_EQ(network.search_path(network.source, path), false);
}

TEST(flow_network, max_flow_build) {
    FlowNetwork network;
    ASSERT_TRUE(network.parse_edge("S A 1"));
    ASSERT_TRUE(network.parse_edge("A B 1"));
    ASSERT_TRUE(network.parse_edge("B T 1"));
    ASSERT_TRUE(network.parse_edge("S T 2"));
    ASSERT_EQ(network.is_valid(), true);
    network.build_max_flow();
    ASSERT_EQ(network.vertex_flow(*network.source), -3);
    ASSERT_EQ(network.vertex_flow(*network.target), 3);
    network.graph.for_each([&] (FlowNetwork::Vertex& vertex) -> void {
        if (&vertex != network.source && &vertex != network.target) {
            ASSERT_EQ(network.vertex_flow(vertex), 0);
        }
    });
}

// array tests
TEST(Array, MassiveAddAndClearTest) {
    Array<std::string> array;
    const int size = 10000;
    for (int i = 0; i < size; i++) {
        array.add("test_string");
    }
    ASSERT_EQ(array.length(), size);
    for (int i = 0; i < size; i++) {
        ASSERT_EQ(array[i], "test_string");
    }
    array.clear();
    ASSERT_EQ(array.length(), 0);
}
