== 二叉树性质：
=== 1. 具备n个节点的二叉树，有n-1条边
```
证明: 除了root外，每个节点都有一个父亲，共有(n-1)个有父亲的节点，从而必有n-1条边
```
=== 2. 第$k$层至多有$2^(k-1)$个节点
当$k=0$时，易验证共有1个节点，满足。当$k=n+1$时:
$
  max(N_(n+1)) = 2 times (2^(n-1)) = 2^(n) = 2^(n+1 - 1)
$
得证
=== 3. 高度为h的二叉树，至少有h个节点， 至多有$2^h - 1$个节点.

证明：
1. 每一层至少有一个节点，从而至少有h个节点
2. 数学归纳法，当$i=1$时，共有1个节点，满足$2^i - 1 = 1$，设$i =h$时，有$2^h -1$个节点，当$i=h+1$,
根据性质2, 当为满二叉树时，h层二叉树的总结点数为:
$
  1 + 2 + ... + 2^(h-1) = (1-2^(h))/(1-2) = 2^h -1
$

高度h的二叉树总结点数为$2^h -1 $时称为满二叉树。

=== 4. （难理解）节点数为n的二叉树，最大高度为n, 最小高度为$log_2(n+1)$
1. 当每层只有一个节点时，易得最大高度为n
2. 因高度为h的数节点至多有$2^h - 1$个节点。 反之，n 取最小高度时，则有:
$
  2^h - 1 >= n => h >= log_2(n+1)
$
这里给的比较直接。实际上，当取得最小高度$h$时，树至多是满二叉树，至少在$h$层有1个节点。而对于$h$的树，其最大节点数量$N_h=2^h - 1 >= n$，从而得证。

== 完全二叉树
出最后一层$k$外，$n_i = 2^(k-1)$(满),最后一层从左往右摆放叶子节点，这样的树叫做完全二叉树。 完全二叉树可以理解为满二叉树少了几个节点

=== 完全二叉树性质:
对完全二叉树(拥有节点n)，从上往下，从左往右依次编号（从1开始），可以得到以下性质：
1. i =1,节点是根节点
2. i>1， i/2 是其父节点
3. 如果$2i >n$,则i无左子节点，否则$2i$ 是其左子节点
4. 如果$2i + 1> n$,则i无右子节点，否则$2i + 1$是其右子节点
5. 节点数量为$M$的完全二叉树，其高度为$overline(log_2(M+1))$(向上取整)
证明：
1. 略

由于从位置1开始存储，那么，第一层的范围是[1,] 第二层的范围是[2,2+2^1), 第三层的范围是[4, 4 + 2^2),第k层的范围（k不是最后一层）$[2^(k-1) - 1 + 1, 2^(k-1) + 2^(k-1) ) = [2^(k-1), 2^k)$. 对于$2^(k-1)$这个节点，其左子节点位置为$2^k$. 右子节点为$2^(k) + 1$. 同理，对于$2^(k-1) + 1$，其左子节点为$2^(k) + 2$, 以此类推。即可得到3，4。

性质5. 证明

当高度为1时，性质5. 成立。在一般情况下， 设树t的高度为$h$，那么$h-1$高度的满二叉树节点数量为$N_(h-1) = 2^(h-1) - 1$, 第$h-1$层的节点数量为$n_(h-1) = 2^(h-2)$， 如果t是满二叉树，则第$h$层数量为$n_h = 2^(h-1)$, 否则,$n_(h) = M - N_(h-1) => 1 <= n_(h) <= 2^(h-1)$, 而$log_2(n_h / 2^(h-1)) <= 1$（此公式表明$n_h$能否填满第$h$层). 因此
$
log_2((M-N_(h-1))/2^(h-1)) <= 1 => log_2([M-2^(h-1) + 1]/2^(h-1)) <= 1 => log_2((M+1)/2^(h-1) - 1) <= 1
$

== $t o p_k$ 问题证明
#let top = $t o p$
不妨设这里的top是最大问题，那么我们假设我们有一个自动排序的容器M，容器头部是最小值。另外假设输入数据$N >> k$, 那么:
1. $1 < n <= k$时， M对数据自动排序
2. $n > k$ 时，不妨设$n = k+1$, 从M中去除最小数据（M头部或者$n u m_(k+1)$），则对于$[1, k + 1]$而言，相当于去除了一个最小值. 得到的是该区间的$top_k$
3. 往下的过程，相当于这个区间逐步扩大，即del(min[1, K+2]) del(min[1, K+3]) ...

这个过程其实也挺微妙的，求top_k 其实并不是直接求top_k，而是逐步剔除不满足条件的值。