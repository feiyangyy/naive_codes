#include "../general.h"
#include <type_traits>
/**
 * 逆序数有两种基本算法
 * 1. 基本的n^2 的双循环迭代 累加，这是一种直接的暴力算法
 * 2. 使用归并排序的思想，归并排序的基本思想是:
 *    1. 左 右两个**有序**序列要合并成一个新的有序序列
 *    2. 分别从左右两个序列中取元素，并比较大小，放入缓存数组中，左右某一个到达末尾时，把另外一个剩下的全部添加到末尾
 *        * 2. 的过程中可以统计逆序数，对于两个有序数组a[0,N], b[0,M],假设原始数组为(a,b) 如果a_i < b_k（左边的本应更小）, 那么逆序数就是(N-i + 1), 即a_i ... a_N > b_k, 
 *        * 他们都能构成逆序对。
 *        * 3. 接下来要考虑，归并递归的过程中，会不会改变逆序数数量。也就是说，对于整个数组K，我们将其拆分成两个K_0, K_1。 分别统计K_0, K_1的逆序数，并排序，再统计K_0,K_1 排序后的元素，形成的逆序数。
 *        * 换句话说，无论K_0 是否排序，其和K_1 中的元素形成的逆序对是不会改变的。 这个结论是显然的，因为无论K_0 如何排序，K_1 的元素再归并之前始终在K_0 元素之前（相对位置不会改变），从而
 *        * 逆序关系不会改变。因此不影响他们对逆序数的贡献。 对于K_1, K_0 我们可以重复这个过程。
 */

// note enable if
template <typename T>
struct InversionMergeCount {
  std::vector<T> help_;
  uint32_t inverse_count_;
  InversionMergeCount(std::size_t sz)
  {
    help_.resize(sz);
    inverse_count_ = 0;
  }

  void Merge(std::vector<T>& inputs, int l, int mid, int r) {
    if(r -l <=1 ){
      return;
    }
    if(r - l <= 2) {
      if (inputs[r] < inputs[l]) {
        ++inverse_count_;
        std::swap(inputs[l], inputs[r]);
      }
      return;
    }
    int li = l, ri = mid;
    int idx = l;
    while (li < mid && ri < r)
    {
      if(inputs[li] <= inputs[mid]) {
        help_[idx++] = inputs[li++];
      } else {
        help_[idx++] = inputs[ri++];
        // mid 是开区间，不需要+1
        inverse_count_ += (mid - li);
      }
    }
    while(li < mid) {
      help_[idx++] = inputs[li++];
    }
    while(ri < r){
      help_[idx++] = inputs[ri++];
    }
    for (int i = li; i < r;++i) {
      inputs[i] = help_[i];
    }
  }

  void MergeSort(std::vector<T>& in, int l, int r) {
    // fuck! 此处总是容易忘记
    if(l >= r - 1) {
      return;
    }
    int mid = (l + r) / 2;
    MergeSort(in, l, mid);
    MergeSort(in, mid, r);
    Merge(in, l, mid, r);
  }

  void Sort(std::vector<T>& in) {
    inverse_count_ = 0;
    MergeSort(in, 0, in.size());
    printf("Inversion count:%d\n", inverse_count_);
  }
};

int main(int argc, char** argv) {
  auto isc = InversionMergeCount<int>(10);
  std::vector<int> a = GenreateRandom(3, -1000, 1000);
  isc.Sort(a);
  return 0;
}