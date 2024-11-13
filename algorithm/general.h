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

#endif