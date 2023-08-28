#include <iostream>
#include <vector>

// incr order
void insertion_sort(std::vector<int>& arr) {
    if (arr.size() <= 1) {
        return;
    }

    // many passes: n = arr.size()
    for (size_t pass = 1; pass < arr.size(); pass++) {
        // insertion
        for (size_t i = pass; i > 0; --i) {
            if (arr[i] < arr[i-1]) {
                int tmp = arr[i];
                arr[i] = arr[i-1];
                arr[i-1] = tmp;
            } else {
                break;
            }
        }
    }
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
    insertion_sort(arr);
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
