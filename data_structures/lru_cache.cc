#include <iostream>
#include <sstream>
#include <unordered_map>

class Node {
 public:
  Node(int k, int v) : key(k), val(v) {
    prev = next = nullptr;
  }

  std::string toString() {
    std::stringstream ss;
    ss << "{ " << key << ": " << val << "}";
    return ss.str();
  }

  int key;
  int val;
  Node* prev;
  Node* next;
};

// could only store 0 and positive numbers
class LRUCache {
 public:
  LRUCache(int capacity) : cap_(capacity) {
    head_ = new Node(-1, -1);
    tail_ = new Node(-1, -1);
    head_->next = tail_;
    tail_->prev = head_;
  }

  ~LRUCache() {
    release(head_);
  }

  int get(int key) {
    std::cout << "try to get " << key << " = ";
    auto iter = index_.find(key);
    if (iter == index_.end()) {
      return -1;
    }

    auto* node = iter->second;
    attach(head_, detach(node));
    return node->val;
  }

  void put(int key, int value) {
    std::cout << "try to put " << key << " : " << value << std::endl;
    auto iter = index_.find(key);
    if (iter != index_.end()) {
      iter->second->val = value;
      attach(head_, detach(iter->second));
      return;
    }

    Node* node = new Node(key, value);
    index_[key] = attach(head_, node);

    if (index_.size() > cap_) {
      auto toDel = detach(tail_->prev);
      index_.erase(toDel->key);
      std::cout << "evict item: " << toDel->toString() << std::endl;
      delete toDel;
    }
  }

 public:  // for test
  size_t size() {
    return index_.size();
  }

  void echo() {
    Node* iter = head_->next;
    while (iter != tail_) {
      std::cout << iter->toString() << "; ";
      iter = iter->next;
    }
    std::cout << std::endl;
  }

 private:
  void release(Node* head) {
    while (head) {
      Node* curr = head;
      head = head->next;
      std::cout << "release " << curr->toString() << std::endl;
      delete curr;
    }
  }

  Node* detach(Node* node) {
    node->next->prev = node->prev;
    node->prev->next = node->next;
    return node;
  }

  Node* attach(Node* head, Node* node) {
    node->next = head->next;
    node->prev = head;

    head->next->prev = node;
    head->next = node;

    return node;
  }

 private:
  int cap_;
  Node* head_;
  Node* tail_;
  std::unordered_map<int, Node*> index_;
};

int main() {
  LRUCache cache(3);
  cache.put(1, 10);
  cache.put(2, 20);
  cache.put(3, 30);
  cache.put(4, 40);
  cache.put(5, 50);
  cache.echo();

  cache.put(4, 400);
  cache.echo();

  std::cout << cache.get(1) << std::endl;
  cache.echo();

  std::cout << cache.get(3) << std::endl;
  cache.echo();

  cache.put(2, 200);
  cache.echo();

  return 0;
}
