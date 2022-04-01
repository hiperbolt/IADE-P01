/* ist1102416 - Tomás Simões */
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "p1.h"

/*  We declare the variable i and j only once and globally
    since they are used for various instances of iteration */
size_t i, j;

/*  Because of different signedness errors, we sometimes need an actual *int* type
    to iterate against */
int a;

int main()
{

    state global = {0};

    printf("Introduza comando: ");

    /*  While the handler returns 1, keep running.
        If 1 is returned, q was pressed, so we exit. */
    while (handler(&global))
    {
    }
    return 0;
}

/*  Handles command input. */
int handler(state *global)
{
    char in[100];
    char *arguments[MAX_ARGUMENT_NUMBER];
    char *pch;

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
    memset(&arguments[0], 0, sizeof(arguments));
    pch = strtok(in, " \n");
    arguments[0] = pch;
    i = 1;
    while (pch != NULL)
    {
        pch = strtok(NULL, " \n");
        if (pch != NULL)
        {
            arguments[i++] = pch;
        }
    }

    switch (in[0])
    {
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
        departure_flights(global, arguments);
        return 1;
    case 'c':
        arrival_flights(global, arguments);
        return 1;
    case 't':
        advance_date(global, arguments);
        return 1;
    default:
        return 1;
    }
}

int add_airport(state *global, char *id, char *country, char *city)
{
    if (global->airports_counter + 1 > MAX_AIRPORTS)
    {
        printf("too many airports\n");
        return 1;
    }

    for (i = 0; i < sizeof(global->airports) / sizeof(global->airports[0]); i++)
    {
        if (!strcmp(global->airports[i].id, id))
        {
            printf("duplicate airport\n");
            return 1;
        }
    }

    for (j = 0; j < MAX_IDENTIFIER; j++)
    {
        if (islower(id[j]) != 0)
        {
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

int list_airport(state *global, char **arguments)
{
    /*  We define the arguments array statically to avoid
        dynamic memory allocation */

    if (arguments[1] == NULL)
    {
        /*  We first create an array of pointers to airport codes
            ISO C90 standard forbids variable lenght array's (unlike
            C99 for example). Therefor we can't do something like:

            char * airport_codes_ptr_arr[global->airports_counter];

            We have to resort to statically sized buffers.
        */
        char *airport_codes_ptr_arr[MAX_AIRPORTS];
        for (a = 0; a < global->airports_counter; a++)
        {
            airport_codes_ptr_arr[a] = &global->airports[a].id[0];
        }
        bubble_sort(airport_codes_ptr_arr, global->airports_counter);
        return 1;
    }
    else
    {
        for (i = 1; arguments[i] != NULL; i++)
        {
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

int add_list_flights(state *global, char **arguments)
{
    if (arguments[1] == NULL)
    {
        for (a = 0; a < global->flights_counter; a++)
        {
            char human_date[11];
            char human_time[6];
            date_to_human(&human_date, &global->flights[a].departure_date);
            time_to_human(&human_time, &global->flights[a].departure_time);
            printf("%s %s %s %s %s\n", global->flights[a].flight_code, global->flights[a].departure_airport->id, global->flights[a].arrival_airport->id, human_date, human_time);
            memset(&human_date[0], 0, sizeof(human_date));
            memset(&human_time[0], 0, sizeof(human_time));
        }

        return 1;
    }
    else
    {
        int departure_airport_exists = 0;
        int arrival_airport_exists = 0;

        airport *departure_airport_ptr;
        airport *arrival_airport_ptr;

        date_struct converted_departure_date;
        time_struct converted_departure_time;
        time_struct converted_flight_duration;

        /*  We create a new flight */
        char flight_code[MAX_FLIGHT_CODE_CHARS];
        char departure_id[MAX_IDENTIFIER];
        char arrival_id[MAX_IDENTIFIER];
        /*  Its not good practice to have magic numbers not defined
            in the header file, however, given that they are
            formatting-related, it doesn't seem too bad. */
        char departure_date[11]; /* "DD-MM-AAAA/0" takes 11 chars */
        char departure_time[6];  /* "HH:MM/0" take 6 chars */
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

        for (i = 0; i < sizeof(global->flights) / sizeof(global->flights[0]); i++)
        {
            if (!strcmp(global->flights[i].flight_code, arguments[1]))
            {
                printf("flight already exists\n");
                return 1;
            }
        }

        for (i = 0; i < sizeof(global->airports) / sizeof(global->airports[0]); i++)
        {
            if (!strcmp(global->airports[i].id, departure_id))
            {
                departure_airport_exists = 1;
            }
            if (!strcmp(global->airports[i].id, departure_id))
            {
                arrival_airport_exists = 1;
            }
        }
        if (departure_airport_exists == 0)
        {
            printf("%s: no such airport ID", departure_id);
            return 1;
        }
        if (arrival_airport_exists == 0)
        {
            printf("%s: no such airport ID", arrival_id);
            return 1;
        }

        if (global->flights_counter + 1 > MAX_FLIGHTS)
        {
            printf("too many flights\n");
            return 1;
        }

        departure_airport_ptr = helper_find_airport(global, departure_id);
        arrival_airport_ptr = helper_find_airport(global, arrival_id);

        convert_date(departure_date, &converted_departure_date);
        convert_time(departure_time, &converted_departure_time);
        convert_time(flight_duration, &converted_flight_duration);

        strcpy(global->flights[global->flights_counter].flight_code, flight_code);
        global->flights[global->flights_counter].departure_airport = departure_airport_ptr;
        global->flights[global->flights_counter].arrival_airport = arrival_airport_ptr;
        global->flights[global->flights_counter].departure_date = converted_departure_date;
        global->flights[global->flights_counter].departure_time = converted_departure_time;
        global->flights[global->flights_counter].flight_duration = converted_flight_duration;
        global->flights[global->flights_counter].max_passengers = max_passengers;
        global->flights_counter++;
    }

    return 1;
}

int departure_flights(state *global, char **arguments)
{
    /*  We create an array of pointers to the flights
        in a specific airport.
        As explained previously, C90 does not allow VLA,
        so we must define the array size as the max possible
        number of flights.
    */
    flight *flights_ptr_arr[MAX_FLIGHTS];
    int number_of_flights = 0;
    for (a = 0; a < global->flights_counter; a++)
    {
        /*  We check if the current indexed flight's departure airport
            points to the argument provided airport */
        if (global->flights[a].departure_airport == helper_find_airport(global, arguments[1]))
        {
            flights_ptr_arr[number_of_flights] = &global->flights[a];
            number_of_flights++;
        }
    }

    helper_bubblesort_datetime(global, flights_ptr_arr, number_of_flights);

    return 1;
}

int arrival_flights(state *global, char **arguments)
{
    printf("%s", global->airports[0].id);
    printf("%s", arguments[0]);
    return 1;
}

int advance_date(state *global, char **arguments)
{
    /* TODO: everything */
    return 1;
}

void bubble_sort(void *arr, size_t type_size, int number_elements, int (*comparison)(const void *, const void *))
{
    /*
        If comparison = 1, switch
        If comparison = 0, hold.
    */

    for (i = 0; i < number_elements - 1; i++)
    {
        for (j = 0; j < number_elements - i - 1; j++)
        {
            if (comparison(arr + j * type_size, arr + (j + 1) * type_size) > 0)
            {
                void *temp = malloc(type_size);
                memcpy(temp, (arr + j * type_size), type_size);
                memcpy((arr + j * type_size), (arr + (j+1) * type_size), type_size);
                memcpy((arr + (j+1) * type_size), temp, type_size);
                free(temp);
            }
        }
    }
}

airport *helper_find_airport(state *global, char airport_name[])
{
    /*
        Auxiliary function that searches for a specific
        airport and returns its information a pointer to an
        array with a copy of it.
    */
    int i;
    for (i = 0; i < global->airports_counter; i++)
    {
        if (!strncmp(global->airports[i].id, airport_name, MAX_IDENTIFIER))
        {
            return &global->airports[i];
        }
    }
    return &global->airports[MAX_AIRPORTS];
    /*
        A possible optimization would be to keep track of
        previously found airports so that some comparison
        instructions would be saved.
        Realistically, this would do little to speed up the search.
    */
}

int helper_find_departing_flights(state *global, airport *departing_airport)
{
    /*
        Auxiliary function that calculates the number of
        flights departing from a specific airport.
        Recieves the airport pointer and returns
        the number in integer.
    */
    int i;
    int departing_flights = 0;
    for (i = 0; i < global->flights_counter; i++)
    {
        /* If they point to the same object */
        if (global->flights[i].departure_airport == departing_airport)
        {
            departing_flights++;
        }
    }
    return departing_flights;
}

void convert_date(char *date, date_struct *date_strct)
{
    /*
        From the man page:
        "sscanf() reads its input from the character string pointed to by str"

        Therefor, we can use the same formatting options we would with
        scanf() and dump the result directly into a buffer.

    */
    sscanf(date, "%d-%d-%d", &date_strct->day, &date_strct->month, &date_strct->year);
}

void convert_time(char *time, time_struct *time_strct)
{
    sscanf(time, "%d:%d", &time_strct->hour, &time_strct->min);
}

void date_to_human(char *buffer, date_struct *date_strct)
{
    sprintf(buffer, "%02d-%02d-%02d", date_strct->day, date_strct->month, date_strct->year);
}

void time_to_human(char *buffer, time_struct *time_strct)
{
    sprintf(buffer, "%02d:%02d", time_strct->hour, time_strct->min);
}

int helper_get_datetime(date_struct date, time_struct time)
{
    return ((date.year * 100000000) + (date.month * 1000000) + (date.day * 10000) + (time.hour * 100) + time.min);
}

int helper_flights_datetime_compare(const void *flight1, const void *flight2)
{
    /*  Returns >0 if the datetime of flight1 is larger than flight2 */
    const flight *casted_flight1 = flight1;
    const flight *casted_flight2 = flight2;
    return helper_get_datetime(casted_flight1->departure_date, casted_flight1->departure_time) - helper_get_datetime(casted_flight2->departure_date, casted_flight2->departure_time);
}

void helper_bubblesort_datetime(state *global, flight **flights_arr, int number_of_flights)
{
    bubble_sort(flights_arr, sizeof(flight*), number_of_flights, helper_flights_datetime_compare);
    printf(flights_arr);
}