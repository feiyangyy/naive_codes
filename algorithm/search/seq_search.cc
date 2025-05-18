#include "../general.h"
#include "CLI/CLI.hpp"
#define BLOCK_MAIN
#include "../heap.cc"

struct ListSearch{
  typedef LinkListKV<std::string, int> LType;
  LinkListKV<std::string, int> *prev_ = nullptr;
  uint32_t wc = 0;
  ListSearch() {
    prev_ = new LType();
    prev_->next_ = nullptr;
  }

  void Put(const std::string &k, int v){
    bool found = false;
    auto prv = prev_;
    while (prv->next_)
    {
      if (prv->next_->key_ == k)
      {
        found = true;
        break;
      }
      prv = prv->next_;
    }
    if(!found) {
      ++wc;
      prv->next_ = new LType();
      prv->next_->key_ = k;
      prv->next_->val_ = v;
      prv->next_->next_ = nullptr;
      return;
    }
    prv->next_->val_ = v;
  }

  int Get(const std::string &k, int &v) {
    bool found = false;
    auto prv = this->prev_;
    while(prv->next_) {
      if(prv->key_ == k) {
        found = true;
        break;
      }
      prv = prv->next_;
    }
    if(!found) {
      return -1;
    }
    v = prv->next_->val_;
    return 0;
  }

  void Delete(const std::string& key) {
    auto prv = prev_;
    while(prv->next_) {
      if(prv->next_->key_ == key) {
        auto pdel = prv->next_;
        prv->next_ = prv->next_->next_;
        delete pdel;
        --wc;
        return;
      }
      prv = prv->next_;
    }
  }

  static bool CompareNode(const LType& l, const LType& r) {
    return l.val_ >= r.val_;
  }

  void GetTop10() {
    auto prv = prev_;
    auto minh = Heap<LType>(10, CompareNode);
    while (prv->next_) {
      // this is not a good impl, but just let it go.
      if(!minh.Full(10)) {
        minh.Push(*prv->next_);
      }
      else if (prv->next_->val_ > minh.Top().val_) // 此处不用CompareNode 是因为其中等号不太严格，导致过多的copy和pop
      {
        minh.Pop();
        minh.Push(*prv->next_);
      }
      prv = prv->next_;
    }
    int idx = 10;
    while (!minh.Empty())
    {
      auto&& v = minh.Pop();
      printf("Top %d is %s, freq %d\n", idx--, v.key_.c_str(), v.val_);
    }
  }
};

// 文件词频统计顺序查找
int DoWorldCountLS(const std::string& file) {
  std::ifstream ifs(file);
  if(!ifs.is_open()){
    printf("Open file failed!\n");
    return -1;
  }
  std::string word;
  ListSearch ls;
  int v;
  uint32_t word_cnt = 0;
  while (!ifs.eof())
  {
    ++word_cnt;
    ifs >> word;
    // printf("Word:%s\n", word.c_str());
    if (ls.Get(word, v))
    {
      ls.Put(word, 1);
    }
    else
    {
      ls.Put(word, v + 1);
    }
  }
  printf("Total count:%d:%d\n", word_cnt, ls.wc);
  ls.GetTop10();
  printf("OK!\n");
  return 0;
}

#if 0
struct BinarySearch {
  using LType = LinkListKV<std::string, int>;
  // 链表结构，能否进行二分查找? 不能。跳表可以
  LType* ordered_prev_;

};
#endif
template<typename T, typename U>
struct BinarySearch {
  struct Node{
    T key_;
    U val_;
  };

  std::vector<Node> elements_;
  
  BinarySearch(std::size_t sz){
    elements_.reserve(sz);
  }


  // 开闭区间!
  uint32_t Rank(const T& k, uint32_t l, uint32_t r) {
    // 到达边界
    if(l >= r) {
      return l;
    }
    // when r = l+1, mid = l. and if vec[l] != k, we will return l.
    // note, what if a[0].key > k?
    uint32_t mid = (l + r) /2;
    // 到达退出边界的情况
    // 参考case 分析
    #if 0
    if(mid == l) {
       // case 1 a[l].k < k, will go to mid+1, r, and get next pos(l+1 )
       // case 2 a[l].k > k, will go to l mid-1, and get l (insert pos.)
       // case 3, a[l].k = k, we find it. just as the code below.
       // assert(elements_[l].key >= k);
    }
    #endif
    if(k < elements_[mid].key_) {
      return Rank(k, l, mid - 1);
    } else if(k>elements_[mid].key_) {
      return Rank(k, mid + 1, r);
    }
    return mid;
  };

  void Put(const Node& n) {
    uint32_t pos = Rank(n.key_, 0, elements_.size());
    // not in
    // we can't get a pos > size.
    if(elements_.size() <= pos) {
      elements_.insert(elements_.end(), n);
      return;
    } else if( elements_[pos].key_ != n.key_) {
      elements_.insert(elements_.begin() + pos, n);
      return;
    }
    // update
    elements_[pos].val_ = n.val_;
  }

  bool Get(const T& k, Node& n) {
    uint32_t pos =Rank(k,0, elements_.size());
    if(pos >= elements_.size() || elements_.at(pos).key_ != k) {
      return false;
    }
    n = elements_.at(pos);
    return true;
  }

  void Delete(const T& k) {
    // tbd.
  }

  void TopK(uint32_t k){
    auto cmp_node = [](const Node& l, const Node &r) {
      return l.val_ >= r.val_;
    };
    // TopK 改造
    auto min_heap = Heap<Node>(10, cmp_node);
    while(!elements_.empty()) {
      
    }
  }

};


int main(int argc, char** argv) {
  CLI::App app{"Heap implementation"};
  std::string file;
  uint32_t cnt;
  std::string type;
  app.add_option("-f,--file", file, "InputFile")->required();
  CLI11_PARSE(app, argc, argv);
  return DoWorldCountLS(file);
} 
