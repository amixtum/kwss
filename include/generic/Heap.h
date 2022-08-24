#ifndef HEAP_H_
#define HEAP_H_

#include <climits>
#include <cmath>
#include <vector>

template<class K, class T>
class Heap
{
public:
  Heap();

  void insert(K key, T item);

  T extract_min();

  void heapify();

  bool empty();

private:
  int parent(int index);

  int left_child(int index);

  int right_child(int index);

  void swap(int left, int right);

private:
  std::vector<std::pair<K, T>> _heap;
};

template<class K, class T>
Heap<K, T>::Heap()
{
  _heap.push_back({ INT_MAX, T() });
}

template<class K, class T>
void
Heap<K, T>::insert(K key, T item)
{
  _heap.push_back({ key, item });
  auto itemIndex = _heap.size() - 1;

  while (itemIndex > 1 &&
         _heap[itemIndex].first < _heap[parent(itemIndex)].first) {
    swap(itemIndex, parent(itemIndex));
    itemIndex = parent(itemIndex);
  }
}

template<class K, class T>
T
Heap<K, T>::extract_min()
{
  auto minimum = _heap[1].second;
  _heap[1] = _heap[_heap.size() - 1];
  _heap.pop_back();

  auto outerIndex = 1;
  auto innerIndex = 0;
  auto isHeap = false;

  while (!isHeap && outerIndex * 2 < _heap.size()) {
    innerIndex = left_child(outerIndex);

    if (innerIndex < _heap.size() - 1) {
      if (_heap[innerIndex].first > _heap[innerIndex + 1].first) {
        innerIndex += 1;
      }
    }

    // not a min _heap
    if (_heap[innerIndex].first < _heap[outerIndex].first) {
      swap(innerIndex, outerIndex);
      outerIndex = innerIndex;
    } else {
      isHeap = true;
    }
  }

  return minimum;
}

template<class K, class T>
void
Heap<K, T>::heapify()
{
  for (auto index = std::floor(_heap.size() / 2); index > 0; index -= 1) {
    auto outerIndex = index;
    auto innerIndex = index;
    auto isHeap = false;

    while (!isHeap && outerIndex * 2 < _heap.size()) {
      innerIndex = left_child(outerIndex);

      if (innerIndex < _heap.size() - 1) {
        if (_heap[innerIndex].first > _heap[innerIndex + 1].first) {
          innerIndex += 1;
        }
      }

      // not a min _heap
      if (_heap[innerIndex].first < _heap[outerIndex].first) {
        swap(innerIndex, outerIndex);
        outerIndex = innerIndex;
      } else {
        isHeap = true;
      }
    }
  }
}

template<class K, class T>
bool
Heap<K, T>::empty()
{
  return _heap.size() == 0;
}

template<class K, class T>
int
Heap<K, T>::parent(int index)
{
  return std::floor(((index) / 2));
}

template<class K, class T>
int
Heap<K, T>::left_child(int index)
{
  return ((index)*2);
}

template<class K, class T>
int
Heap<K, T>::right_child(int index)
{
  return (index)*2 + 1;
}

template<class K, class T>
void
Heap<K, T>::swap(int left, int right)
{
  auto temp = _heap[left];
  _heap[left] = _heap[right];
  _heap[right] = temp;
}

#endif // Heap.h included
