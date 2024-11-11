// 1. 关注热点
// 2. 勤奋，执行力
// 3. 要挣钱，不挣钱就会被钱搞

#include <iostream>
#include <vector>
#include <stdint.h>
#include <algorithm>
#include <random>
#include <chrono>
#include <assert.h>
#include <fstream>
#include <CLI/CLI.hpp>


std::vector<int> GenreateRandom(int n, int min, int max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min, max);
  std::vector<int> result(n);
  std::for_each(result.begin(), result.end(), [&dis, &gen](int& n){n = dis(gen);});
  return result;
}


// 每次选择最小的，放到最前方
uint64_t SelectionSort(std::vector<int>& inputs) {
  uint64_t operate_cnt = 0;
  uint64_t swap_cnt = 0; 
  for (auto iter = inputs.begin(); iter != inputs.end();++iter) {
    auto it_min = iter;
    // no swap here.
    for (auto it = iter; it != inputs.end(); ++it){
      ++operate_cnt;
      if(*it < *it_min) {
        it_min = it;
      }
    }
    if(it_min != iter) {
      ++swap_cnt;
      std::swap(*it_min, *iter);
    }
  }
  printf("Operate cnt:%ld, swap cnt:%ld\n", operate_cnt, swap_cnt);
  return operate_cnt;
}
// Find a place to insert.
uint64_t InsertionSort(std::vector<int>& vals) {
  // auto i = vals.begin();
  int num = 0;
  uint64_t count = 0;
  for (auto i = vals.begin() + 1; i != vals.end(); ++i) {
    auto n = i;
    num = *n;
    // 找到插入位置
    // 完全顺序，这个循环需要执行完，但是不需要移动
    // 完全逆序，则第一次就可以移动，但是移动的次数较多
    // 总体而言比较稳定
  // 不好的实现，在寻找插入位置时，应当从最大概率的位置开始找，我们要找的是要比当前小的元素，从而 是从后往前找
  // 此处留作反例
  #if 0
    for(auto k = vals.begin(); k != n; ++k) {
      // 找到位置
      ++count;
      if(*k > *n) {
        // [k,n-1] -> [k+1, n]
        std::move(k, n, k+1);
        // 插入
        *k = num;
        break;
      }
    }
  #endif
    // 迭代器end可以表示结束的范围，但不能表示开始的结束, rend 算是么?
    // 这个已经算是改进版的插排了，原版是依次交换的。基于依次交换能够产生**希尔排序**算法
    auto insert_pos = i;
    for(auto k = i - 1; *k > num && std::distance(vals.begin(), k) >= 0; --k) {
      ++count;
      insert_pos = k;
    }
    if(insert_pos != i) {
      // 注意开闭区间
      std::move(insert_pos, i, insert_pos+1);
      *insert_pos = num;
    }
    // 原版实现:
    /*
      auto p = i;
      // 此处的比较和交换是主要的性能消耗，对于一个有序序列，不会发生这种比较和交换，因此复杂度是o(N)
      // 如果此处是逆序序列，那么要做o(N)次比较和交换，整体的性能就在o(N^2)级别
      for(auto k = i - 1; *k > num && std::distance(vals.begin(), k)>=0; --k, p=k+1) {
        std::swap(*k, *p);
      }
    */
  }
  printf("Operate count:%ld\n", count);
  return count;
}

// Shell排序
// 希尔排序的思想是，先把序列搞得差不多有序（通过不同步长的）插排，然后再使用上面的插排排一下
void ShellSort(std::vector<int>& intput) {
  static std::vector<int> steps = {32, 16, 8, 4, 2, 1};
  // 规模比较小的数据就不处理了
  if(intput.size() <= 128) {
    return;
  }
  uint64_t count = 0; 
  // 注意:这里没有考虑步长超过数据总量的情形，我们可以考虑通过算法生成这样的序列
  // 从而保证步长不会超过数据总量，这里主要是为了明确希尔排序的原理，希望我自己以后
  // 看到能立马看清楚
  for(auto s: steps) {
    for(int i = s; i < intput.size(); i += s) {
      for(int j = i - s, k = i; j >= 0 && intput[j] > intput[k]; j -= s, k -= s) {
        ++count;
        std::swap(intput[k], intput[j]);
      }
    }
  }
  printf("Shell sort, with datasize %zd, ops:%ld\n", intput.size(), count);
}

// 归并排序
// 先拆分，拆分到不可拆分为止，然后再合并的过程中进行排序，区间表示是[l,mid) [mid, r)
struct MergeSort {
  std::vector<int> help;
  explicit MergeSort(int dsize) {
    help.resize(dsize);
  }
  // 自顶向下的归排，最重要的是归并部分，排序是在归并过程中自动完成的
  // 这里容易出错的其实是边界问题，这里假设选定的都是开闭区间
  void Merge(std::vector<int>& input, int l, int mid, int r) {
    // 不可拆分
    if(r - l <= 1) {
      return;
    }
    // 归并
    int k = l;
    int li =l, ri= mid;
    while(li < mid && ri < r) {
      if(input[li] < input[ri]){
        help[k++] = input[li++];
      } else {
        help[k++] = input[ri++];
      }
    }
    while(li < mid) {
      help[k++] = input[li++];
    }
    while(ri < r) {
      help[k++] = input[ri++];
    }
    // copy back...
    for(int i = l; i <r; ++i) {
      input[i] = help[i];
    }
  }

  void Sort(std::vector<int>& input, int l, int mid, int r) {
    if (r-l <= 1) {
      // printf("Return!\n");
      return ;
    }
    Sort(input, l, (mid + l)/2, mid);
    Sort(input, mid, (r + mid)/2, r);
    Merge(input, l, mid, r);
  }
};

// 自下而上
/*
 特长，兴趣，方法，勤奋
*/
struct MergeSortBT {
  std::vector<int> help;
  MergeSortBT(int sz) {
    help.resize(sz);
  }
  // Close-Open
  // 此处考虑的是对一个区间进行切分和合并，并且给定的这个区间，双边是已排序的了
  // 注意此处烧了一个mid参数，mid 参数是自动推算的
  void Merge(std::vector<int> &input, int l, int r) {
    int mid = (l + r) / 2;
    int k = l;
    int li = l, ri = mid;

    if(r - l <= 1) {
      return;
    }
    while(li < mid && ri < r) {
      if(input[li] < input[ri]) {
        help[k++] = input[li++];
      } else {
        help[k++] = input[ri++];
      }
    }
    // 注意这个地方的边界，为什么会经常写错呢? 比如 li< mid, 写成li < l
    while(li < mid) {
      help[k++] = input[li++];
    }
    while(ri < r) {
      help[k++] = input[ri++];
    }
    for(int i = l; i<r; ++i) {
      input[i] = help[i];
    }
  }
  // 此处的问题：步长的划分, 2 4 8 ... 
  // 这里有个数学上的问题，就是任意数（数组长度）和2^n 的关系，比如 19 最接近的是 16, 相差3 对于归并来说，相差1是可以兼容的
  // 这里要理解这个过程
  void Sort(std::vector<int>& input) {
    std::vector<int> steps;
    for (int k = input.size(); k > 1; k >>= 1)
    {
      steps.push_back(k);
    }
    std::reverse(steps.begin(), steps.end());
    int sz = input.size();
    for(const auto s: steps) {
      for (int l = 0; l < sz; l += s) {
        // 逐个合并
        Merge(input, l, (l + s) > sz ? sz : (l + s));
      }
    }
  }

};

std::vector<int> ReadInputs(const std::string& file) {
  std::fstream ifs(file) ;
  if(!ifs.is_open()) {
    return {};
  }
  int nb_number;
  ifs >> nb_number;
  std::vector<int> res(nb_number);
  int idx = 0;
  while(ifs >> nb_number) {
    res[idx++] = nb_number;
    if(idx >= res.size()) {
      break;
    }
  }
  ifs.close();
  return res;
}

int main(int argc, char** argv) {

  CLI::App app{"Sort algorithm implementations."};
  std::string file;
  std::string st;
  app.add_option("-f,--file", file, "InputFile")->required();
  app.add_option("-t,--type", st, "Sort type")->required();
  CLI11_PARSE(app, argc, argv);
  auto&& v = ReadInputs(file);
  auto start = std::chrono::high_resolution_clock::now();
  if(st == "sel") {
    SelectionSort(v);
  } else if(st == "ins") {
    InsertionSort(v);
  } else if(st == "shell") {
    ShellSort(v);
  } else if (st == "ms") {
    // v = GenreateRandom(16, -10000, 10000);
    auto ms = MergeSort(v.size());
    ms.Sort(v, 0, v.size() /2, v.size());
  } else if(st == "msbt") {
    auto ms = MergeSortBT(v.size());
    ms.Sort(v);
  }
  assert(std::is_sorted(v.begin(), v.end()));
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = end -start;
  printf("Sort type %s, data_size:%ld, Time cost:%lf ms\n", st.c_str(), v.size(), duration.count());
  return 0;
#if 0
  
  auto&& vec = GenreateRandom(1024 * 10, -1024 * 512, 1024 *512);
  
  auto start = std::chrono::high_resolution_clock::now();
  SelectionSort(vec);
  assert(std::is_sorted(vec.begin(), vec.end()));
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = end -start;
  printf("Time cost:%lf ms\n", duration.count());

  auto&& vec2 = GenreateRandom(1024 * 10, -1024 * 512, 1024 *512);
  start = std::chrono::high_resolution_clock::now();
  InsertionSort(vec2);
  assert(std::is_sorted(vec2.begin(), vec2.end()));
  end = std::chrono::high_resolution_clock::now();
  duration = end -start;
  printf("Insertion time cost:%lf ms\n", duration.count());
  {

    auto&& vec2 = GenreateRandom(1024 * 10, -1024 * 512, 1024 *512);
    start = std::chrono::high_resolution_clock::now();
    ShellSort(vec2);
    assert(std::is_sorted(vec2.begin(), vec2.end()));
    end = std::chrono::high_resolution_clock::now();
    duration = end -start;
    printf("Shell sort time cost:%lf ms\n", duration.count());
  }
#endif
  return 0;
}
