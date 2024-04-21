#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

class FixedMaxHeap {
 public:
  FixedMaxHeap(size_t capacity) : cap_(capacity) {
    data_.reserve(cap_ + 1);
    // use an sentinel in the
    data_.push_back(INT_MAX);
    // if we set a sentinel in position 0, then the binary
    // heap looks like:
    //     0
    //     |
    //     1
    //    / \
        //   2   3
    //   and we will have some really nice properties:
    //      1. parent(index) = index/2
    //      2. children(index) = index*2, index*2+1(maybe exist or not)
    //      3. we need not check the if the index overflow every time
  }

  size_t size() const {
    return data_.size() - 1;
  }

  bool isFull() const {
    return size() == cap_;
  }

  bool isEmpty() const {
    return size() == 0;
  }

  // will raise exception when there is no element
  int top() const {
    if (isEmpty()) {
      throw std::invalid_argument("heap is empty");
    }

    return data_[ROOT_IDX];
  }

  void push(int val) {
    if (isFull()) {
      throw std::invalid_argument("heap is full");
    }

    std::cout << "try to push: " << val << std::endl;
    data_.push_back(val);
    adjustUp();
  }

  int pop() {
    if (isEmpty()) {
      throw std::invalid_argument("heap is empty");
    }
    std::cout << "try to pop: " << top() << std::endl;

    int val = top();
    data_[ROOT_IDX] = data_.back();
    data_.pop_back();  // shrink
    adjustDown();      // make it a heap again

    return val;
  }

 private:
  // insert from bottom level to the root
  // suppose that: only the last value does not
  // compose the heap properties
  void adjustUp() {
    size_t pos = data_.size() - 1;
    int val = data_[pos];

    for (; data_[pos / 2] < data_[pos]; pos /= 2) {
      data_[pos] = data_[pos / 2];
    }

    if (pos != data_.size() - 1) {
      data_[pos] = val;
    }
  }

  // adjust from the root to the bottom
  // suppose that: only the root value does not
  // compose the heap properties
  void adjustDown() {
    size_t pos = ROOT_IDX;
    int val = data_[pos];

    while (pos * 2 < data_.size()) {
      size_t child = pos * 2;

      // find the larger one to be poped
      if (child + 1 < data_.size() && data_[child + 1] > data_[child]) {
        child++;
      }

      if (data_[child] > data_[pos]) {
        data_[pos] = data_[child];
        pos = child;
      } else {
        break;
      }
    }

    if (pos != ROOT_IDX) {
      data_[pos] = val;
    }
  }

 public:
  void echo() {  // for debug
    for (size_t i = ROOT_IDX; i < data_.size(); ++i) {
      std::cout << data_[i] << " ";
    }
    std::cout << std::endl;
  }

 private:
  static const size_t ROOT_IDX = 1;

  size_t cap_;
  std::vector<int> data_;
};

int main() {
  std::vector<int> data(100);
  for (int i = 0; i < 100; ++i) {
    data[i] = i;
  }
  std::random_device rd;
  std::mt19937 rng(rd());
  std::shuffle(data.begin(), data.end(), rng);

  // find top 5 smallest number
  FixedMaxHeap q(5);
  for (auto num : data) {
    if (!q.isFull()) {
      q.push(num);
    } else if (num < q.top()) {
      q.pop();
      q.push(num);
    }

    q.echo();
  }

  std::cout << "The smallest 5 number are:" << std::endl;
  while (!q.isEmpty()) {
    std::cout << q.top() << " ";
    q.pop();
  }

  return 0;
}
