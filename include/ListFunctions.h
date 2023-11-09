#if !defined(LIST_FUNCTIONS_H)
#define LIST_FUNCTIONS_H

#define RED    "\033[0;31m"
#define GREEN  "\e[0;32m"
#define YELLOW "\e[0;33m"
#define WHITE  "\e[0;37m"
#define BLUE   "\e[0;34m"

#include <cstdlib>
#include <assert.h>

typedef int elem_t;

static const elem_t poison = *((elem_t*) "KLEO");

static const size_t defaultListSize   =  10;    
static const size_t reallocationConst =  2;

static const int defaultPrevAddres    = -1;
static const int defaultNextAddres    =  0;

enum Error_t {
    LIST_NO_ERRORS                     = 0,
    LIST_ERROR_SIZE_LOWER_ZERO         = 1,
    LIST_ERROR_SIZE_LOWER_DEFAULT      = 2,
    LIST_ERROR_LIST_POINTER_IS_ZERO    = 3,
    LIST_ERROR_DATA_POINTER_IS_ZERO    = 4,
    LIST_ERROR_NEXT_POINTER_IS_ZERO    = 5,
    LIST_ERROR_PREV_POINTER_IS_ZERO    = 6,
    LIST_ERROR_NULL_ELEMENT_CHANGED    = 7,
    LIST_ERROR_POSITION_EQUAL_TALE     = 8,
    LIST_ERROR_POSITION_MORE_THAN_SIZE = 9,
    LIST_ERROR_POSITION_MORE_THAN_TALE = 10,
    LIST_ERROR_REMOVE_FREE_ELEMENT     = 11,
};

static Error_t Errno = LIST_NO_ERRORS;

struct list_t {
    elem_t *data;
    int    *next;
    int    *prev;
    int     head;
    int     tale;
    size_t  free;
    size_t  size;
};

void ListCtor(list_t *list, int *next, int *prev, elem_t *data);
void ListCtor(list_t *list);
list_t *ListResize(list_t *list);
void ListDtor(list_t *list);

Error_t Verify(list_t *list, size_t position, const char *nameOfFunction);

void ListDump(list_t *list);
void PrintOfListData(elem_t *data, size_t size);
void PrintOfListPtrs(int* ptrs, size_t size, const char* color);
void GenerateOfPngDump(const char *dumpName, const char* PngName, const list_t* list);

size_t Insert(list_t *list, const elem_t insertedValue, const size_t position);
void InsertToEnd(list_t *list, const size_t position);

size_t Remove(list_t *list, size_t position);

#endif // LIST_FUNCTIONS
