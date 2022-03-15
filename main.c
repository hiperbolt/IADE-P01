/* ist1102416 - Tomás Simões */
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "main.h"


int main(){
    printf("Introduza comando: ");

    /*  While the handler returns 1, keep running.
        If 1 is returned, q was pressed, so we exit. */
    while (handler()) {}
    
    return 0;


}

/*  Handles command input. */
int handler() {
    char in[100];

    /*  We use fgets() instead of scanf
        because its cleaner (in comparison to using scanf("%[^\n]",in) ) and safer
        than using gets(). */
    fgets(in, 100, stdin);

    switch (in[0]) {
        case 'q': 
            return 0;
        case 'a':
            add_airport(in);
            return 1;
        case 'l':
            list_airport(in);
            return 1;
        case 'v':
            add_list_flights(in);
            return 1;
        case 'p':
            departure_flights(in);
            return 1;
        case 'c':
            arrival_flights(in);
            return 1;
        case 't':
            advance_date(in);
            return 1;
        default:
            return 1;
    }
}

int add_airport(char *in) {
    return 1;
}

int list_airport(char *in) {
    return 1;
}

int add_list_flights(char *in) {
    return 1;
}

int departure_flights(char *in) {
    return 1;
}

int arrival_flights(char *in) {
    return 1;
}

int advance_date(char *in) {
    return 1;
}



