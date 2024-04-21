#include <iostream>
#include <vector>

// incr order
void bubbleSort(std::vector<int>& arr) {
  if (arr.size() <= 1) {
    return;
  }

  // many passes: n = arr.size()
  for (size_t pass = 0; pass < arr.size() - 1; pass++) {
    size_t curr_pos = pass;

    for (size_t i = arr.size() - 1; i > curr_pos; --i) {
      if (arr[i] < arr[i - 1]) {
        // swap
        int tmp = arr[i];
        arr[i] = arr[i - 1];
        arr[i - 1] = tmp;
      }
    }
  }
}

void print(const std::vector<int>& arr) {
  for (auto num : arr) {
    std::cout << num << " ";
  }
  std::cout << std::endl;
}

void test(std::vector<int>& arr) {
  std::cout << "before sort: ";
  print(arr);
  bubbleSort(arr);
  std::cout << "after  sort: ";
  print(arr);
}

int main() {
  {
    std::vector<int> arr{4, 10, 2, 1};
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
