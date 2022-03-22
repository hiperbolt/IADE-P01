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

    /*  Input parsing.
        We essentially divide the array copied from stdin
        to a new multidimensional array, where every
        new vector is a whitespace separated token.

        This is an ugly (and probably
        not very memory safe) implementation and should
        be improved.
    */

    /* TODO: GARANTE QUE O ULTIMO TOKEN É PARSED CORRETAMENTE */
    char arguments[100][100];
    char * pch;
    pch = strtok (in," ");
    strcpy(arguments[0], pch);
    int i = 1;
    while (pch != NULL)
    {
        pch = strtok (NULL, " ");
        if (pch != NULL) {
        strcpy(arguments[i], pch);
        i++;
        }
    }


    switch (in[0]) {
        case 'q': 
            return 0;
        case 'a':
            add_airport(global, arguments[1], arguments[2], arguments[3]);
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

int add_airport(state *global, char *id, char *country, char *city) {
    if (global->airports_counter + 1 > MAX_AIRPORTS)
    {
        printf("too many airports");
        return 1;
    }

    for (int i = 0; i < sizeof(global->airports)/sizeof(global->airports[0]); i++) {
        if(!strcmp(global->airports[i].id, id)) {   
            printf("duplicate airport");
            return 1;
        }
    }
    
    for (int i = 0; i > sizeof(id); i++) {
        if (islower(id[i]) == 0){
            printf("invalid airport ID");
            return 1;
        }
    }

    strcpy(global->airports[global->airports_counter].id, id);
    strcpy(global->airports[global->airports_counter].country, country);
    strcpy(global->airports[global->airports_counter].city, city);
    global->airports_counter++;
    printf("airport %s", id);
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


