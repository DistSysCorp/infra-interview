#include <iostream>
#include <vector>

// a helper function for recursive
void helper_sort(std::vector<int>&arr, int begin, int end) {
    if (begin >= end) {
        return;
    }

    int pivot = begin;
    int last = begin;
    for (int i = begin+1; i <= end; ++i) {
        if (arr[i] < arr[pivot]) {
            ++last; // expand the smaller list
            std::swap(arr[i], arr[last]);
        }
    }

    // put pivot to right place
    std::swap(arr[pivot], arr[last]);

    // sort left list and right list recursively
    helper_sort(arr, begin, last-1);
    helper_sort(arr, last+1, end);
}

// incr order
void quick_sort(std::vector<int>& arr) {
    if (arr.size() <= 1) {
        return;
    }

    helper_sort(arr, 0, arr.size()-1);
}

void print(const std::vector<int> &arr)
{
    for (auto num : arr)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

void test(std::vector<int> &arr)
{
    std::cout << "before sort: ";
    print(arr);
    quick_sort(arr);
    std::cout << "after  sort: ";
    print(arr);
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
