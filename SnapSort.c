// Wie sortiere ich am besten. Programmer Solution: "Use the power of code not reflect mathematics.
/* Snap Sort Dataset 0     time: 4.710740 seconds
Snap Sort Dataset 1     time: 0.002137 seconds
Snap Sort Dataset 2     time: 0.002351 seconds
Snap Sort Dataset 3     time: 1.922978 seconds
Snap Sort Dataset 4     time: 4.825857 seconds */
// Here we go:
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

// Define the pool and block structures
struct block
{
    char *data;
    size_t size;
    struct block *next;
};

struct pool
{
    char *base;
    size_t size;
    struct block *free_list;
};

// Function declarations
void init_pool(struct pool *p, size_t size);
void *pool_alloc(struct pool *p, size_t size);
void free_pool(struct pool *p);

// Data pool and size
#define DATA_SIZE 32000

int data[DATA_SIZE];
// int nums[DATA_SIZE];
int numsnap[DATA_SIZE];
long snapcompare = 0, snapswap = 0;

// Define the snap structure
struct snap
{
    int num;
    struct snap *next;
    struct snap *prev;
};

void print_list(struct snap *base)
{
    struct snap *current = base;
    printf("Linked list state: ");
    while (current != NULL)
    {
        printf("%d ", current->num);
        current = current->next;
    }
    printf("\n");
}

void SnapSort(int size, int nums[])
{
    struct pool my_pool;
    struct snap *snap = NULL, *base = NULL, *head = NULL, *snap_ptr = NULL;
    const long snap_size = sizeof(struct snap);

    //  printf("Getting the memory for sorting %ld Bytes\n", snap_size * DATA_SIZE);
    init_pool(&my_pool, snap_size * DATA_SIZE);

    // Initialize the linked list with the first element
    snap = pool_alloc(&my_pool, snap_size);
    if (snap != NULL)
    {
        snap->num = nums[0];
        snap->next = NULL;
        snap->prev = NULL;
        base = snap;
        head = snap;
    }
    else
    {
        printf("Memory allocation failed\n");
        return;
    }

    // Insert the rest of the elements into the linked list
    for (int i = 1; i < size; i++)
    {
        struct snap *new_snap = pool_alloc(&my_pool, snap_size);
        if (new_snap == NULL)
        {
            printf("Memory allocation failed\n");
            return;
        }
        new_snap->num = nums[i];
        new_snap->next = NULL;
        new_snap->prev = NULL;

        if (new_snap->num < base->num)
        {
            // Insert at the beginning
            new_snap->next = base;
            base->prev = new_snap;
            base = new_snap;
        }
        else if (new_snap->num > head->num)
        {
            // Insert at the end
            new_snap->prev = head;
            head->next = new_snap;
            head = new_snap;
        }
        else
        {
            // Insert in the middle
            snap_ptr = base;
            while (snap_ptr != NULL && snap_ptr->num < new_snap->num)
            {
                snap_ptr = snap_ptr->next;
            }
            new_snap->next = snap_ptr;
            new_snap->prev = snap_ptr->prev;
            if (snap_ptr->prev != NULL)
            {
                snap_ptr->prev->next = new_snap;
            }
            else
            {
                base = new_snap; // Adjust base if inserted at the very beginning
            }
            snap_ptr->prev = new_snap;
        }
        // print_list(base); // Debugging statement to print the list after each insertion
    }

    // Copy sorted elements back to the array
    snap_ptr = base;
    for (int i = 0; i < size; i++)
    {
        if (snap_ptr == NULL)
        {
            printf("Error out of data - Index %d\n", i);
            return;
        }
        nums[i] = snap_ptr->num;
        if ((snap_ptr = snap_ptr->next) != NULL)
        {
            if (snap_ptr->num < nums[i])
            {
                printf("Error data sort - Index %d Value %d Next %d\n", i, nums[i], snap_ptr->num);
            }
           // printf("%d\t%d\n", i, nums[i]); // Debug help
        }
    }
}
#if 0
void generate_data(int z)
{
    // z ==0 Complete Random
    for (int i = 0; i < DATA_SIZE; i++)
    {
        data[i] = rand() % DATA_SIZE;
    }
}
#endif
// Thank you to Bing for the different Datasets
void generate_data(int z) {
    switch (z) {
        case 0: // Komplett zufällige Daten
            for (int i = 0; i < DATA_SIZE; i++) {
                data[i] = rand() % DATA_SIZE;
            }
            break;
        case 1: // Sortierte Daten
            for (int i = 0; i < DATA_SIZE; i++) {
                data[i] = i;
            }
            break;
        case 2: // Umgekehrt sortierte Daten
            for (int i = 0; i < DATA_SIZE; i++) {
                data[i] = DATA_SIZE - i - 1;
            }
            break;
        case 3: // Teilweise sortierte Daten
            for (int i = 0; i < DATA_SIZE; i++) {
                data[i] = i;
            }
            // Zufällige Permutation eines Teils des Arrays
            for (int i = DATA_SIZE / 4; i < 3 * DATA_SIZE / 4; i++) {
                int j = rand() % (3 * DATA_SIZE / 4 - DATA_SIZE / 4) + DATA_SIZE / 4;
                int temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
            break;
        case 4: // Daten mit vielen Duplikaten
            for (int i = 0; i < DATA_SIZE; i++) {
                data[i] = rand() % (DATA_SIZE / 10);
            }
            break;
        default:
            // Standardmäßig komplett zufällige Daten
            for (int i = 0; i < DATA_SIZE; i++) {
                data[i] = rand() % DATA_SIZE;
            }
            break;
    }
}


void init_pool(struct pool *p, size_t size)
{
    p->base = (char *)malloc(size);
    p->size = size;
    p->free_list = (struct block *)malloc(sizeof(struct block));
    p->free_list->data = p->base;
    p->free_list->size = size;
    p->free_list->next = NULL;
}

void *pool_alloc(struct pool *p, size_t size)
{
    struct block *current = p->free_list;
    struct block *prev = NULL;

    while (current != NULL)
    {
        if (current->size >= size)
        {
            void *result = current->data;
            current->data += size;
            current->size -= size;

            if (current->size == 0)
            {
                if (prev == NULL)
                {
                    p->free_list = current->next;
                }
                else
                {
                    prev->next = current->next;
                }
                free(current);
            }
            return result;
        }
        prev = current;
        current = current->next;
    }
    return NULL;
}

void free_pool(struct pool *p)
{
    free(p->base);
    struct block *current = p->free_list;
    while (current != NULL)
    {
        struct block *next = current->next;
        free(current);
        current = next;
    }
}

#define DATA_SAMPLES   5
int main()
{
    clock_t start, end;
    double cpu_time_used;
    srand(time(0));

    // Snap Sort
    for (int z = 0; z < DATA_SAMPLES; z++)
    {
        generate_data(z);

        memcpy(numsnap, data, DATA_SIZE * sizeof(int));
        start = clock();
        SnapSort(DATA_SIZE, numsnap);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Snap Sort Dataset %d\ttime: %f seconds\n", z, cpu_time_used);
    }
return 0;
}

