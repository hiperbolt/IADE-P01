/* ist1102416 - Tomás Simões */
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "p1.h"


int main(){
    state global = {0};

    printf("Introduza comando: ");

    /*  While the handler returns 1, keep running.
        If 1 is returned, q was pressed, so we exit. */
    while (handler(&global)) {}
    return 0;


}

/*  Handles command input. */
int handler(state *global) {
    char in[100];

    /*  We use fgets() instead of scanf
        because its cleaner (in comparison to using scanf("%[^\n]",in) ) and safer
        than using gets(). */
    fgets(in, 100, stdin);

    switch (in[0]) {
        case 'q': 
            return 0;
        case 'a':
            add_airport(global, in);
            return 1;
        case 'l':
            list_airport(global, in);
            return 1;
        case 'v':
            add_list_flights(global, in);
            return 1;
        case 'p':
            departure_flights(global, in);
            return 1;
        case 'c':
            arrival_flights(global, in);
            return 1;
        case 't':
            advance_date(global, in);
            return 1;
        default:
            return 1;
    }
}

int add_airport(state *global, char *in) {
    char id[MAX_IDENTIFIER];
    char country[MAX_COUNTRY_CHARS];
    char city[MAX_CITY_CHARS];

    strncpy(id, in + 2, strlen(in) - 2);
    printf(id);


    if (global->airports_counter + 1 > MAX_AIRPORTS)
    {
        printf("too many airports");
        return 1;
    }

    int len = sizeof(global->airports)/sizeof(global->airports[0]);
    for (int i = 0; i < len; ++i) {
        if(!strcmp(global->airports[i].id, id)) {   
            printf("found!");
        }
    }

    
    return 1;
}

int list_airport(state *global, char *in) {
    return 1;
}

int add_list_flights(state *global, char *in) {
    return 1;
}

int departure_flights(state *global, char *in) {
    return 1;
}

int arrival_flights(state *global, char *in) {
    return 1;
}

int advance_date(state *global, char *in) {
    return 1;
}

int binary_search(int array[], int target, int low, int high) {
    /*
        Abstract binary search auxiliary function.

        Implements a generic iterative binary search
        algorithm.

    */
     while (low <= high) {
        int mid = low + (high - low) / 2;

        if (array[mid] == target) {
            return mid; }

        if (array[mid] < target) {
            low = mid + 1; }

        else {
            high = mid - 1; }
    }

  return -1; /* in case nothing was found */

}

