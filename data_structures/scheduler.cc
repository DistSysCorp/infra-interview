#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

int64_t getCurrMs() {
  auto now = std::chrono::system_clock::now();
  std::chrono::milliseconds nowMs =
      std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
  return nowMs.count();
}

class Scheduler {
 public:
  Scheduler() {
    stopped_ = false;
    std::thread loop([this]() { this->scheduleLoop(); });
    loop.detach();
  }

  ~Scheduler() {
    std::unique_lock<std::mutex> lock(mu_);
    stopped_ = true;
  }

  int64_t schedule(std::function<void()> task, int64_t delayInSec) {
    std::unique_lock<std::mutex> lock(mu_);

    int64_t taskId = idGen_++;
    int64_t targetTs = getCurrMs() + delayInSec * 1000;

    Task newTask(taskId, task, targetTs);
    tasks_[taskId] = newTask;
    taskBuckets_[targetTs].push_back(taskId);
    runCv_.notify_one();

    return taskId;
  }

  void cancel(int64_t taskId) {
    std::unique_lock<std::mutex> lock(mu_);
    tasks_.erase(taskId);
  }

 private:
  struct Task {
    Task() = default;
    Task(int64_t id, std::function<void()> r, int64_t t) : taskId(id), run(r), targetTs(t) {}

    Task(const Task& other) {
      taskId = other.taskId;
      run = other.run;
      targetTs = other.targetTs;
    }

    Task& operator=(const Task& other) {
      taskId = other.taskId;
      run = other.run;
      targetTs = other.targetTs;
      return *this;
    }

    int64_t taskId;
    std::function<void()> run;
    int64_t targetTs;
  };

  void scheduleLoop() {
    std::unique_lock<std::mutex> lock(mu_);
    while (!stopped_) {
      if (taskBuckets_.empty()) {
        runCv_.wait_for(lock, std::chrono::milliseconds(1000));
        continue;
      }

      // check if we have tasks need to be executed
      int64_t nextTs = taskBuckets_.begin()->first;
      int64_t delay = nextTs - getCurrMs();

      if (delay < 0) {
        auto& taskIds = taskBuckets_.begin()->second;
        for (auto taskId : taskIds) {
          // the task maybe canceled
          if (tasks_.find(taskId) == tasks_.end()) {
            continue;
          }

          std::thread t(tasks_[taskId].run);
          t.detach();  // run in back ground
          tasks_.erase(taskId);
        }
        taskBuckets_.erase(nextTs);
      } else {
        runCv_.wait_for(lock, std::chrono::milliseconds(delay));
      }
    }
  }

 private:
  std::mutex mu_;
  std::condition_variable runCv_;
  bool stopped_;
  int64_t idGen_;
  std::unordered_map<int64_t, Task> tasks_;
  std::map<int64_t, std::vector<int64_t>> taskBuckets_;
};

int main() {
  Scheduler tasks;

  std::cout << "Now time is: " << getCurrMs() << std::endl;
  auto task1 =
      tasks.schedule([]() { std::cout << "Now time is: " << getCurrMs() << std::endl; }, 2);

  auto task2 =
      tasks.schedule([]() { std::cout << "Now time is: " << getCurrMs() << std::endl; }, 1);

  tasks.cancel(task2);
  std::this_thread::sleep_for(std::chrono::seconds(10));
}
