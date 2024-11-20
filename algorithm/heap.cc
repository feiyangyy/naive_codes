#include <string>
#include <vector>
#include "general.h"
#include <fstream>
#include <functional>
#include <CLI/CLI.hpp>
#include <assert.h>
template <typename T>
bool CompareLE(const T &lhs, const T &rhs)
{
  return lhs < rhs;
}

template <typename T>
bool CompareGE(const T &lhs, const T &rhs)
{
  return lhs > rhs;
}

template <typename T>
struct Heap {
  std::vector<T> q;
  uint32_t cnt;
  // cmp child 对 parent 的偏序关系，大根堆是<= 小根堆是>=
  std::function<bool(const T&, const T&)> fp_cmp = nullptr;
  explicit Heap(uint32_t sz, std::function<bool(const T &, const T &)> cmp)
  {
    fp_cmp = cmp;
    q.reserve(sz + 1);
    // 必须提供默认构造函数
    q.push_back(T{});
    cnt = 0;
  }
  // 下沉，删去一个节点后，从尾部将一个节点放到根节点，并逐步下沉
  /**
   * Warn: 此处需要下沉到最合适位置，对于大根堆来说，要选择子节点中最小的哪个（而不是任意），因为这是大根堆性质要求的
   * 同理，对于小根堆来说，如果要交换，则需要交换子节点中最大的那个，使的交换过后的偏序成立
   * 对于调试而言，我们可以assert 一些必须成立的条件，比如对于堆来说，我们的条件是assert(cmp(q[chdl], q[pos]), cmp(q[chdr], q[pos]))
   */
  void Sink(int pos) {
    // 错误实现
#if 0
    int child = pos << 1;
    if (child > cnt) {
      return;
    }
    if (!fp_cmp(q[child], q[pos])) {
      std::swap(q[child], q[pos]);
      Sink(child);
      assert(fp_cmp(q[child], q[pos]));
      assert(fp_cmp(q[child + 1], q[pos]));
      return;
    }
    child += 1;
    if(child > cnt) {
      return;
    }
    if (!fp_cmp(q[child], q[pos])) {
      std::swap(q[child], q[pos]);
      Sink(child);
      assert(fp_cmp(q[child], q[pos]));
      assert(fp_cmp(q[child - 1], q[pos]));
      return;
    }
#endif
    int lc, rc;
    lc = pos << 1;
    rc = lc + 1;
    int swc;
    // 左右都存在
    if(rc <= cnt) {
      // 左子节点可以作为右子节点的子节点
      if(fp_cmp(q[lc], q[rc])) {
        swc = rc;
      } else {
        swc = lc;
      }
      // 不能作为子节点
      if(!fp_cmp(q[swc], q[pos])) {
        std::swap(q[swc], q[pos]);
        Sink(swc);
      }
      return;
    }
    // left-only
    if(lc <= cnt) {
      // 不能作为子节点
      if(!fp_cmp(q[lc], q[pos])) {
        std::swap(q[lc], q[pos]);
        Sink(lc);
      }
      return;
    }
  }
  // 上浮, 从某个位置与父亲比较，并逐步上浮
  void Swim(int pos) {
    int parent = pos >> 1;
    if(!parent) {
      return;
    }
    if (!fp_cmp(q[pos], q[parent]))
    {
      std::swap(q[parent], q[pos]);
      Swim(parent);
    }
  }
  
  void Push(const T& v) {
    // 如果Pop 没有pop_back, 那么就需要判定最后的位置
    // 果然KISS 最佳。。。。
    if(cnt < q.size() - 1) {
      q[cnt+1] = v;
    } else {
      q.push_back(v);
    }
    ++cnt;
    Swim(cnt);
  }

  T Pop () {
    if(!cnt) {
      return T{};
    }
    T v = q.at(1);
    q[1] = q[cnt];
    // 这个会影响topk 为什么呢
    // 因为push_back 工作的不对!
    // q.pop_back();
    --cnt;
    Sink(1);
    return v;
  }

  T Top() {
    return q[1];
  }

  bool Empty() {
    return cnt == 0;
  }

  bool Full(int k) {
    return cnt >= k;
  }
  // 以从小到大排序为例，先建立大根堆，再逐个pop即可（pop出来的元素放到末尾）
  static void Sort(const std::string& file){
    std::ifstream ifs(file);
    if(!ifs.is_open()) {
      printf("Got fucked!\n");
      return;
    }
    int sz;
    ifs >> sz;
    int num;
    Heap<T> max_heap(sz, CompareLE<T>);
    while (!ifs.eof())
    {
      ifs >> num;
      max_heap.Push(num);
    }
    int idx = max_heap.cnt;
    while (!max_heap.Empty())
    {
      idx = max_heap.cnt;
      max_heap.q[idx] = max_heap.Pop();
    }
    assert(idx == 1);
    assert(std::is_sorted(max_heap.q.begin(), max_heap.q.end()));
  }
};



// 最大topk
void DoTopK(const std::string &file, int k)
{
  std::ifstream ifs(file);
  if (!ifs.is_open())
  {
    printf("Can't open %s\n", file.c_str());
    return;
  }
  int num;
  // 小根堆
  Heap<int> min_heap(k, CompareGE<int>);
  while (!ifs.eof())
  {
    ifs >> num;
    if(!min_heap.Full(k)) {
      min_heap.Push(num);
    } else {
      if(min_heap.Top() < num){
        // pop完了自动调整，但是可以有更好的算法，这里我们主要是验证
        min_heap.Pop();
        min_heap.Push(num);
      }
    }
  }
  int cnt = 0;
  while (!min_heap.Empty())
  {
    if(cnt < 10){
      printf("%d ", min_heap.Pop());
      ++cnt;
      continue;
    }
    printf("\n");
    cnt = 0;
  }
  printf("\n");
}

void TestHeap() {
  Heap<int> min_heap(5, CompareGE<int>);
  min_heap.Push(100);
  min_heap.Push(3000);
  min_heap.Push(20);
  min_heap.Push(1);
  min_heap.Push(9);
  min_heap.Push(-3);
  while (!min_heap.Empty())
  {
    printf("%d ", min_heap.Pop());
    // min_heap.Pop();
  }
  printf("\n");
}

// 多叉堆
/**
 * 多叉堆，多叉堆的起始位置要选在(n-1), 这样，k个节点的子节点的数组序号(不是编号）就是(k n, kn + 1, kn +2, ..., kn + n-1)
 */
template <typename T, int n>
struct MultiBrachHeap {
  std::vector<T> q_;
  uint32_t cnt_ = 0;
  std::function<bool(const T &, const T &)> fp_cmp_;
  enum
  {
    branch_cnt = n,
    offset = n - 2,
  };

  explicit MultiBrachHeap(std::size_t max_size, std::function<bool(const T &, const T &)> cmp)
  {
    q_.reserve(max_size + n);
    q_.push_back(T{});
    // 起始位置和编号的关系，以及非空判断
    // offset 代表的是空白位置，或者空白计数-1(因为索引从0开始)
    // cnt 代表当前元素计数（编号），cnt + offset 就应当是当前元素位置
    for (int i = 0; i <= offset; ++i)
    {
      q_.push_back(T{});
    }
    fp_cmp_ = cmp;
  }

  void Sink(int pos) {
    // 此处要找到pos对应的节点编号，再根据此编号，找到对应的child 范围
    // int min_chd = seq * n;
    int seq = pos - offset;
    int min_chd_pos = seq * n;
    int max_chd_pos = min_chd_pos + n - 1;
    int swc = min_chd_pos;
    // 孤寡老人
    if (min_chd_pos > cnt_ + offset)
    {
      return;
    }
    // 闭区间
    if (max_chd_pos > cnt_ + offset)
    {
      max_chd_pos = cnt_ + offset;
    }
    for (int i = min_chd_pos + 1; i <= max_chd_pos; ++i)
    {
      // 偏序关系的传递性
      // 找到逆子
      if(fp_cmp_(q_[swc], q_[i])) {
        swc = i;
        continue;
      }
    }
    if(!fp_cmp_(q_[swc], q_[pos])) {
      std::swap(q_[swc], q_[pos]);
      Sink(swc);
    }
  }

  void Swim(int pos) {
    int parent_seq = pos / n;
    if(!parent_seq) {
      return;
    }
    int parent_pos = parent_seq + offset;
    // 除了当前这个节点，parent对应的其他子节点均满足偏序关系
    if (!fp_cmp_(q_[pos], q_[parent_pos]))
    {
      std::swap(q_[pos], q_[parent_pos]);
      Swim(parent_pos);
    }
  }
  // note, 此处不做堆排，仅实现k叉堆
  void Push(const T &v) {
    q_.push_back(v);
    ++cnt_;
    Swim(cnt_ + offset);
  }

  T Pop() {
    if(!cnt_) {
      return T{};
    }
    // first element
    T v = q_.at(offset + 1);
    std::swap(q_[offset + cnt_], q_[offset + 1]);
    q_.pop_back();
    --cnt_;
    Sink(offset + 1);
    return v;
  }

  bool Empty(){
    return cnt_ == 0;
  }
};

void TestMultiHeap(){
  auto v = GenreateRandom(20, -10000, 10000);
  auto min_heap = MultiBrachHeap<int, 8>(1000, CompareGE<int>);
  for(const auto &vv:v) {
    min_heap.Push(vv);
  }
  int idx = 0;
  while (!min_heap.Empty())
  {
    v[idx++] = min_heap.Pop();
  }
  assert(std::is_sorted(v.begin(), v.end()));
  printf("Multi heap check passed!\n");
  PrintVector<int>(v, 10);
}

#ifndef BLOCK_MAIN
int main(int argc, char** argv) {
  CLI::App app{"Heap implementation"};
  std::string file;
  // std::string st;
  uint32_t cnt;
  std::string type;
  app.add_option("-f,--file", file, "InputFile")->required();
  app.add_option("-c, --count", cnt, "Maximum count");
  app.add_option("-t, --type", type, "Heap operation type")->required();

  CLI11_PARSE(app, argc, argv);
  if (type == "topk")
  {
    DoTopK(file, cnt);
  } else if(type == "sort") {
    Heap<int>::Sort(file);
    printf("Sort finished! check passed!\n");
  } else if(type == "mt") {
    TestMultiHeap();
  }
  else
  {
    printf("Invalid operation type :%s \n", type.c_str());
    return -1;
  }
  return 0;
}
#endif