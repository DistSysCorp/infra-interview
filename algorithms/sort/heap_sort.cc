#include <iostream>
#include <vector>

void heapify(std::vector<int> &arr, int begin, int end) {
  if (begin >= end) {
    return;
  }

  int root = begin;
  int left = root * 2 + 1;
  int right = left + 1;

  int max = root;
  if (left <= end && arr[left] > arr[max]) {
    max = left;
  }
  if (right <= end && arr[right] > arr[max]) {
    max = right;
  }

  if (max == root) {
    return;
  }
  std::swap(arr[root], arr[max]);
  heapify(arr, max, end);
}

// incr order
void heapSort(std::vector<int> &arr) {
  if (arr.size() <= 1) {
    return;
  }

  // step 1: construct heap
  for (int root = arr.size(); root >= 0; --root) {
    heapify(arr, root, arr.size() - 1);
  }

  // step 2: pop and heapify loop
  for (int end = arr.size() - 1; end > 0; --end) {
    std::swap(arr[0], arr[end]);
    heapify(arr, 0, end - 1);
  }
}

void print(const std::vector<int> &arr) {
  for (auto num : arr) {
    std::cout << num << " ";
  }
  std::cout << std::endl;
}

void test(std::vector<int> &arr) {
  std::cout << "before sort: ";
  print(arr);
  heapSort(arr);
  std::cout << "after  sort: ";
  print(arr);
}

int main() {
  {
    std::vector<int> arr{4, 10, 2, 1};
    test(arr);
  }

  {
    std::vector<int> arr{1, 2, 3, 4};
    test(arr);
  }

  {
    std::vector<int> arr{4, 3, 2, 1};
    test(arr);
  }

  {
    std::vector<int> arr{4, 4, 2, 1};
    test(arr);
  }

  {
    std::vector<int> arr{4};
    test(arr);
  }

  {
    std::vector<int> arr;
    test(arr);
  }

  return 0;
}
