#include <iostream>
#include <vector>

// a helper function for recursive
void helperSort(std::vector<int> &arr, int begin, int end) {
  if (begin >= end) {
    return;
  }

  int pivot = begin;
  int last = begin;
  for (int i = begin + 1; i <= end; ++i) {
    if (arr[i] < arr[pivot]) {
      ++last;  // expand the smaller list
      std::swap(arr[i], arr[last]);
    }
  }

  // put pivot to right place
  std::swap(arr[pivot], arr[last]);

  // sort left list and right list recursively
  helperSort(arr, begin, last - 1);
  helperSort(arr, last + 1, end);
}

// incr order
void quickSort(std::vector<int> &arr) {
  if (arr.size() <= 1) {
    return;
  }

  helperSort(arr, 0, arr.size() - 1);
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
  quickSort(arr);
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
