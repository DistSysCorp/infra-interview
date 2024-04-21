// 在 linux 平台上，通过 `g++ BlockingQueue.cc -std=c++17 -lpthread` 编译即可

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
class BlockingQueue {
 public:
  BlockingQueue(size_t cap) : capacity_(cap) {}

  void push(const T& data) {
    std::unique_lock<std::mutex> lock(mu_);
    while (queue_.size() >= capacity_) {
      std::cout << "queue is full, blocking" << std::endl;
      fullCond_.wait(lock);
    }

    queue_.push(data);
    emptyCond_.notify_one();
  }

  size_t size() const {
    std::unique_lock<std::mutex> lock(mu_);
    return queue_.size();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(mu_);
    while (queue_.empty()) {
      std::cout << "queue is empty, blocking" << std::endl;
      emptyCond_.wait(lock);
    }

    auto value = queue_.front();
    queue_.pop();
    fullCond_.notify_one();
    return value;
  }

 private:
  std::queue<T> queue_;
  size_t capacity_;

  std::mutex mu_;
  std::condition_variable emptyCond_;
  std::condition_variable fullCond_;
};

int main() {
  std::mutex printMu;
  BlockingQueue<int> q(2);

  auto push = [&q, &mu = printMu](std::vector<int> data) {
    for (auto num : data) {
      {
        std::lock_guard<std::mutex> lock(mu);
        std::cout << std::this_thread::get_id() << ": push " << num << std::endl;
      }
      q.push(num);
    }
  };

  auto pop = [&q, &mu = printMu](size_t count) {
    {
      std::lock_guard<std::mutex> lock(mu);
      std::cout << std::this_thread::get_id() << ": wait for 1s, then start pop" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while (count--) {
      auto num = q.pop();
      {
        std::lock_guard<std::mutex> lock(mu);
        std::cout << std::this_thread::get_id() << ": pop = " << num << std::endl;
      }
    }
  };

  std::thread t1(std::bind(push, std::vector<int>({1, 2, 3, 4})));
  std::thread t2(std::bind(pop, 8));
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::thread t3(std::bind(push, std::vector<int>({5, 6, 7, 8})));

  t1.join();
  t2.join();
  t3.join();

  return 0;
}
