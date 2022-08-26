#ifndef BINARY_SEARCH_TREE_H_
#define BINARY_SEARCH_TREE_H_

#include <cmath>
#include <memory>
#include <random>
#include <stdexcept>

#include "UnionFind.h"

template<class K, class V>
class BinarySearchTree
{
public:
  using Node = std::shared_ptr<BinarySearchTree<K, V>>;

  BinarySearchTree(K key, V val);

  BinarySearchTree(const BinarySearchTree<K, V>& other);

  Node node();

  K key() const;

  V value() const;

  Node parent() const;

  Node left() const;

  Node right() const;

  Node root() const;

protected:
  void set_ptrs(Node parent, Node left, Node right);

  void set_parent(Node parent);

  void set_left(Node left);

  void set_right(Node right);

  void set_values(K key, V value);

protected:
  int _size = 1;

private:
  K _key;
  V _value;

  Node _parent = nullptr;
  Node _left = nullptr;
  Node _right = nullptr;

public:
  static Node MakeNode(K key, V value);

  static Node Search(Node root, K key);

  static Node Min(Node root);

  static Node Max(Node root);

  static Node Predecessor(Node node);

  static Node Successor(Node node);

  static Node Select(Node root, int index);

  static int Rank(Node root, Node node);

  static int Rank(Node root, K key);

  static void Inorder(Node root, std::vector<Node>& nodes);

  static void RandomTraverse(Node root, std::vector<Node>& nodes);

  static Node InsertNode(Node root, K key, V value);

  static Node InsertNode(Node root, Node node);

  static Node DeleteNode(Node to_delete);

  static Node DeleteNode(Node root, K to_delete);

private:
  static Node RotateLeft(Node to_rotate);

  static Node RotateRight(Node to_rotate);

  static int _Rank(Node node, K key);

  static bool _Transplant(Node node, Node xplant_with);

  static void _DecrementSize(Node to_delete);

  static void _IncrementSize(Node toAdd);
};

template<class K, class V>
BinarySearchTree<K, V>::BinarySearchTree(K key, V val)
  : _key(key)
  , _value(val)
{
}

template<class K, class V>
BinarySearchTree<K, V>::BinarySearchTree(const BinarySearchTree<K, V>& other)
  : _key(other.key())
  , _value(other.value())
{
  _parent = other.parent();
  _left = other.left();
  _right = other.right();
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::node()
{
  return this;
}

template<class K, class V>
void
BinarySearchTree<K, V>::set_ptrs(Node parent, Node left, Node right)
{
  _parent = parent;
  _left = left;
  _right = right;
}

template<class K, class V>
void
BinarySearchTree<K, V>::set_parent(Node parent)
{
  _parent = parent;
}

template<class K, class V>
void
BinarySearchTree<K, V>::set_left(Node left)
{
  _left = left;
}

template<class K, class V>
void
BinarySearchTree<K, V>::set_right(Node right)
{
  _right = right;
}

template<class K, class V>
void
BinarySearchTree<K, V>::set_values(K key, V value)
{
  _key = key;
  _value = value;
}

template<class K, class V>
K
BinarySearchTree<K, V>::key() const
{
  return _key;
}

template<class K, class V>
V
BinarySearchTree<K, V>::value() const
{
  return _value;
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::parent() const
{
  return _parent;
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::left() const
{
  return _left;
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::right() const
{
  return _right;
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::RotateLeft(Node to_rotate)
{
  auto rightSubtree = to_rotate->right();

  if (rightSubtree != nullptr) {
    to_rotate->set_right(rightSubtree->left());
    rightSubtree->left()->set_parent(to_rotate);
    rightSubtree->set_parent(to_rotate->parent());
  }

  if (to_rotate->parent() == nullptr) {
    // ?
  } else if (to_rotate == to_rotate->parent()->left()) {
    to_rotate->parent()->set_left(rightSubtree);
  } else {
    to_rotate->parent()->set_right(rightSubtree);
  }

  if (rightSubtree != nullptr) {
    rightSubtree->set_left(to_rotate);
  }

  to_rotate->set_parent(rightSubtree);

  return rightSubtree;
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::RotateRight(Node to_rotate)
{
  auto leftSubtree = to_rotate->left();

  to_rotate->set_left(leftSubtree->right());

  if (leftSubtree != nullptr) {
    leftSubtree->right()->set_parent(to_rotate);
    leftSubtree->set_parent(to_rotate->parent());
    leftSubtree->set_right(to_rotate);
  }

  if (to_rotate->parent() == nullptr) {
    // ?
  } else if (to_rotate == to_rotate->parent()->left()) {
    to_rotate->parent()->set_left(leftSubtree);
  } else {
    to_rotate->parent()->set_right(leftSubtree);
  }

  to_rotate->set_parent(leftSubtree);

  return leftSubtree;
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::Search(Node root, K key)
{
  if (key == root->key()) {
    return root;
  } else if (key < root->key()) {
    if (root->left() == nullptr) {
      return nullptr;
    } else {
      return BinarySearchTree<K, V>::Search(root->left(), key);
    }
  } else {
    if (root->right() == nullptr) {
      return nullptr;
    } else {
      return BinarySearchTree<K, V>::Search(root->right(), key);
    }
  }
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::Min(Node root)
{
  if (root->left() == nullptr) {
    return root;
  }

  return BinarySearchTree<K, V>::Min(root->left());
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::Max(Node root)
{
  if (root->right() == nullptr) {
    return root;
  }

  return BinarySearchTree<K, V>::Max(root->right());
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::Predecessor(Node node)
{
  if (node == nullptr) {
    return nullptr;
  }

  if (node->left() != nullptr) {
    return BinarySearchTree<K, V>::Max(node->left());
  }

  auto iter1 = node->parent();
  auto iter2 = node;

  while (iter1 != nullptr && iter2 == iter1->left()) {
    iter2 = iter1;
    iter1 = iter1->parent();
  }

  return iter1;
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::Successor(Node node)
{
  if (node == nullptr) {
    return nullptr;
  }

  if (node->right() != nullptr) {
    return BinarySearchTree<K, V>::Min(node->right());
  }

  auto iter1 = node->parent();
  auto iter2 = node;

  while (iter1 != nullptr && iter2 == iter1->right()) {
    iter2 = iter1;
    iter1 = iter1->parent();
  }

  return iter1;
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::InsertNode(Node root, K key, V value)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  auto newNode = BinarySearchTree<K, V>::MakeNode(key, value);

  BinarySearchTree<K, V>::Node newParent = nullptr;
  BinarySearchTree<K, V>::Node iter = root;

  while (iter != nullptr) {
    newParent = iter;

    if (key < iter->key()) {
      iter = iter->left();
    } else if (key > iter->key()) {
      iter = iter->right();
    } else {
      iter = iter->right();
    }
  }

  newNode->set_parent(newParent);

  if (newNode->key() < newParent->key()) {
    newParent->set_left(newNode);
  } else {
    newParent->set_right(newNode);
  }

  BinarySearchTree<K, V>::_IncrementSize(newNode);

  return newNode;
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::InsertNode(Node root, Node node)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  BinarySearchTree<K, V>::Node newParent = nullptr;
  BinarySearchTree<K, V>::Node iter = root;

  while (iter != nullptr) {
    newParent = iter;

    if (node->key() < iter->key()) {
      iter = iter->left();
    } else if (node->key() > iter->key()) {
      iter = iter->right();
    } else {
      iter = iter->right();
    }
  }

  node->set_parent(newParent);

  if (node->key() < newParent->key()) {
    newParent->set_left(node);
  } else {
    newParent->set_right(node);
  }

  BinarySearchTree<K, V>::_IncrementSize(node);

  return node;
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::DeleteNode(Node to_delete)
{
  auto deletedRoot = false;
  if (to_delete->left() == nullptr) {

    deletedRoot =
      BinarySearchTree<K, V>::_Transplant(to_delete, to_delete->right());

    BinarySearchTree<K, V>::_DecrementSize(to_delete);

    if (deletedRoot) {
      return to_delete->right();
    }
  } else if (to_delete->right() == nullptr) {

    deletedRoot =
      BinarySearchTree<K, V>::_Transplant(to_delete, to_delete->left());

    BinarySearchTree<K, V>::_DecrementSize(to_delete);

    if (deletedRoot) {
      return to_delete->left();
    }
  } else {
    auto pred = BinarySearchTree<K, V>::Predecessor(to_delete);

    to_delete->set_values(pred->key(), pred->value());

    deletedRoot = BinarySearchTree<K, V>::_Transplant(pred, pred->left());

    BinarySearchTree<K, V>::_DecrementSize(pred);
  }

  return to_delete;
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::DeleteNode(Node root, K k)
{
  auto to_delete = BinarySearchTree<K, V>::Search(root, k);
  if (to_delete == nullptr) {
    return nullptr;
  }
  auto deletedRoot = false;

  if (to_delete->left() == nullptr) {

    deletedRoot =
      BinarySearchTree<K, V>::_Transplant(to_delete, to_delete->right());

    BinarySearchTree<K, V>::_DecrementSize(to_delete);

    if (deletedRoot) {
      return to_delete->right();
    }
  } else if (to_delete->right() == nullptr) {

    deletedRoot =
      BinarySearchTree<K, V>::_Transplant(to_delete, to_delete->left());

    BinarySearchTree<K, V>::_DecrementSize(to_delete);

    if (deletedRoot) {
      return to_delete->left();
    }
  } else {
    auto pred = BinarySearchTree<K, V>::Predecessor(to_delete);

    to_delete->set_values(pred->key(), pred->value());

    deletedRoot = BinarySearchTree<K, V>::_Transplant(pred, pred->left());

    BinarySearchTree<K, V>::_DecrementSize(pred);
  }

  return to_delete;
}

template<class K, class V>
bool
BinarySearchTree<K, V>::_Transplant(Node xplant, Node xplant_with)
{
  if (xplant != nullptr) {
    if (xplant->parent() != nullptr) {
      if (xplant->parent()->left() == xplant) {
        xplant->parent()->set_left(xplant_with);
      } else {
        xplant->parent()->set_right(xplant_with);
      }

      if (xplant_with != nullptr) {
        xplant_with->set_parent(xplant->parent());
      }
    } else {
      if (xplant_with != nullptr) {
        xplant_with->set_parent(nullptr);
        return true;
      }
    }
  }

  return false;
}

template<class K, class V>
void
BinarySearchTree<K, V>::_DecrementSize(Node to_delete)
{
  auto size_iter = to_delete->parent();
  while (size_iter != nullptr) {
    size_iter->_size -= 1;
    size_iter = size_iter->parent();
  }
}

template<class K, class V>
void
BinarySearchTree<K, V>::_IncrementSize(Node toAdd)
{
  auto size_iter = toAdd->parent();
  while (size_iter != nullptr) {
    size_iter->_size += 1;
    size_iter = size_iter->parent();
  }
}

template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::Select(Node root, int index)
{
  if (root == nullptr) {
    return nullptr;
  }

  int leftSize = 0;

  if (root->left() != nullptr) {
    leftSize = root->left()->_size;
  }

  if (leftSize == index) {
    return root;
  }

  else if (leftSize > index) {
    return BinarySearchTree<K, V>::Select(root->left(), index);
  }

  else {
    if (root->right() != nullptr) {
      return BinarySearchTree<K, V>::Select(root->right(),
                                            index - leftSize - 1);
    }

    return root->right();
  }
}

template<class K, class V>
int
BinarySearchTree<K, V>::Rank(Node root, Node node)
{
  if (root != nullptr && node != nullptr &&
      BinarySearchTree<K, V>::Search(root, node->key()) != nullptr) {
    return BinarySearchTree<K, V>::_Rank(root, node->key());
  }
  return -1;
}

template<class K, class V>
int
BinarySearchTree<K, V>::Rank(Node root, K key)
{
  if (root != nullptr && BinarySearchTree<K, V>::Search(root, key) != nullptr) {
    return BinarySearchTree<K, V>::_Rank(root, key);
  }

  return -1;
}

template<class K, class V>
void
BinarySearchTree<K, V>::Inorder(Node root, std::vector<Node>& nodes)
{
  if (root == nullptr) {
    return;
  }

  BinarySearchTree<K, V>::Inorder(root->left(), nodes);
  nodes.push_back(root);
  BinarySearchTree<K, V>::Inorder(root->right(), nodes);
}

template<class K, class V>
void
BinarySearchTree<K, V>::RandomTraverse(Node root, std::vector<Node>& nodes)
{
  if (root == nullptr) {
    return;
  }
  std::random_device rd;
  std::mt19937 gen(rd());

  auto left_first = gen() % 1000 < 500;

  if (left_first) {
    BinarySearchTree<K, V>::RandomTraverse(root->left(), nodes);
    nodes.push_back(root);
    BinarySearchTree<K, V>::RandomTraverse(root->right(), nodes);
  } else {
    BinarySearchTree<K, V>::RandomTraverse(root->right(), nodes);
    nodes.push_back(root);
    BinarySearchTree<K, V>::RandomTraverse(root->left(), nodes);
  }
}

// allocates a new binary search tree
template<class K, class V>
typename BinarySearchTree<K, V>::Node
BinarySearchTree<K, V>::MakeNode(K key, V value)
{
  return std::make_shared<BinarySearchTree<K, V>>(key, value);
}

template<class K, class V>
int
BinarySearchTree<K, V>::_Rank(Node node, K key)
{
  if (node == nullptr) {
    return -1;
  }

  auto iter = node;
  int rnk = 0;

  while (iter != nullptr) {
    if (iter->key() < key) {
      if (iter->left() != nullptr) {
        rnk += 1 + iter->left()->_size;
      } else {
        rnk += 1;
      }
      iter = iter->right();
    } else if (iter->key() > key) {
      iter = iter->left();
    } else {
      if (iter->left() != nullptr) {
        return (rnk + iter->left()->_size);
      } else {
        return rnk;
      }
    }
  }

  return -1;
}

#endif // BinarySearchTree.h included
