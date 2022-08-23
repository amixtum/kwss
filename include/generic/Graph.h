#ifndef GRAPH_H_
#define GRAPH_H_

#include <climits>
#include <cstdint>
#include <map>
#include <queue>
#include <random>
#include <vector>

#include "Heap.h"

template<class VertexType>
class Graph
{
public:
  Graph();

  Graph(Graph<VertexType>& copy_from);

  void clear();

  void add_vertex(const VertexType& v);

  void remove_vertex(const VertexType& v);

  void add_edge(const VertexType& from, const VertexType& to, int weight);

  void addNewEdge(const VertexType& from, const VertexType& to, int weight);

  void remove_edge(const VertexType& from, const VertexType& to);

  bool has_edge(const VertexType& from, const VertexType& to);

  bool is_sink(const VertexType& from);

  int weight(const VertexType& from, const VertexType& to);

  std::map<VertexType, std::map<VertexType, int>>& get_map();

  std::map<VertexType, int>& edges(const VertexType& from);

  Graph<VertexType> bfs(VertexType& src_vertex);

  void contract_rand_edge_no_parallel();

  void contract_edge_no_parallel(const std::pair<VertexType, VertexType> edge);

  std::pair<VertexType, VertexType> choose_rand_edge();

  std::pair<VertexType, VertexType> choose_rand_edge(VertexType& start_vertex);

private:
  std::map<VertexType, std::map<VertexType, int>> _map;
};

template<class VertexType>
Graph<VertexType>::Graph()
{
}

template<class VertexType>
Graph<VertexType>::Graph(Graph<VertexType>& copy_from)
{
  _map.clear();

  for (auto& elem : copy_from.get_map()) {
    add_vertex(elem.first);
    for (auto& destVertex : elem.second) {
      addNewEdge(elem.first, destVertex.first, destVertex.second);
    }
  }
}

template<class VertexType>
void
Graph<VertexType>::clear()
{
  _map.clear();
}

template<class VertexType>
void
Graph<VertexType>::add_vertex(const VertexType& v)
{
  _map.insert({ v, std::map<VertexType, int>() });
}

template<class VertexType>
void
Graph<VertexType>::remove_vertex(const VertexType& v)
{
  for (auto& src_vertex : _map) {
    auto toRemove = src_vertex.second.find(v);
    if (toRemove != src_vertex.second.end()) {
      src_vertex.second.erase(toRemove);
    }
  }

  _map.erase(v);
}

template<class VertexType>
void
Graph<VertexType>::add_edge(const VertexType& from,
                            const VertexType& to,
                            int weight)
{
  auto findTo = _map[from].find(to);
  if (findTo != _map[from].end()) {
    _map[from][to] += weight;
  } else {
    _map[from].insert({ to, weight });
  }
}

template<class VertexType>
void
Graph<VertexType>::addNewEdge(const VertexType& from,
                              const VertexType& to,
                              int weight)
{
  _map[from].insert({ to, weight });
}

template<class VertexType>
void
Graph<VertexType>::remove_edge(const VertexType& from, const VertexType& to)
{
  _map[from].erase(to);
}

template<class VertexType>
bool
Graph<VertexType>::has_edge(const VertexType& from, const VertexType& to)
{
  return _map[from].find(to) != _map[from].end();
}

template<class VertexType>
bool
Graph<VertexType>::is_sink(const VertexType& from)
{
  return _map[from].size() > 0;
}

template<class VertexType>
int
Graph<VertexType>::weight(const VertexType& from, const VertexType& to)
{
  return _map[from][to];
}

template<class VertexType>
std::map<VertexType, std::map<VertexType, int>>&
Graph<VertexType>::get_map()
{
  return _map;
}

template<class VertexType>
std::map<VertexType, int>&
Graph<VertexType>::edges(const VertexType& from)
{
  return _map[from];
}

template<class VertexType>
Graph<VertexType>
Graph<VertexType>::bfs(VertexType& src_vertex)
{
  std::queue<VertexType> toSearch;
  std::map<VertexType, bool> visited;
  Graph<VertexType> connected;

  for (auto& vertex : _map) {
    visited.insert(std::make_pair(vertex.first, false));
  }

  visited[src_vertex] = true;

  toSearch.push(src_vertex);

  while (toSearch.size() > 0) {
    auto vertex = toSearch.front();
    toSearch.pop();

    connected.add_vertex(vertex);

    for (auto& destVertex : edges(vertex)) {
      connected.addNewEdge(vertex, destVertex.first, destVertex.second);

      if (!visited[destVertex.first]) {
        visited[destVertex.first] = true;

        toSearch.push(destVertex.first);
      }
    }
  }

  return connected;
}

template<class VertexType>
void
Graph<VertexType>::contract_rand_edge_no_parallel()
{
  auto edge = choose_rand_edge();

  for (auto& destVertex : _map[edge.second]) {
    auto edgeList = _map[edge.first];
    if (edgeList.find(destVertex.first) == edgeList.end()) {
      addNewEdge(edge.first,
                 destVertex.first,
                 weight(edge.first, edge.second) +
                   weight(edge.second, destVertex.first));
    }

    remove_edge(edge.second, destVertex.first);
  }

  for (auto& src_vertex : _map) {
    auto it = _map[src_vertex.first].find(edge.second);
    if (it != _map[src_vertex.first].end()) {
      if (_map[src_vertex.first].find(edge.first) !=
          _map[src_vertex.first].end()) {
        addNewEdge(
          src_vertex.first, edge.first, weight(src_vertex.first, edge.second));
      }

      remove_edge(src_vertex.first, edge.second);
    }
  }

  remove_vertex(edge.second);
}

template<class VertexType>
void
Graph<VertexType>::contract_edge_no_parallel(
  const std::pair<VertexType, VertexType> edge)
{
  for (auto& destVertex : _map[edge.second]) {
    if (_map[edge.first].find(destVertex.first) == _map[edge.first].end()) {
      addNewEdge(edge.first,
                 destVertex.first,
                 weight(edge.first, edge.second) +
                   weight(edge.second, destVertex.first));
    }

    remove_edge(edge.second, destVertex.first);
  }

  for (auto& src_vertex : _map) {
    auto it = _map[src_vertex.first].find(edge.second);
    if (it != _map[src_vertex.first].end()) {
      if (_map[src_vertex.first].find(edge.first) !=
          _map[src_vertex.first].end()) {
        addNewEdge(
          src_vertex.first, edge.first, weight(src_vertex.first, edge.second));
      }

      remove_edge(src_vertex.first, edge.second);
    }
  }

  remove_vertex(edge.second);
}

template<class VertexType>
std::pair<VertexType, VertexType>
Graph<VertexType>::choose_rand_edge()
{
  std::random_device rd;
  std::mt19937 gen(rd());

  auto vIt = get_map().begin();
  std::forward(vIt, gen() % get_map().size());
  auto start_vertex = vIt->first;

  auto eIt = vIt->second.begin();
  while (eIt != vIt->second.end()) {
    vIt = get_map().begin();
    std::forward(vIt, gen() % get_map().size());
    start_vertex = vIt->first;
    eIt = vIt->second.begin();
  }

  std::forward(eIt, gen() % vIt->second.size());
  auto endVertex = eIt->first;

  return std::make_pair(start_vertex, endVertex);
}

template<class VertexType>
std::pair<VertexType, VertexType>
Graph<VertexType>::choose_rand_edge(VertexType& start_vertex)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  auto eIt = _map[start_vertex].begin();

  std::forward(eIt, gen() % _map[start_vertex].size());
  auto endVertex = eIt->first;

  return std::make_pair(start_vertex, endVertex);
}

#endif // Graph.h included
