#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "search.h"

int main(int argc, char *argv[]){

    if ( argc > 5 ){
        printf("Only the first four inputs matter broh!");
    }

    int min = atoi(argv[1]);
    int max = atoi(argv[2]);
    int repeats = atoi(argv[3]);
    char* which = argv[4];
    clock_t start, end;
    double cputime;

    if ( strchr( which, 97 ) != NULL ){
        // do array search and time and print

        printf("LINERAR ARRAY SEARCH:\n");

        for (int i = min; i <= max ; ++i) {

            int len = 1 << i;
            int* it = make_sequential_array( len );

            double times = 0;

            for (int l = 0; l < repeats; ++l) { // loop that implements the repeat

                for (int j = 0; j < len / 2; ++j) { // successful searches
                    start = clock();
                    linear_array_search(it, len, j);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;

                    start = clock();
                    linear_array_search(it, len, j + 50);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;
                }

                for (int j = 0; j < len / 2; ++j) { // unsuccessful searches
                    start = clock();
                    linear_array_search(it, len, -j);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;

                    start = clock();
                    linear_array_search(it, len, -j - 50);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;
                }
            }

            printf("Size: %d Time: %10.3e Searches: %d\n", len, times, len*2*repeats);

            free(it);
        }
    }
    if ( strchr( which, 98 ) != NULL ){
        // do binary search and time and print
        printf("BINARY ARRAY SEARCH:\n");

        for (int i = min; i <= max ; ++i) {

            int len = 1 << i;
            int* it = make_sequential_array( len );

            double times = 0;

            for (int l = 0; l < repeats; ++l) { // loop that implements the repeat

                for (int j = 0; j < len / 2; ++j) { // successful searches
                    start = clock();
                    binary_array_search(it, len, j);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;

                    start = clock();
                    binary_array_search(it, len, j + 50);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;
                }

                for (int j = 0; j < len / 2; ++j) { // unsuccessful searches
                    start = clock();
                    binary_array_search(it, len, -j);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;

                    start = clock();
                    binary_array_search(it, len, -j - 50);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;
                }
            }

            printf("Size: %d Time: %10.3e Searches: %d\n", len, times, len*2*repeats);

            free(it);
        }
    }
    if ( strchr( which, 108 ) != NULL ){
        // do linked list search and time and print
        printf("LINKED LIST SEARCH:\n");

        for (int i = min; i <= max ; ++i) {

            int len = 1 << i;
            list_t* it = make_sequential_list( len );

            double times = 0;

            for (int l = 0; l < repeats; ++l) { // loop that implements the repeat

                for (int j = 0; j < len / 2; ++j) { // successful searches
                    start = clock();
                    linkedlist_search(it, len, j);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;

                    start = clock();
                    linkedlist_search(it, len, j + 50);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;
                }

                for (int j = 0; j < len / 2; ++j) { // unsuccessful searches
                    start = clock();
                    linkedlist_search(it, len, -j);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;

                    start = clock();
                    linkedlist_search(it, len, -j - 50);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;
                }
            }

            printf("Size: %d Time: %10.3e Searches: %d\n", len, times, len*2*repeats);

            list_free(it);
        }
    }
    if ( strchr( which, 116 ) != NULL ){
        // do tree search and time and print
        printf("BINARY TREE SEARCH:\n");

        for (int i = min; i <= max ; ++i) {

            int len = 1 << i;
            bst_t* it = make_sequential_tree( len );

            double times = 0;

            for (int l = 0; l < repeats; ++l) { // loop that implements the repeat

                for (int j = 0; j < len / 2; ++j) { // successful searches
                    start = clock();
                    binary_tree_search(it, len, j);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;

                    start = clock();
                    binary_tree_search(it, len, j + 50);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;
                }

                for (int j = 0; j < len / 2; ++j) { // unsuccessful searches
                    start = clock();
                    binary_tree_search(it, len, -j);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;

                    start = clock();
                    binary_tree_search(it, len, -j - 50);
                    end = clock();
                    cputime = (double)(end - start) / CLOCKS_PER_SEC;
                    times += cputime;
                }
            }

            printf("Size: %d Time: %10.3e Searches: %d\n", len, times, len*2*repeats);

            bst_free(it);
        }
    }
}
