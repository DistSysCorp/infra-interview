#include <iostream>
#include <vector>

struct LinkNode {
  LinkNode* next;
  int val;

  LinkNode(int v) : val(v), next(nullptr) {}
};

// incr order
LinkNode* mergeSort(LinkNode* head) {
  // empty list or list with only one node
  if (head == nullptr || head->next == nullptr) {
    return head;
  }

  // step 1: split
  LinkNode dummy_head(0);
  dummy_head.next = head;
  LinkNode* faster = &dummy_head;
  LinkNode* slower = &dummy_head;  // tail of left sub-list

  while (faster != nullptr) {
    faster = faster->next;
    if (faster != nullptr) {
      faster = faster->next;
    } else {
      break;
    }

    slower = slower->next;
  }

  LinkNode* left = dummy_head.next;
  LinkNode* right = slower->next;
  slower->next = nullptr;  // seal the left list

  // step 2: sort recursively
  left = mergeSort(left);
  right = mergeSort(right);

  // step 3: merge
  LinkNode* tail = &dummy_head;
  while (left != nullptr && right != nullptr) {
    // detach
    LinkNode* curr = nullptr;
    if (left->val < right->val) {
      curr = left;
      left = left->next;
    } else {
      curr = right;
      right = right->next;
    }

    // attach
    tail->next = curr;
    tail = tail->next;
  }

  if (left != nullptr) {
    tail->next = left;
  }
  if (right != nullptr) {
    tail->next = right;
  }
  return dummy_head.next;
}

void print(const LinkNode* iter) {
  for (; iter; iter = iter->next) {
    std::cout << iter->val << " ";
  }
  std::cout << std::endl;
}

void test(std::vector<int>& arr) {
  LinkNode dummy_head(0);
  LinkNode* tail = &dummy_head;
  for (auto num : arr) {
    tail->next = new LinkNode(num);
    tail = tail->next;
  }
  tail->next = nullptr;  // seal the tail

  std::cout << "before sort: ";
  print(dummy_head.next);
  dummy_head.next = mergeSort(dummy_head.next);
  std::cout << "after  sort: ";
  print(dummy_head.next);
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
