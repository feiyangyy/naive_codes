#ifndef GENREAL_H_
#define GENREAL_H_ 1
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <fstream>

template <typename T>
std::vector<T> CreateVectorFromFile(const std::string& file) {
  std::fstream ifs(file);
  if (!ifs.is_open())
  {
    return {};
  }
  int nb_number;
  ifs >> nb_number;
  std::vector<T> res(nb_number);
  int idx = 0;
  while (ifs >> nb_number)
  {
    res[idx++] = nb_number;
    if (idx >= res.size())
    {
      break;
    }
  }
  ifs.close();
  return res;
}

template <typename T>
struct LinkList {
  T val;
  LinkList* node = nullptr;
};

template <typename T>
struct BinaryTree {
  T val;
  BinaryTree* left = nullptr;
  BinaryTree* right = nullptr;
};

std::vector<int> GenreateRandom(int n, int min, int max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min, max);
  std::vector<int> result(n);
  std::for_each(result.begin(), result.end(), [&dis, &gen](int& n){n = dis(gen);});
  return result;
}
template <typename T>
void PrintVector(std::vector<T> vec, int w) {
  if(!w) {
    w = 10;
  }
  int cnt = 0;
  for(const auto &v :vec){
    std::cout << v << " ";
    ++cnt;
    if(cnt == 10) {
      cnt = 0;
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}

template <typename T, typename U>
struct LinkListKV {
  // default
  LinkListKV(){}
  // with variable.
  LinkListKV(const T& k, const U& v):key_(k), val_(v), next_(nullptr){}
  // key
  T key_;
  // optional
  U val_;
  LinkListKV<T, U> *next_;
};


template<typename K, typename V>
struct BSTNode {
  BSTNode(const K& k, const V& v):k_(k), v_(v) {
  }
  ~BSTNode() {
  }

  BSTNode<K, V> *left_ = nullptr;
  BSTNode<K, V> *right_ = nullptr;
  K k_;
  V v_;
  uint32_t n_ = 1;
};

template <typename K, typename V>
struct BST {

  using NodeType = BSTNode<K, V>;

  NodeType* GetImpl(BSTNode<K, V> *node, const K& key, V *out) {
    if(!node) {
      return nullptr;
    }
    if(node->k_ == key) {
      *out = node->v_;
      return node;
    } else if(key < node->k_) {
      return GetImpl(node->left_, k,  out);
    } else {
      return GetImpl(node->right_, k, out);
    }
  }

  NodeType* Get(const K& key, V *out) {
    return GetImpl(root_, key, out);
  }
  /**
   * 基于链表的特点，BST的构造过程中不需要移动元素。二分查找构造有序线性表的最主要开销在于移动元素
   * 并且二分查找基于随机访问的需求无法应用于有序链表中
   * 有分析可以证明，BST的查找性能稍微逊色于二分查找，但插入性能远高于二分查找的顺序线性表
   */
  BSTNode<K, V> * PutImpl(BSTNode<K, V> *node, const K&key, const V& v) {
    // assume that node != nullptr
    // assert(node);
    if(!node) {
      return new BST<K, V>(key, v);
    }
    if (key == node->k_) {
      node->v_ = v;
      return node;
    }
    if(key < node->k_) {
      node->left_ = PutImpl(node->left_, key, v);
    } else {
      node->right_ = PutImpl(node->right_, key, v);
    }
    node->n_ = (node->left_?node->left_->n_:0) + (node->right_?node->right_->n_:0) + 1;
    return node;
  }

  void Put(const K& key, const V& v) {
    root_ = PutImpl(root_, key, v);
  }

  NodeType* MinImpl(NodeType* node) {
    // assert(node);
    if(!node) {
      return nullptr;
    }
    if(!node->left_) {
      return node;
    }
    return MinImpl(node->left_);
  }

  NodeType* Min(){
    if(!root_) {
      return nullptr;
    }
    return MinImpl(root_);
  }

  NodeType* MaxImpl(NodeType* node) {
    if(!node) {
      return nullptr;
    }
    if(!node->right_) {
      return node;
    }
    return MaxImpl(node->right_);
  } 

  NodeType* Max(){
    if(!root_) {
      return nullptr;
    }
    return MaxImpl(root_);
  }

  // key < cur, floor(key) in left tree
  K FloorImpl(const K& key, NodeType* node, bool* fnd){
    if(!node) {
      return K{};
    }
    if(key < node->k_) {
      return FloorImpl(key, node->left_, fnd);
    }
    if(key>node->k_) {
      K res = FloorImpl(key, node->right_, fnd);
      if(fnd) {
        return res;
      }
    }
    // key == node->k or can't find key in the right tree.
    fnd = true;
    return node->k_;
  }

  K Floor(const K& key, bool* fnd){
    return FloorImpl(key, root_, fnd);
  }

  K CeilingImpl(const K& key, NodeType* node, bool* fnd) {
    if(!node) {
      return K{};
    }
    if(key > node->k_) {
      return CeilingImpl(key,node->right_, fnd);
    }
    if(key < node->k_) {
      K res = CeilingImpl(key, node->left_, fnd);
      if(fnd) {
        return res;
      }
    }
    fnd = true;
    return node->k_;
  }

  NodeType* DeleteMinImpl(NodeType* node, bool do_not_delete){
    if(!node) {
      return node;
    }
    if(node->left_) {
      node->left_ = DeleteMinImpl(node->left_, DeleteMaxImpl);
      node->n_ = ((node->left_)? node->left_->n_:0 ) + ((node->right_)? node->right_->n_ : 0) + 1;
      return node;
    }
    NodeType* res = node->right_;
    delete node;
    return res;
  }

  NodeType* DeleteMaxImpl(NodeType* node, bool do_not_delete) {
    if(!node) {
      return node;
    }
    if(node->right_) {
      node->right_ = DeletMaxImpl(node->right_);
      node->n_ = ((node->left_)? node->left_->n_:0 ) + ((node->right_)? node->right_->n_ : 0) + 1;
      return node;
    }
    NodeType* res = node->left_;
    delete node;
    return res;
  }

  void DeleteMin(bool dont_delete){
    root_ = DeleteMinImpl(root_, dont_delete);
  }

  void DeleteMax(bool dont_delete) {
    root_ = DeleteMaxImpl(root_, dont_delete);
  }

  NodeType* DeleteImpl(const K& key, NodeType* node) {
    if(!node) {
      return nullptr;
    }
    if(key < node->k_) {
      node->left_= DeleteImpl(key, node->left_);
      return node;
    } else if(key > node->k_) {
      node->right_ =  DeleteImpl(key, node->right_);
      return node;
    }
    NodeType* result = node->left_;
    if(!node->right_) {
      result = node->left_;
      delete node;
    } else if(!node->left_) {
      result =node->right_;
      delete node;
    } else {
      // 1. 找到后继节点(只有左右都不为空的时候才需要调整)
      NodeType* succ = MinImpl(node->right_);
      assert(succ);
      if(succ) {
        DeleteMinImpl(node->right_, true);
        succ->left_ = node->left_;
        succ->right_ = node->right_;
        result =  succ;
      }
    }
    delete node;
    return result;
  }
  BSTNode<K, V> *root_ = nullptr;
};

#endif
