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
    char * arguments[MAX_ARGUMENT_NUMBER];
    memset(&arguments[0], 0, sizeof(arguments));
    char * pch;
    pch = strtok (in," \n");
    arguments[0] = pch;
    int i = 1;
    while (pch != NULL)
    {
        pch = strtok (NULL, " \n");
        if (pch != NULL) {
            arguments[i++] = pch;
        }
    }


    switch (in[0]) {
        case 'q': 
            return 0;
        case 'a':
            add_airport(global, arguments[1], arguments[2], arguments[3]);
            return 1;
        case 'l':
            list_airport(global, arguments);
            return 1;
        case 'v':
            add_list_flights(global, arguments);
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
        printf("too many airports\n");
        return 1;
    }

    int i;
    for (i = 0; i < sizeof(global->airports)/sizeof(global->airports[0]); i++) {
        if(!strcmp(global->airports[i].id, id)) {   
            printf("duplicate airport\n");
            return 1;
        }
    }
    
    int j;
    for (j = 0; j < MAX_IDENTIFIER; j++) {
        if (islower(id[j]) != 0){
            printf("invalid airport ID\n");
            return 1;
        }
    }

    strcpy(global->airports[global->airports_counter].id, id);
    strcpy(global->airports[global->airports_counter].country, country);
    strcpy(global->airports[global->airports_counter].city, city);
    global->airports_counter++;
    printf("airport %s\n", id);
    return 1;
}


int list_airport(state *global, char** arguments) {
    /*  We define the arguments array statically to avoid
        dynamic memory allocation */

    if (arguments[1] == NULL) {
        /*  If the command has no arguments,
            we sort alphabetically */
        /*bubble_sort();*/
        return 1;
    } else {
          /* We prepare the array for the biggest possible size
            it *could* occupy if all the arguments were their max.
            size. The 5 is to account for 4 whitespaces + a "#" */
        int max_result_size = MAX_IDENTIFIER + MAX_CITY_CHARS + MAX_COUNTRY_CHARS + 5;
        char airport_listing[MAX_AIRPORTS][max_result_size];

        int i;
        for (i=1; arguments[i] != NULL ; i++) {
            /* We receive the current airport's pointer from the auxiliary function */
            airport *current_airport = helper_find_airport(global, arguments[i]);

            /* We use another auxilary routine to calculate how many airports
               exit from the previously obtained airport */
            int departing_flights = helper_find_departing_flights(global, current_airport);

            printf("%s %s %s #%d\n", current_airport->id, current_airport->country, current_airport->city, departing_flights);
        }

    }
    return 1;
}

int add_list_flights(state *global, char** arguments) {
    if (arguments[1] == "NULL") {
        /*helper_sort_flight_creation_date();*/
        return 1;
    } else {
        /*  We create a new flight */
        char flight_code[MAX_FLIGHT_CODE_CHARS];
        char departure_id[MAX_IDENTIFIER];
        char arrival_id[MAX_IDENTIFIER];
        /*  Its not good practice to have magic numbers not defined
            in the header file, however, given that they are
            formatting-related, it doesn't seem too bad. */
        char departure_date[11]; /* "DD-MM-AAAA/0" takes 11 chars */
        char departure_time[6]; /* "HH:MM/0" take 6 chars */
        char flight_duration[6]; /* same format as departure_time */
        int max_passengers;

        strcpy(flight_code, arguments[1]);
        strcpy(departure_id, arguments[2]);
        strcpy(arrival_id, arguments[3]);
        strcpy(departure_date, arguments[4]);
        strcpy(departure_time, arguments[5]);
        strcpy(flight_duration, arguments[6]);
        max_passengers = atoi(arguments[7]);

        /* First, we check for possible exceptions */
        /*int i;
        for (i = 0; i < MAX_FLIGHT_CODE_CHARS; i++) {
            if (i < 2) {
                if (islower(arguments[1][i]) != 0) {
                    printf("invalid flight code");
                    return 1;
                }
            }
            if ()
        }*/

        int i;
        for (i = 0; i < sizeof(global->flights)/sizeof(global->flights[0]); i++) {
            if(!strcmp(global->flights[i].flight_code, arguments[1])) {   
                printf("flight already exists\n");
                return 1;
            }
        }

        int departure_airport_exists = 0;
        int arrival_airport_exists = 0;
        for (i = 0; i < sizeof(global->airports)/sizeof(global->airports[0]); i++) {
            if(!strcmp(global->airports[i].id, departure_id)) {   
                departure_airport_exists = 1;
            }
            if(!strcmp(global->airports[i].id, departure_id)) {   
                arrival_airport_exists = 1;
            }
        }
        if (departure_airport_exists == 0) {
            printf("%s: no such airport ID", departure_id);
            return 1;
        }
        if (arrival_airport_exists == 0) {
            printf("%s: no such airport ID", arrival_id);
            return 1;
        }

        if (global->flights_counter + 1 > MAX_FLIGHTS)
        {
            printf("too many flights\n");
            return 1;
        }



        airport * departure_airport_ptr = helper_find_airport(global, departure_id);
        airport * arrival_airport_ptr = helper_find_airport(global, arrival_id);

        /* TODO: convert dates and time */
        int converted_departure_time = 0;
        int arrival_time = 0;
        int converted_flight_duration = 0;

        strcpy(global->flights[global->flights_counter].flight_code, flight_code);
        global->flights[global->flights_counter].departure_airport = departure_airport_ptr;
        global->flights[global->flights_counter].arrival_airport = arrival_airport_ptr;
        global->flights[global->flights_counter].departure_time = converted_departure_time;
        global->flights[global->flights_counter].arrival_time = arrival_time;
        global->flights[global->flights_counter].flight_duration = converted_flight_duration;
        global->flights[global->flights_counter].max_passengers = max_passengers;
        global->flights_counter++;
    }
    
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


int bubble_sort(int idk) {
    /*  TO IMPLEMENT */
    return 1;
}

airport* helper_find_airport(state *global, char airport_name[]) {
    /*
        Auxiliary function that searches for a specific
        airport and returns its information a pointer to an
        array with a copy of it.
    */
    int i;
    for(i=0; i < global->airports_counter; i++) {
        if (!strncmp(global->airports[i].id, airport_name, MAX_IDENTIFIER)) {
            return &global->airports[i];
        }
    }
    /*
        A possible optimization would be to keep track of
        previously found airports so that some comparison
        instructions would be saved.
        Realistically, this would do little to speed up the search.
    */
}

int helper_find_departing_flights(state *global, airport *departing_airport) {
    /*
        Auxiliary function that calculates the number of
        flights departing from a specific airport.
        Recieves the airport pointer and returns
        the number in integer.
    */
   int i;
   int departing_flights = 0;
   for (i = 0; i < global->flights_counter; i++) {
        /* If they point to the same object */
        if (global->flights[i].departure_airport == departing_airport){
            departing_flights++;
        }
   }
   return departing_flights;
}
