#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

#define DATA_SIZE 32000

int data[DATA_SIZE], numbs[DATA_SIZE], numqs[DATA_SIZE], nummys[DATA_SIZE], numbsort[DATA_SIZE], numheap[DATA_SIZE], numgemini[DATA_SIZE];
long bscompare = 0, bsswap = 0, qscompare = 0, qsswap = 0, mysswap = 0, myscompare = 0, bingswap = 0, bingcompare = 0, heapcompare = 0, heapswap = 0, geminicompare = 0, geminiswap = 0;

void JoseSort(int size, int nums[])
{
    for (int i = 0; i < size; i++)
        for (int j = i + 1; j < size; j++)
        {
            if (nums[j] < nums[i])
            {
                int mem = nums[i];
                nums[i] = nums[j];
                nums[j] = mem;
                bsswap++;
            }
            bscompare++;
        }
}

int is_not_sorted(int size, int nums[])
{
    for (int i = 0; i < size - 1; i++)
    {
        myscompare++;
        if (nums[i] > nums[i + 1])
            return 1;
    }
    return 0;
}

void myBubbleSort(int size, int nums[])
{
    if (is_not_sorted(size, nums))
        for (int i = 0; i < size - 1; i++)
            for (int j = 0; j < size - i - 1; j++)
            {
                if (nums[j] > nums[j + 1])
                {
                    int mem = nums[j];
                    nums[j] = nums[j + 1];
                    nums[j + 1] = mem;
                    mysswap++;
                }
                myscompare++;
            }
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
    qsswap++;
}

int partition(int arr[], int low, int high)
{
    int pivotIndex = low + rand() % (high - low + 1);
    swap(&arr[pivotIndex], &arr[high]);
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (arr[j] <= arr[high])
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
        qscompare++;
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void MyQuickSort(int nums[], int size)
{
    if (is_not_sorted(size, nums))
        quickSort(nums, 0, size - 1);
}

void merge(int nums[], int left, int mid, int right)
{
    int n1 = mid - left + 1, n2 = right - mid, L[n1], R[n2];
    for (int i = 0; i < n1; i++)
        L[i] = nums[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = nums[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        bingcompare++;
        if (L[i] <= R[j])
            nums[k++] = L[i++];
        else
            nums[k++] = R[j++];
        bingswap++;
    }
    while (i < n1)
        nums[k++] = L[i++], bingswap++;
    while (j < n2)
        nums[k++] = R[j++], bingswap++;
}

void mergeSort(int nums[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(nums, left, mid);
        mergeSort(nums, mid + 1, right);
        merge(nums, left, mid, right);
    }
}

void BingSort(int size, int nums[])
{
    if (is_not_sorted(size, nums))
        mergeSort(nums, 0, size - 1);
}

void heapify(int arr[], int n, int i)
{
    int largest = i, left = 2 * i + 1, right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;
    if (largest != i)
    {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--)
    {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

void MyHeapSort(int nums[], int size)
{
    if (is_not_sorted(size, nums))
        heapSort(nums, size);
}

void GeminiSort(int arr[], int size)
{
    if (size <= 1)
        return;
    int pivot = arr[size / 2], left[size], right[size], leftSize = 0, rightSize = 0;
    for (int i = 0; i < size; i++)
    {
        geminicompare++;
        if (arr[i] < pivot)
            left[leftSize++] = arr[i];
        else if (arr[i] > pivot)
            right[rightSize++] = arr[i];
    }
    GeminiSort(left, leftSize);
    GeminiSort(right, rightSize);
    int i = 0, j = 0, k = 0;
    while (i < leftSize)
        arr[k++] = left[i++], geminiswap++;
    while (j < rightSize)
        arr[k++] = right[j++], geminiswap++;
}

void copy_data()
{
    memcpy(numbs, data, DATA_SIZE * sizeof(int));
    memcpy(numqs, data, DATA_SIZE * sizeof(int));
    memcpy(nummys, data, DATA_SIZE * sizeof(int));
    memcpy(numbsort, data, DATA_SIZE * sizeof(int));
    memcpy(numheap, data, DATA_SIZE * sizeof(int));
    memcpy(numgemini, data, DATA_SIZE * sizeof(int));
}

int main()
{
    srand(time(0));
    printf("Generating data %d elements\n", DATA_SIZE);
    for (int i = 0; i < DATA_SIZE; i++)
        data[i] = rand() % DATA_SIZE;
    clock_t start, end;
    double cpu_time_used;

    for (int i = 0; i < 3; i++)
    {
        bsswap = qsswap = bscompare = qscompare = 0, mysswap = 0, myscompare = 0, bingswap = 0, bingcompare = 0, heapcompare = 0, heapswap = 0, geminicompare = 0, geminiswap = 0;
        
        copy_data();
        
        start = clock();
        JoseSort(DATA_SIZE, numbs);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Jose Sort time: %f seconds. %ld compares %ld swaps\n", cpu_time_used, bscompare, bsswap);
        
        copy_data();
        
        start = clock();
        myBubbleSort(DATA_SIZE, nummys);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Bubble Sort time: %f seconds. %ld compares %ld swaps\n", cpu_time_used, myscompare, mysswap);
        
        copy_data();
        
        start = clock();
        MyQuickSort(numqs, DATA_SIZE);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Quick Sort time: \t%f seconds. %ld compares %ld swaps\n", cpu_time_used, qscompare, qsswap);
        
        copy_data();
        
        start = clock();
        BingSort(DATA_SIZE, numbsort);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Bing Sort time: \t%f seconds. %ld compares %ld swaps\n", cpu_time_used, bingcompare, bingswap);
        
        copy_data();
        
        start = clock();
        MyHeapSort(numheap, DATA_SIZE);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Heap Sort time: \t%f seconds. %ld compares %ld swaps\n", cpu_time_used, heapcompare, heapswap);
        
        copy_data();
        
        start = clock();
        GeminiSort(numgemini, DATA_SIZE);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Gemini Sort time: \t%f seconds. %ld compares %ld swaps\n", cpu_time_used, geminicompare, geminiswap);
        
        for (int i = 0; i < DATA_SIZE; i++)
        {
            if (numbs[i] != numqs[i] || numbs[i] != nummys[i] || numbs[i] != numbsort[i] || numbs[i] != numheap[i] || numbs[i] != numgemini[i])
            {
                printf("Sort error Index %d: \n", i);
                printf("numbs: %d, numqs: %d, nummys: %d, numbsort: %d, numheap: %d, numgemini: %d\n", numbs[i], numqs[i], nummys[i], numbsort[i], numheap[i], numgemini[i]);
            }
        }
        
    }
    return 0;
}