#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <typename K, typename V>
struct LinkNode {
  LinkNode() = default;
  LinkNode(K k, V v) : key(k), val(v) {
    next = nullptr;
  }

  K key;
  V val;
  LinkNode *next;
};

template <typename K, typename V>
class HashTable {
 public:
  HashTable() {
    table_.resize(INIT_BUCKET_SIZE);
    size_ = 0;
  }

  ~HashTable() {
    for (auto head : table_) {
      for (auto it = head; it != nullptr;) {
        auto curr = it;
        it = it->next;

        delete curr;
      }
    }
  }

 public:
  size_t size() const {
    return size_;
  }

  void insert(K key, V val) {
    std::cout << "insert(" << key << ", " << val << ")" << std::endl;

    auto *iter = find(key);
    if (iter != nullptr) {  // update
      iter->val = val;
      return;
    } else {  // insert in the head
      size_t loc = hasher_(key) % table_.size();
      auto newNode = new LinkNode<K, V>(key, val);
      newNode->next = iter;
      table_[loc] = newNode;
      ++size_;
    }
  }

  bool erase(K key) {
    std::cout << "erase(" << key << ")" << std::endl;
    size_t hash = hasher_(key);
    size_t loc = hash % table_.size();

    auto *iter = table_[loc];
    LinkNode<K, V> *prev = nullptr;
    while (iter) {
      if (iter->key == key) {
        break;
      }
      prev = iter;
      iter = iter->next;
    }

    // not found
    if (iter == nullptr) {
      return false;
    }

    // remove
    if (prev == nullptr) {  // head
      table_[loc] = iter->next;
    } else {
      prev->next = iter->next;
    }
    --size_;
    delete iter;
    return true;
  }

  V get(K key) const {
    auto *iter = find(key);
    if (iter == nullptr) {
      std::stringstream ss;
      ss << key << " is not found";
      throw std::runtime_error(ss.str());
    }
    return iter->val;
  }

  size_t count(K key) const {
    auto iter = find(key);

    // do not find
    if (iter == nullptr) {
      return 0;
    } else {
      return 1;
    }
  }

  void echo() {
    std::cout << "==================" << std::endl;
    std::cout << "all elements are:" << std::endl;
    for (auto *head : table_) {
      for (auto *iter = head; iter != nullptr; iter = iter->next) {
        std::cout << "(" << iter->key << ":" << iter->val << ")";
        std::cout << std::endl;
      }
    }
  }

 private:
  LinkNode<K, V> *find(K key) const noexcept {
    size_t hash = hasher_(key);
    size_t loc = hash % table_.size();

    auto *iter = table_[loc];
    while (iter != nullptr && iter->key != key) {
      iter = iter->next;
    }
    return iter;
  }

 private:
  const size_t INIT_BUCKET_SIZE = 13;

  std::vector<LinkNode<K, V> *> table_;
  std::hash<K> hasher_;
  size_t size_;
};

int main() {
  HashTable<int, int> m;
  m.insert(1, 1);
  m.insert(1, 2);
  m.echo();
  std::cout << "m.get(1)=" << m.get(1) << std::endl;

  m.insert(2, 1);
  m.echo();
  m.erase(1);
  m.echo();

  return 0;
}
