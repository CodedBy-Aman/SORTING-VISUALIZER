#include "sorting_algos.h"

void swap(SortState *state, int i, int j) {
    int temp = state->arr[i];
    state->arr[i] = state->arr[j];
    state->arr[j] = temp;
    state->swaps++;
}

int bubbleStep(SortState *state, int *i, int *j) {
    if (*i >= state->size-1) return 1;
    if (*j < state->size - *i - 1) {
        state->comparisons++;
        if (state->arr[*j] > state->arr[*j+1])
            swap(state,*j,*j+1);
        (*j)++;
    } else {
        *j = 0;
        (*i)++;
    }
    return 0;
}

int insertionStep(SortState *state, int *i, int *j) {
    static int key;
    static int inserting = 0;

    if (*i >= state->size)
        return 1; // Done sorting

    if (!inserting) {
        key = state->arr[*i];
        *j = *i - 1;
        inserting = 1;
    }

    if (*j >= 0 && state->arr[*j] > key) {
        state->arr[*j + 1] = state->arr[*j];
        (*j)--;
        state->swaps++;
        return 0; // Continue animation
    } else {
        state->arr[*j + 1] = key;
        (*i)++;
        inserting = 0;
    }

    return 0; // Keep running until sorted
}


int selectionStep(SortState *state, int *i, int *j, int *min_idx) {
    if (*i >= state->size-1) return 1;
    if (*j < state->size) {
        if (state->arr[*j] < state->arr[*min_idx])
            *min_idx = *j;
        (*j)++;
    } else {
        swap(state, *i, *min_idx);
        (*i)++;
        *j = *i + 1;
        *min_idx = *i;
    }
    state->comparisons++;
    return 0;
}

int mergeStep(SortState *state, MergeState *m) {
    static int l = 0, r = 0, mid = 0, left[MAX_SIZE], right[MAX_SIZE];
    static int n1 = 0, n2 = 0;
    static int i = 0, j = 0, k = 0;
    static int step = 0;

    // Step 0: Setup merge sizes and arrays
    if (m->mergeSize == 0) {
        m->mergeSize = 2;
        m->mergeLeft = 0;
    }

    // When finished one full pass
    if (m->mergeLeft >= state->size - 1) {
        if (m->mergeSize >= state->size) {
            m->sorted = 1;
            return 1; // Done sorting
        }
        m->mergeLeft = 0;
        m->mergeSize *= 2;
        return 0;
    }

    // Step 1: Setup current subarrays
    if (step == 0) {
        l = m->mergeLeft;
        mid = l + m->mergeSize / 2 - 1;
        r = (l + m->mergeSize - 1 < state->size) ? (l + m->mergeSize - 1) : (state->size - 1);
        if (mid >= state->size) {
            m->mergeLeft += m->mergeSize;
            return 0;
        }

        n1 = mid - l + 1;
        n2 = r - mid;
        for (i = 0; i < n1; i++) left[i] = state->arr[l + i];
        for (j = 0; j < n2; j++) right[j] = state->arr[mid + 1 + j];
        i = j = 0;
        k = l;
        step = 1;
    }

    // Step 2: Merge animation (element by element)
    if (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            state->arr[k++] = left[i++];
        } else {
            state->arr[k++] = right[j++];
        }
        state->comparisons++;
        state->swaps++; // count as visual overwrite
        return 0;
    }

    // Step 3: Copy leftovers (left array)
    if (i < n1) {
        state->arr[k++] = left[i++];
        return 0;
    }

    // Step 4: Copy leftovers (right array)
    if (j < n2) {
        state->arr[k++] = right[j++];
        return 0;
    }

    // Step 5: Done merging this segment, move to next
    step = 0;
    m->mergeLeft += m->mergeSize;
    return 0;
}
