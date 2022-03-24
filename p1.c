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
    char * arguments[MAX_ARGUMENT_NUMBER] = {0};
    char * pch;
    pch = strtok (in," ");
    arguments[0] = pch;
    int i = 1;
    while (pch != NULL)
    {
        pch = strtok (NULL, " ");
        if (pch != NULL) {
            arguments[i++] = pch;
        }
    }


    switch (in[0]) {
        case 'q': 
            return 0;
        case 'a':
            printf("Antes de chamar: glob: %p, arguments: %p\n", global, arguments);
            add_airport(global, arguments[1], arguments[2], arguments[3]);
            return 1;
        case 'l':
            printf("Antes de chamar: glob: %p, arguments: %p\n", global, arguments);
            list_airport(global, arguments);
            return 1;
        case 'v':
            printf("Antes de chamar: glob: %p, arguments: %p\n", global, arguments);
            add_list_flights(global, &arguments);
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

    int i;
    for (i = 0; i < sizeof(global->airports)/sizeof(global->airports[0]); i++) {
        if(!strcmp(global->airports[i].id, id)) {   
            printf("duplicate airport");
            return 1;
        }
    }
    
    int j;
    for (j = 0; j > sizeof(id); j++) {
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


int list_airport(state *global, char** arguments) {
    /*  We define the arguments array statically to avoid
        dynamic memory allocation */

    if (arguments[1] == NULL) {
        /*  If the command has no arguments,
            we sort alphabetically */
        /*bubble_sort();*/
        printf("chegamos aqui");
        return 1;
    } else {
          /* We prepare the array for the biggest possible size
            it *could* occupy if all the arguments were their max.
            size. The 5 is to account for 4 whitespaces + a "#" */ 
        char airport_listing[MAX_IDENTIFIER + MAX_CITY_CHARS + MAX_COUNTRY_CHARS + MAX_FLIGHTS + 5][MAX_ARGUMENT_NUMBER];
        
        int i;
        for (i=0; i < sizeof(arguments)/sizeof(arguments[0]) - 1; i++) {

            /* strcpy(airport_listing[i], ("%s %s %s #%d", ) 
        }     
    } */
    printf(arguments);

    return 1;
}

int add_list_flights(state *global, char** arguments) {
    printf(arguments);
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


int bubble_sort(){
    /*  TO IMPLEMENT */
    return 1;
}