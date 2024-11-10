#include <iostream>
#include <vector>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <CLI/CLI.hpp>

struct Union {
  std::vector<int> tp;
  std::vector<int> zw;
  int32_t count;
  explicit Union(int n) {
    tp.resize(n);
    zw.resize(n);
    for (int i = 0; i<n;++i) {
      tp[i] = i;
      // Record the brach quantites of current tree.
      zw[i] = 0;
    }
    count = n;
  }

  int Find(int key) {
    // int conn = tp[key];
    key = tp[key];
    while(key != tp[key]) {
      key = tp[key];
    }
    return key;
  }

  bool IsConncted(int l, int r) {
    return Find(l) == Find(r);
  }
  int UnionCount () {
    return count;
  }
  void DoUnion(int l, int r) {
    int lt = Find(l);
    int rt = Find(r);
    if(lt == rt) {
      return ;
    }
    // basic operation...
    // Connect rt to both lt and l are fine.but connecting to root is more efficient!
    // tp[rt] = lt;
    // 计算机算法大概如此，有很多细节性的东西要记忆，如果记不住细节，也很难记住由此而产生的代码细节
    // weight version.
    // 比较树权重，将小树，链接到大树，并且增加lt的权重，这里的权重实际上应该是分支的数目
    if(zw[lt] < zw[rt]){
      tp[lt] = rt;
      zw[lt] += 1;
    } else {
      tp[rt] = lt;
      zw[rt] += 1;
    }
    --count;
    assert(count >= 0);
  }
};

int main() {
  std::ifstream ifs("mf.txt");
  std::string line;
  while(line== "") {
    std::getline(ifs, line);
    std::cout<<line <<std::endl;
  }
  uint32_t n = std::stoul(line);
  Union u = Union(n);
  int widx = 0, l=0,r=0;
  for(;;) {
    std::getline(ifs, line);
    if(line == "" || ifs.eof()) {
      break;
    }
    widx = line.find_last_of(" ");
    // std::cout << line.substr(0, widx) <<  "," << line.substr(widx+1, line.size()-1) <<std::endl;
    l = std::stoi(line.substr(0, widx));
    r = std::stoi(line.substr(widx+1, line.size()-1));
    u.DoUnion(l, r);
  }
  std::cout <<"Union count:" << u.UnionCount() << std::endl;
  return 0;
}