#include "visualize.h"
#include <stdbool.h>

void bubble_sort(int *arr, int n) {
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (!viz_render_state(arr, n, j, j+1, -1, n - i)) return;
            if (arr[j] > arr[j+1]) {
                int t = arr[j]; arr[j] = arr[j+1]; arr[j+1] = t;
                swapped = true;
                if (!viz_render_state(arr, n, j, j+1, -1, n - i)) return;
            }
        }
        if (!swapped) break;
    }
    viz_render_state(arr, n, -1, -1, -1, 0); // final
}
