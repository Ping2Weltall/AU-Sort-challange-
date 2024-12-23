// Ich muß noche testen ob die Sortierung wirklich in Ordnung ist diese Zahlen lassen mich erschaudern was Mathematiker mit Ihren Wissen anrichten können ⚠️. Kein Wunder brauchen die Atomkraftwerke. 🤔😁
Hier meine Programmtechnische Lösumg für das sortieren großer Datenmengen,
// #Computeralex #b0561223
// Stand: 23. Dezember 2024
/* Perfor.ance:
Dataset 0       Size: 11000000  Komplett zufällige Daten
Snap Sort Dataset 0     time: 3.757126 seconds
Dataset 1       Size: 11000000  Sortierte Daten
Snap Sort Dataset 1     time: 3.120501 seconds
Dataset 2       Size: 11000000  Umgekehrt sortierte Daten
Snap Sort Dataset 2     time: 3.116910 seconds
Dataset 3       Size: 11000000  Teilweise sortierte Daten     Zufällige Permutation eines Teils des Arrays
Snap Sort Dataset 3     time: 3.457552 seconds
Dataset 4       Size: 11000000  Daten mit vielen Duplikaten
Snap Sort Dataset 4     time: 3.737063 seconds
Dataset 5       Size: 11000000  Standardmäßig komplett zufällige Daten
Snap Sort Dataset 5     time: 3.740030 seconds

[Program finished]
Muß man sich mal vorstellen.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


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

#define DATA_SIZE 11000000
#define SNAP_CURSOR_SIZE 10

double data[DATA_SIZE];
double numsnap[DATA_SIZE];
long snapcompare = 0, snapswap = 0;

struct snap
{
    double num;
    struct snap *next;
    struct snap *prev;
};
struct snap_cursor {
    struct snap *snap;
    double num;
};

struct snap *SnapSort(int size, double nums[]) {
    struct pool my_pool;
    struct snap *snap = NULL, *base = NULL, *head = NULL, *snap_ptr = NULL;
    struct snap_cursor snap_cursor[SNAP_CURSOR_SIZE];
    const long snap_size = sizeof(struct snap);

    // Initialize the memory pool
    init_pool(&my_pool, snap_size * DATA_SIZE);

    // Initialize the linked list with the first element
    snap = pool_alloc(&my_pool, snap_size);
    if (snap != NULL) {
        snap->num = nums[0];
        snap->next = NULL;
        snap->prev = NULL;
        base = snap;
        head = snap;

        // Initialize the snap_cursor array
        for (int i = 0; i < SNAP_CURSOR_SIZE; i++) {
            snap_cursor[i].snap = snap;
            snap_cursor[i].num = snap->num;
        }
    } else {
        printf("Memory allocation failed\n");
        return NULL;
    }

    // Insert the rest of the elements into the linked list
    for (int i = 1; i < size; i++) {
        struct snap *new_snap = pool_alloc(&my_pool, snap_size);
        if (new_snap == NULL) {
            printf("Memory allocation failed\n");
            return NULL;
        }
        new_snap->num = nums[i];
        new_snap->next = NULL;
        new_snap->prev = NULL;

        // Find the best starting point using snap_cursor
        int closest_index = 0;
        for (int j = 1; j < SNAP_CURSOR_SIZE; j++) {
            if (fabs(snap_cursor[j].num - new_snap->num) < fabs(snap_cursor[closest_index].num - new_snap->num)) {
                closest_index = j;
            }
        }
        snap_ptr = snap_cursor[closest_index].snap;

        // Insert the new element
        if (new_snap->num < base->num) {
            // Insert at the beginning
            new_snap->next = base;
            base->prev = new_snap;
            base = new_snap;
        } else if (new_snap->num > head->num) {
            // Insert at the end
            new_snap->prev = head;
            head->next = new_snap;
            head = new_snap;
        } else {
            // Insert in the middle
            while (snap_ptr != NULL && snap_ptr->num < new_snap->num) {
                snap_ptr = snap_ptr->next;
            }
            new_snap->next = snap_ptr;
            new_snap->prev = snap_ptr->prev;
            if (snap_ptr->prev != NULL) {
                snap_ptr->prev->next = new_snap;
            } else {
                base = new_snap; // Adjust base if inserted at the very beginning
            }
            snap_ptr->prev = new_snap;
        }

        // Update snap_cursor
        for (int j = SNAP_CURSOR_SIZE - 1; j > 0; j--) {
            snap_cursor[j] = snap_cursor[j - 1];
        }
        snap_cursor[0].snap = new_snap;
        snap_cursor[0].num = new_snap->num;
    }
    return base;
}

#if 0
// With sanity check 
struct snap *SnapSort(int size, int nums[]) {
    struct pool my_pool;
    struct snap *snap = NULL, *base = NULL, *head = NULL, *snap_ptr = NULL;
    struct snap_cursor snap_cursor[SNAP_CURSOR_SIZE];
    const long snap_size = sizeof(struct snap);

    // Initialize the memory pool
    init_pool(&my_pool, snap_size * DATA_SIZE);

    // Initialize the linked list with the first element
    snap = pool_alloc(&my_pool, snap_size);
    if (snap != NULL) {
        snap->num = nums[0];
        snap->next = NULL;
        snap->prev = NULL;
        base = snap;
        head = snap;

        // Initialize the snap_cursor array
        for (int i = 0; i < SNAP_CURSOR_SIZE; i++) {
            snap_cursor[i].snap = snap;
            snap_cursor[i].num = snap->num;
        }
    } else {
        printf("Memory allocation failed\n");
        return NULL;
    }

    // Insert the rest of the elements into the linked list
    for (int i = 1; i < size; i++) {
        struct snap *new_snap = pool_alloc(&my_pool, snap_size);
        if (new_snap == NULL) {
            printf("Memory allocation failed\n");
            return NULL;
        }
        new_snap->num = nums[i];
        new_snap->next = NULL;
        new_snap->prev = NULL;

        // Find the best starting point using snap_cursor
        int closest_index = 0;
        for (int j = 1; j < SNAP_CURSOR_SIZE; j++) {
            if (abs(snap_cursor[j].num - new_snap->num) < abs(snap_cursor[closest_index].num - new_snap->num)) {
                closest_index = j;
            }
        }
        snap_ptr = snap_cursor[closest_index].snap;

        // Insert the new element
        if (new_snap->num < base->num) {
            // Insert at the beginning
            new_snap->next = base;
            base->prev = new_snap;
            base = new_snap;
        } else if (new_snap->num > head->num) {
            // Insert at the end
            new_snap->prev = head;
            head->next = new_snap;
            head = new_snap;
        } else {
            // Insert in the middle
            while (snap_ptr != NULL && snap_ptr->num < new_snap->num) {
                snap_ptr = snap_ptr->next;
            }
            new_snap->next = snap_ptr;
            new_snap->prev = snap_ptr->prev;
            if (snap_ptr->prev != NULL) {
                snap_ptr->prev->next = new_snap;
            } else {
                base = new_snap; // Adjust base if inserted at the very beginning
            }
            snap_ptr->prev = new_snap;
        }

        // Update snap_cursor
        for (int j = SNAP_CURSOR_SIZE - 1; j > 0; j--) {
            snap_cursor[j] = snap_cursor[j - 1];
        }
        snap_cursor[0].snap = new_snap;
        snap_cursor[0].num = new_snap->num;
    }
    return base;
}
#endif
#if 0 // No sanity check
struct snap *SnapSort(int size, int nums[])
{
    struct pool my_pool;
    struct snap *snap = NULL, *base = NULL, *head = NULL, *snap_ptr = NULL;
    struct snap_cursor snap_cursor[SNAP_CURSOR_SIZE];
    const long snap_size = sizeof(struct snap);

    // Initialize the memory pool
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

        // Initialize the snap_cursor array
        for (int i = 0; i < SNAP_CURSOR_SIZE; i++)
        {
            snap_cursor[i].snap = snap;
            snap_cursor[i].num = snap->num;
        }
    }
    else
    {
        printf("Memory allocation failed\n");
        return (NULL);
    }

    // Insert the rest of the elements into the linked list
    for (int i = 1; i < size; i++)
    {
        struct snap *new_snap = pool_alloc(&my_pool, snap_size);
        if (new_snap == NULL)
        {
            printf("Memory allocation failed\n");
            return (NULL);
        }
        new_snap->num = nums[i];
        new_snap->next = NULL;
        new_snap->prev = NULL;

        // Find the best starting point using snap_cursor
        int closest_index = 0;
        for (int j = 1; j < SNAP_CURSOR_SIZE; j++)
        {
            if (abs(snap_cursor[j].num - new_snap->num) < abs(snap_cursor[closest_index].num - new_snap->num))
            {
                closest_index = j;
            }
        }
        snap_ptr = snap_cursor[closest_index].snap;

        // Insert the new element
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
        // Update snap_cursor
        for (int j = SNAP_CURSOR_SIZE - 1; j > 0; j--)
        {
            snap_cursor[j] = snap_cursor[j - 1];
        }
        snap_cursor[0].snap = new_snap;
        snap_cursor[0].num = new_snap->num;
    }
    return (base);
}
#endif
// Thank you to Bing for the different Datasets
void generate_data(int z)
{
    printf("Dataset %d\tSize: %d \t", z, DATA_SIZE);
    switch (z)
    {
    case 0:
        printf("Komplett zufällige Daten\n");

        for (int i = 0; i < DATA_SIZE; i++)
        {
            data[i] = rand() % DATA_SIZE;
        }
        break;
    case 1:
        printf("Sortierte Daten\n");
        for (int i = 0; i < DATA_SIZE; i++)
        {
            data[i] = i;
        }
        break;
    case 2:
        printf("Umgekehrt sortierte Daten\n");
        for (int i = 0; i < DATA_SIZE; i++)
        {
            data[i] = DATA_SIZE - i - 1;
        }
        break;
    case 3:
        printf("Teilweise sortierte Daten\t");
        for (int i = 0; i < DATA_SIZE; i++)
        {
            data[i] = i;
        }
        printf("Zufällige Permutation eines Teils des Arrays\n");
        for (int i = DATA_SIZE / 4; i < 3 * DATA_SIZE / 4; i++)
        {
            int j = rand() % (3 * DATA_SIZE / 4 - DATA_SIZE / 4) + DATA_SIZE / 4;
            int temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
        break;
    case 4: //
        printf("Daten mit vielen Duplikaten\n");
        for (int i = 0; i < DATA_SIZE; i++)
        {
            data[i] = rand() % (DATA_SIZE / 10);
        }

        break;
    default:
        printf("Standardmäßig komplett zufällige Daten\n");
        for (int i = 0; i < DATA_SIZE; i++)
        {
            data[i] = rand() % DATA_SIZE;
        }
        break;
    }
}

#define DATA_SAMPLES 6
int main()
{
    clock_t start, end;
    double cpu_time_used;
    srand(time(0));
    struct snap *base = NULL;

    // Snap Sort
    for (int z = 0; z < DATA_SAMPLES; z++)
    {
        generate_data(z);

        memcpy(numsnap, data, DATA_SIZE * sizeof(int));
        start = clock();
        if ((base =  SnapSort(DATA_SIZE, numsnap)) == NULL)
        {
        printf("Fail!\n");
        }
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Snap Sort Dataset %d\ttime: %f seconds\n", z, cpu_time_used);
    }
    return 0;
}
#if 0
int main() {
    srand(time(0)); // Initialize the random number generator

    int data[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; i++) {
        data[i] = rand() % DATA_SIZE;
    }

    struct snap *base = SnapSort(DATA_SIZE, data);

    // Print sorted data for verification
    struct snap *current = base;
    while (current != NULL) {
        int n;
        //printf("%d ", current->num);
        
        if ((current = current->next) != NULL)
        {
           if (n < current)
           {
              printf("Value against the river!!! %d < %d\n");
              exit(1);
           }
        }
    }
    printf("\n");

    return 0;
}
#endif
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
 