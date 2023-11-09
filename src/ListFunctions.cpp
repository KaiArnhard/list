#include <cstdio>
#include "Graphvis.h"
#include "ListFunctions.h"

void ListCtor(list_t *list, int *next, int *prev, elem_t *data) {
    *list = {
    .data = data,
    .next = next,
    .prev = prev,
    .head = 1,
    .tale = defaultListSize - 1,
    .free = 1,
    .size = defaultListSize
    };
}

void ListCtor(list_t *list) {
    elem_t *data = (elem_t*) calloc(defaultListSize, sizeof(elem_t));
    int *next = (int*) calloc(defaultListSize, sizeof(int));
    int *prev = (int*) calloc(defaultListSize, sizeof(int));
    
    size_t counter = 1;
    for (; counter < defaultListSize; counter++) {
        next[counter] = counter + 1;
        prev[counter] = defaultPrevAddres;
    }
    *data = poison;
    *prev = defaultNextAddres;
    next[--counter] = defaultNextAddres;
    ListCtor(list, next, prev, data);    
}

void ListDtor(list_t *list) {
    for (size_t counter = 0; counter < list->size; counter++) {
        list->data[counter] = poison;
        list->next[counter] = poison;
        list->prev[counter] = poison;
    }
    list->free = poison;
    list->head = poison;
    list->tale = poison;
    list->size = poison;
    
    free(list->data);
    free(list->next);
    free(list->prev);
    
    list = nullptr;
}

void ListDump(list_t *list) {
    if (Errno == LIST_ERROR_DATA_POINTER_IS_ZERO || Errno == LIST_ERROR_LIST_POINTER_IS_ZERO || Errno == LIST_ERROR_NEXT_POINTER_IS_ZERO || Errno == LIST_ERROR_PREV_POINTER_IS_ZERO) {
        abort();
    }
    if (Errno == LIST_ERROR_SIZE_LOWER_ZERO) {
        printf("Size of list if lower zero\n");
    } else {
    printf(BLUE "Head: %d\t Tale: %d\t Free: %d\t Size: %d\n" WHITE, list->head, list->tale, list->free, list->size);
    for (size_t counter = 1; counter < list->size; counter++) {
        printf("%3.d ", counter);
    }
    printf("\n");
    PrintOfListData(list->data, list->size);

    PrintOfListPtrs(list->next, list->size, YELLOW);
    PrintOfListPtrs(list->prev, list->size, RED);

    GenerateOfPngDump("listDump.dot", "listDump.png", list);
    }

    if (Errno) {
        ListDtor(list);
        exit(Errno);
    }
}

void GenerateOfPngDump(const char *dumpName, const char* PngName, const list_t* list) {
    BEGIN(dumpName, List);

    RANK_DIR(filePtr, TB);
    RANK_DIR(filePtr, LR);
    EDGE_COLOR(darkred);

    for (size_t counter = 0; counter < list->size; counter++) {
        GEN_MULTI_SECT_NODE_WITH_2_LABELS(filePtr, counter, list->data[counter], list->next[counter], list->prev[counter], 1, 2);
    }
    GEN_MULTI_SECTION_NODE(filePtr, information, list->head, list->tale, list->free, list->size);
    
    for (size_t counter = 1; counter < list->size; counter++) {
        GEN_OF_ARROW(filePtr, counter, list->next[counter], 1, 1);
        /*if (list->prev[counter] != defaultListAddres) {
            GEN_OF_ARROW(filePtr, counter, list->prev[counter], 2, 2);
        }*/
    }
    END(filePtr);

    GEN_OF_PNG(dumpName, PngName);
}

void PrintOfListData(elem_t *data, size_t size) {
    
    for (size_t counter = 1; counter < size; counter++) {
        if (data[counter] != 0) {
            printf(GREEN "%3.0d " WHITE, data[counter]);
        } else {
            printf(GREEN "%.3d " WHITE, data[counter]);
        }
    }
    printf("\n");
}

void PrintOfListPtrs(int* ptrs, size_t size, const char* color) {
    
    for (size_t counter = 1; counter < size; counter++) {
        if (ptrs[counter] != defaultNextAddres) {
            printf("%s%3.0d " WHITE, color, ptrs[counter]);
        } else {
            printf("%s%.3d " WHITE, color, ptrs[counter]);
        }        
    }
    printf("\n");
}

size_t Insert(list_t *list, const elem_t insertedValue, const size_t position) {
    if (Verify(list, position, "Insert")) {
        ListDump(list);
        return defaultPrevAddres;
    }
    
    ListDump(list);
    if (list->free == defaultNextAddres || list->prev[list->free] != defaultPrevAddres) {
        list = ListResize(list);
    }
    size_t tmp = list->next[list->free];
    list->data[list->free] = insertedValue;

    if ((list->tale == defaultListSize - 1) && list->free == 1 && list->size == defaultListSize && list->head == 1 && position == 1) {
        list->next[list->free] = 2;
        list->prev[list->free] = defaultNextAddres;
        list->tale = list->free;
        list->free = tmp;
        return list->free;
    }
    
    if (position > list->tale) {
        InsertToEnd(list, position);
    } else {
        list->next[list->free]           = list->next[position];
        list->prev[list->next[position]] = list->free;
        list->next[position]             = list->free;
        list->prev[list->free]           = position;
    }

    if (position == 1) {
        list->head           = list->free;
        list->prev[position] = position - 1;
    }
    list->free = tmp;
     
    return list->free;
}

void InsertToEnd(list_t *list, const size_t position) {
    
    list->prev[list->free] = list->tale;
    list->next[list->tale] = list->free;
    list->tale = list->free;
    list->next[list->free]   = defaultNextAddres;
}

size_t Remove(list_t *list, size_t position) {
    if (Verify(list, position, "Remove")) {
        ListDump(list);
        return defaultPrevAddres;
    }
    ListDump(list);

    if (position == list->head) {
        list->prev[list->next[position]] = defaultNextAddres;
        list->head = list->next[position];
    } else if (position == list->tale) {
        list->next[list->prev[position]] = defaultNextAddres;
        list->tale = list->prev[position];
    } else {
        list->next[list->prev[position]] = list->next[position];
        list->prev[list->next[position]] = list->prev[position];
    }
    list->next[position] = list->free;
    list->prev[position] = defaultPrevAddres;
    list->free = position;    

    return list->free;
}

Error_t Verify(list_t *list, size_t position, const char *nameOfFunction) {
    
    assert(list != nullptr);
    if (list->size == -1) {
        Errno = LIST_ERROR_SIZE_LOWER_ZERO;
        return LIST_ERROR_SIZE_LOWER_ZERO;
    } else if (list->size < defaultListSize) {
        Errno = LIST_ERROR_SIZE_LOWER_DEFAULT;
        return LIST_ERROR_SIZE_LOWER_DEFAULT;
    } else if (list->data == nullptr) {
        Errno = LIST_ERROR_DATA_POINTER_IS_ZERO;
        return LIST_ERROR_DATA_POINTER_IS_ZERO;
    } else if (list->next == nullptr) {
        Errno = LIST_ERROR_NEXT_POINTER_IS_ZERO;
        return LIST_ERROR_DATA_POINTER_IS_ZERO;
    } else if (list->prev == nullptr) {
        Errno = LIST_ERROR_PREV_POINTER_IS_ZERO;
        return LIST_ERROR_PREV_POINTER_IS_ZERO;
    } else if (list->data[0] != poison) {
        Errno = LIST_ERROR_NULL_ELEMENT_CHANGED;
        return LIST_ERROR_NULL_ELEMENT_CHANGED;
    } else if (position == list->tale) {
        Errno = LIST_ERROR_POSITION_EQUAL_TALE;
        return LIST_ERROR_POSITION_EQUAL_TALE;
    } else if (position > list->size && list->free != 0) {
        Errno = LIST_ERROR_POSITION_MORE_THAN_SIZE;
        return LIST_ERROR_POSITION_MORE_THAN_SIZE;
    } else if (list->prev[position] == defaultPrevAddres && !strcmp(nameOfFunction, "Remove")) {
        Errno = LIST_ERROR_REMOVE_FREE_ELEMENT;
        return LIST_ERROR_REMOVE_FREE_ELEMENT;
    }

    return LIST_NO_ERRORS;
}

list_t *ListResize(list_t *list) {
    list->free     = list->size;
    size_t counter = list->size;

    list->size *= reallocationConst;
    list->data = (elem_t*) realloc(list->data, list->size * sizeof(elem_t));
    list->next = (int*) realloc(list->next, list->size * sizeof(int));
    list->prev = (int*) realloc(list->prev, list->size * sizeof(int));
    
    for (; counter < list->size; counter++) {
        list->next[counter] = counter + 1;
        list->prev[counter] = defaultPrevAddres;
    }
    list->next[counter] = 0;
    return list;
    
}
