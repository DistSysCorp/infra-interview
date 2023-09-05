#include <iostream>
#include <vector>

// a helper function for recursive
void helper_sort(std::vector<int>&arr, int begin, int end) {
    if (begin >= end) {
        return;
    }

    // split
    // int mid = begin + (end-begin)/2;
    int mid = (begin + end) / 2;

    // sort recursively
    helper_sort(arr, begin, mid);
    helper_sort(arr, mid+1, end);

    // merge
    std::vector<int> tmp_left;
    tmp_left.assign(arr.begin()+begin, arr.begin()+mid+1);
    std::vector<int>& tmp_right = arr;

    int l = 0;
    int r = mid+1;
    int m = begin;
    while (l < tmp_left.size() && r <= end) {
        if (tmp_left[l] < tmp_right[r]) {
            arr[m++] = tmp_left[l++];
        } else {
            arr[m++] = tmp_right[r++];
        }
    }
    while (l < tmp_left.size()) {
        arr[m++] = tmp_left[l++];
    }
    while (r <= end) {
        arr[m++] = tmp_right[r++];
    }
}

// incr order
void merge_sort(std::vector<int>& arr) {
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
    merge_sort(arr);
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
