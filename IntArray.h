# ifndef __INT_ARRAY
# define __INT_ARRAY

# ifndef __BOOLEAN
# define __BOOLEAN

# define true 1
# define false 0

# endif

# define INITIAL_ARR_SIZE 5

typedef struct _intArray {
	int* root;
	int end;
	int size;
} intArray;

void init(intArray* list) {
        list->size = INITIAL_ARR_SIZE;
        list->root = (int*) malloc (sizeof(int) * list->size);
        list->end = 0;
}

int isFull(intArray* list) {
        return ( (list->end == list->size) ? true : false );
}

void add(intArray* list, int val) {
        if (isFull(list) == true) {
                list->size *= 2;
                list->root = (int*) realloc (list->root, sizeof(int) * list->size);
        }
        *(list->root + (list->end)++) = val;
}

void remove(intArray* list, int idx) {
        (list->end)--;
        for (int i = idx; i < list->end; i++)
                *(list->root + i) = *(list->root + i + 1);
}

void free(intArray* list) {
        free(list->root);
}

# endif

