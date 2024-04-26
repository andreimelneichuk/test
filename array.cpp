#include <iostream>

int main() {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int* ptr = arr;
    std::cout << "4-й элемент массива: " << *(ptr + 3) << std::endl;
    return 0;
}
