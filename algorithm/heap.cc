#include <string>
#include <vector>
#include "general.h"

template <typename T>
struct Heap {
  std::vector<T> q;
  uint32_t cnt;
  Heap(uint32_t sz) {
    q.reserve(sz);
    cnt = 0;
  }
  // 下沉，删去一个节点后，从尾部将一个节点放到根节点，并逐步下沉
  void Sink(int pos) {
    int child = pos << 1;
    if (child > cnt) {
      return;
    }
    if (q[child] > q[pos]) {
      std::swap(q[child], q[pos]);
      Swim(child);
      return;
    }
    child = pos << 1 + 1;
    if(child > cnt) {
      return;
    }
    if(q[child] > q[pos]) {
      std::swap(q[child], q[pos]);
      Swim(child);
      return;
    }
  }
  // 上浮, 从某个位置与父亲比较，并逐步上浮
  void Swim(int pos) {
    int parent = pos >> 1;
    if(!parent) {
      return;
    }
    if(q[parent] < q[pos]) {
      std::swap(q[parent], q[pos]);
      Swim(parent);
    }
  }
  
  void Push(const T& v) {
    q.push_back(v);
    ++cnt;
    Swim(cnt);
  }

  T Pop () {
    if(!cnt) {
      return T{};
    }
    T v = q.at(1);
    q[1] = q[cnt];
    q.pop_back();
    --cnt;
    return v;
  }

  bool Empty() {
    return (bool) cnt;
  }

};

int main(int argc, char** argv) {
  Heap<int> foo(128);
  auto v = GenreateRandom(128, -10000, 10000);
  for(const auto& vv:v) {
    foo.Push(vv);
  }
  v.clear();
  while(!foo.Empty()) {
    v.push_back(foo.Pop());
  }
  bool is_sort = std::is_sorted(v.begin(), v.end(), [](const int&a, const int& b){
    return a > b;
  });
  if(is_sort) {
    printf("Sorted!\n");
  } else {
    printf("Fucked!\n");
  }
  return 0;
}