#include <iostream>
#include <vector>

// incr order
void selectionSort(std::vector<int> &arr) {
  if (arr.size() <= 1) {
    return;
  }

  // many passes: n = arr.size()
  for (size_t pass = 0; pass < arr.size() - 1; pass++) {
    size_t target_pos = pass;

    // pick a smallest position
    size_t smallest_pos = target_pos;
    for (size_t i = arr.size() - 1; i > target_pos; --i) {
      if (arr[i] < arr[smallest_pos]) {
        smallest_pos = i;
      }
    }

    // swap
    int tmp = arr[smallest_pos];
    arr[smallest_pos] = arr[target_pos];
    arr[target_pos] = tmp;
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
  selectionSort(arr);
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
