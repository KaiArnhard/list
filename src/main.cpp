#include <cstdio>
#include "ListFunctions.h"
#include "Graphvis.h"

int main() {
    list_t list;
    ListCtor(&list);
    for (size_t i = 1; i <= 6; i++) {
        Insert(&list, i * 10, i);
    }
    Insert(&list, 45, 4);
    //Insert(&list, 55, 5);
    //Insert(&list, 70, 7);
    Remove(&list, 1);
    //Insert(&list, 65, 7);
    Remove(&list, 5);
    //Insert(&list, 45, 7);
    //Insert(&list, 80, 8);
    ListDump(&list);

    //PrintOfList(&list);
    
    ListDtor(&list);
}