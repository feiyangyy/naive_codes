#include "../general.h"
#include "CLI/CLI.hpp"
#define BLOCK_MAIN
#include "../heap.cc"

struct ListSearch{
  typedef LinkListKV<std::string, int> LType;
  LinkListKV<std::string, int> *prev_ = nullptr;
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
      prv = prev_->next_;
    }
    if(!found) {
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
        return;
      }
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
  while (!ifs.eof())
  {
    ifs >> word;
    printf("Word:%s\n", word.c_str());
    if (ls.Get(word, v))
    {
      ls.Put(word, 1);
    }
    else
    {
      ls.Put(word, v + 1);
    }
  }
  ls.GetTop10();
  printf("OK!\n");
  return 0;
}

int main(int argc, char** argv) {
  CLI::App app{"Heap implementation"};
  std::string file;
  uint32_t cnt;
  std::string type;
  app.add_option("-f,--file", file, "InputFile")->required();
  CLI11_PARSE(app, argc, argv);
  return DoWorldCountLS(file);
}