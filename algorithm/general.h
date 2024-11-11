#ifndef GENREAL_H_
#define GENREAL_H_ 1
#include <vector>
#include <string>
#include <iostream>
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

#endif