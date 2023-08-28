#include <iostream>
#include <vector>

struct LinkNode {
    LinkNode* next;
    int val;

    LinkNode(int v): val(v), next(nullptr){}
};

// incr order
LinkNode* quick_sort(LinkNode* head) {
    // empty list or list with only one node
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    // select a pivot element
    LinkNode* pivot = head;
    LinkNode* smaller = nullptr;
    LinkNode* larger = nullptr;

    // step 1: split
    auto iter = head->next;
    while (iter) {
        // detach a node
        auto curr = iter;
        iter = iter->next;

        // attach the node to smaller or larger
        if (curr->val < pivot->val) {
            curr->next = smaller; 
            smaller = curr;
        } else {
            curr->next = larger; 
            larger = curr;
        }
    }

    // step 2: sort recursively
    smaller = quick_sort(smaller);
    larger = quick_sort(larger);

    // step 3: concat smaller->pivot->larger
    pivot->next = larger;
    if (smaller == nullptr) {
        return pivot;
    }

    auto tail = smaller;
    while (tail->next) {
        tail = tail->next;
    }
    tail->next = pivot;
    return smaller;
}

void print(const LinkNode* iter) {
    for (; iter; iter = iter->next){
        std::cout << iter->val << " ";
    }
    std::cout << std::endl;
}

void test(std::vector<int> &arr)
{
    LinkNode dummy_head(0);
    LinkNode* tail = &dummy_head;
    for (auto num: arr) {
        tail->next = new LinkNode(num);
        tail = tail->next;
    }
    tail->next = nullptr; // seal the tail

    std::cout << "before sort: ";
    print(dummy_head.next);
    dummy_head.next = quick_sort(dummy_head.next);
    std::cout << "after  sort: ";
    print(dummy_head.next);
}

int main()
{
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
