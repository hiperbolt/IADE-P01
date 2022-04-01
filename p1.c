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
    global.current_date.day = STARTING_DAY;
    global.current_date.month = STARTING_MONTH;
    global.current_date.year = STARTING_YEAR;

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
    pch = strtok(in, " \n\t");
    arguments[0] = pch;
    i = 1;
    while (pch != NULL)
    {
        pch = strtok(NULL, " \n\t");
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
        add_airport(global, arguments);
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

int add_airport(state *global, char **arguments) {
    char id[MAX_IDENTIFIER];
    char country[MAX_COUNTRY_CHARS];
    char city[MAX_CITY_CHARS];
    strcpy(id, arguments[1]);
    strcpy(country, arguments[2]);
    strncat(city, arguments[3], strlen(arguments[3]) + 1);

    a = 4;
    while (arguments[a] != 0)
    {
        strncat(city, " ", 2);
        strncat(city, arguments[a], strlen(arguments[a]) + 1);
        a++;
    }
    
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
        airport * airport_codes_ptr_arr[MAX_AIRPORTS];
        for (a = 0; a < global->airports_counter; a++)
        {
            airport_codes_ptr_arr[a] = &(global->airports[a]);
        }

        bubble_sort(airport_codes_ptr_arr, sizeof(airport*), global->airports_counter, helper_flights_code_compare);
        /*
            We now print the airport codes in alphabetical order.
            We use the helper function helper_flights_code_compare
            to sort the array of pointers to airport codes.
        */
        for (a = 0; a < global->airports_counter; a++)
        {
            /* We use another auxilary routine to calculate how many flights
               exit from the previously obtained airport */
            int departing_flights = helper_find_departing_flights(global, airport_codes_ptr_arr[a]);

            printf("%s %s %s %d\n", airport_codes_ptr_arr[a]->id, airport_codes_ptr_arr[a]->city, airport_codes_ptr_arr[a]->country, departing_flights);
        }
        return 1;
    }
    else
    {
        airport *current_airport;
        int departing_flights;
        for (i = 1; arguments[i] != NULL; i++)
        {
            if (helper_find_airport(global, arguments[i]) == NULL)
            {
                printf("%s: no such airport ID\n", arguments[i]);
                continue;
            }
            
            /* We receive the current airport's pointer from the auxiliary function */
            current_airport = helper_find_airport(global, arguments[i]);

            /* We use another auxilary routine to calculate how many flights
               exit from the previously obtained airport */
            departing_flights = helper_find_departing_flights(global, current_airport);

            printf("%s %s %s %d\n", current_airport->id, current_airport->city, current_airport->country, departing_flights);
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
        airport *departure_airport_ptr;
        airport *arrival_airport_ptr;

        date_struct converted_departure_date;
        time_struct converted_departure_time, converted_flight_duration;

        /*  We create a new flight */
        char flight_code[MAX_FLIGHT_CODE_CHARS], departure_id[MAX_IDENTIFIER], arrival_id[MAX_IDENTIFIER];
        /*  Its not good practice to have magic numbers not defined
            in the header file, however, given that they are
            formatting-related, it doesn't seem too bad. */
        /* "DD-MM-AAAA/0" takes 11 chars */
        /* "HH:MM/0" take 6 chars */
        char departure_date[11], departure_time[6], flight_duration[6], comparison_departure_date[11];
        int num_passengers;

        strcpy(flight_code, arguments[1]);
        strcpy(departure_id, arguments[2]);
        strcpy(arrival_id, arguments[3]);
        strcpy(departure_date, arguments[4]);
        strcpy(departure_time, arguments[5]);
        strcpy(flight_duration, arguments[6]);
        num_passengers = atoi(arguments[7]);

        departure_airport_ptr = helper_find_airport(global, departure_id);
        arrival_airport_ptr = helper_find_airport(global, arrival_id);

        convert_date(departure_date, &converted_departure_date);
        convert_time(departure_time, &converted_departure_time);
        convert_time(flight_duration, &converted_flight_duration);

        /* First, we check for possible exceptions */

        if (strlen(flight_code) < 3 || strlen(flight_code) > 6)
        {
            printf("invalid flight code\n");
            return 1;
        }
        for (i = 0; i < strlen(flight_code); i++) {
            if (i < 2) {
                if (islower(flight_code[i]) != 0) {
                    printf("invalid flight code\n");
                    return 1;
                }
            } else {
                if (isdigit(flight_code[i]) == 0) {
                    printf("invalid flight code\n");
                    return 1;
                }
            }
        }

        for (i = 0; i < sizeof(global->flights) / sizeof(global->flights[0]); i++)
        {
            date_to_human(comparison_departure_date, &global->flights[i].departure_date);
            if (!strcmp(global->flights[i].flight_code, arguments[1]) && !strcmp(comparison_departure_date, departure_date))
            {
                printf("flight already exists\n");
                return 1;
            }
        }

        if (helper_find_airport(global, departure_id) == NULL)
        {
            printf("%s: no such airport ID\n", departure_id);
            return 1;
        }
        if (helper_find_airport(global, arrival_id) == NULL)
        {
            printf("%s: no such airport ID\n", arrival_id);
            return 1;
        }

        if (global->flights_counter + 1 > MAX_FLIGHTS)
        {
            printf("too many flights\n");
            return 1;
        }

        if (validate_date(global, convert_to_epoch(converted_departure_date.day, converted_departure_date.month, converted_departure_date.year)))
        {
            printf("invalid date\n");
            return 1;
        }
        if ((converted_flight_duration.hour * 60) + (converted_flight_duration.min) > 720) /* 12 * 60 = 720 minutes */
        {
            printf("invalid duration\n");
            return 1;
        }
        if (num_passengers < 10 || num_passengers > 100)
        {
            printf("invalid capacity\n");
            return 1;
        }

        strcpy(global->flights[global->flights_counter].flight_code, flight_code);
        global->flights[global->flights_counter].departure_airport = departure_airport_ptr;
        global->flights[global->flights_counter].arrival_airport = arrival_airport_ptr;
        global->flights[global->flights_counter].departure_date = converted_departure_date;
        global->flights[global->flights_counter].departure_time = converted_departure_time;
        global->flights[global->flights_counter].flight_duration = converted_flight_duration;
        global->flights[global->flights_counter].max_passengers = num_passengers;
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
    flight *flights_ptr_arr[MAX_FLIGHTS] = {0};
    char buffer_departure_date[11];
    char buffer_departure_time[6];
    int number_of_flights = 0;

    /* We check for a possible "no such airport" exception */
    if (helper_find_airport(global, arguments[1]) == NULL)
    {
        printf("%s: no such airport ID\n", arguments[1]);
        return 1;
    } 

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

    if (number_of_flights)
    {
        bubble_sort(flights_ptr_arr, sizeof(flight *), number_of_flights, helper_flights_departure_datetime_compare);
    }

    for (a = 0; a < number_of_flights; a++)
    {
        date_to_human(buffer_departure_date, &flights_ptr_arr[a]->departure_date);
        time_to_human(buffer_departure_time, &flights_ptr_arr[a]->departure_time);
        printf("%s %s %s %s\n", flights_ptr_arr[a]->flight_code, flights_ptr_arr[a]->arrival_airport->id, buffer_departure_date, buffer_departure_time);
    }
    
    return 1;
}

int arrival_flights(state *global, char **arguments)
{
    flight *flights_ptr_arr[MAX_FLIGHTS] = {0};
    date_struct buffer_arrival_date = {0};
    time_struct buffer_arrival_time = {0};
    char buffer_arrival_date_human[11];
    char buffer_arrival_time_human[6];
    int number_of_flights = 0;

    /* We check for a possible "no such airport" exception */
    if (helper_find_airport(global, arguments[1]) == NULL)
    {
        printf("%s: no such airport ID\n", arguments[1]);
        return 1;
    }

    for (a = 0; a < global->flights_counter; a++)
    {
        /*  We check if the current indexed flight's departure airport
            points to the argument provided airport */
        if (global->flights[a].arrival_airport == helper_find_airport(global, arguments[1]))
        {
            flights_ptr_arr[number_of_flights] = &global->flights[a];
            number_of_flights++;
        }
    }

    if (number_of_flights)
    {
        bubble_sort(flights_ptr_arr, sizeof(flight *), number_of_flights, helper_flights_arrival_datetime_compare);
    }

    for (a = 0; a < number_of_flights; a++)
    {
        
        arrival_datetime_calculate(&buffer_arrival_date, &buffer_arrival_time, &(flights_ptr_arr[a]->departure_date), &(flights_ptr_arr[a]->departure_time), &(flights_ptr_arr[a]->flight_duration));
        date_to_human(buffer_arrival_date_human, &buffer_arrival_date);
        time_to_human(buffer_arrival_time_human, &buffer_arrival_time);
        
        printf("%s %s %s %s\n", flights_ptr_arr[a]->flight_code, flights_ptr_arr[a]->departure_airport->id, buffer_arrival_date_human, buffer_arrival_time_human);
    }

    return 1;
}

int advance_date(state *global, char **arguments)
{
    date_struct new_date;
    char buffer[11];

    /* We need to check if the date is in the past, of if it's too ahead in the future */
    convert_date(arguments[1], &new_date);
    if (validate_date(global, convert_to_epoch(new_date.day, new_date.month, new_date.year)))
    {
        printf("invalid date\n");
        return 1;
    }
    
    global->current_date = new_date;
    date_to_human(buffer, &new_date);
    printf("%s\n", buffer);
    return 1;
}

void bubble_sort(void *arr, unsigned int type_size,unsigned int number_elements, long (*comparison)(const void *, const void *))
{
    char *arr_bytes = arr;
    for (i = 0; i < number_elements - 1; i++)
    {
        for (j = 0; j < number_elements - i - 1; j++)
        {
            if (comparison(arr_bytes + j * type_size, arr_bytes + (j + 1) * type_size) > 0)
            {
                void *temp = malloc(type_size);
                memcpy(temp, arr_bytes + j * type_size, type_size);
                memcpy(arr_bytes + j * type_size, arr_bytes + (j + 1) * type_size, type_size);
                memcpy(arr_bytes + (j + 1) * type_size, temp, type_size);
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
    return NULL;
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

long helper_get_datetime(date_struct date, time_struct time)
{
    return (((long)date.year * 100000000) + ((long)date.month * 1000000) + ((long)date.day * 10000) + ((long)time.hour * 100) + (long)time.min);
}

long helper_flights_departure_datetime_compare(const void *flight1, const void *flight2)
{
    /*  Returns >0 if the datetime of flight1 is larger than flight2 */
    const flight *casted_flight1 = *(flight **)flight1;
    const flight *casted_flight2 = *(flight **)flight2;
    const long result = helper_get_datetime(casted_flight1->departure_date, casted_flight1->departure_time) - helper_get_datetime(casted_flight2->departure_date, casted_flight2->departure_time);
    return result;
}

long helper_flights_arrival_datetime_compare(const void *flight1, const void *flight2)
{
    /*  Returns >0 if the datetime of flight1 is larger than flight2 */
    const flight *casted_flight1 = *(flight **)flight1;
    const flight *casted_flight2 = *(flight **)flight2;

    date_struct arrival_date_f1 = {0};
    time_struct arrival_time_f1 = {0};

    date_struct arrival_date_f2 = {0};
    time_struct arrival_time_f2 = {0};

    arrival_datetime_calculate(&arrival_date_f1, &arrival_time_f1, &casted_flight1->departure_date, &casted_flight1->departure_time, &casted_flight1->flight_duration);
    arrival_datetime_calculate(&arrival_date_f2, &arrival_time_f2, &casted_flight2->departure_date, &casted_flight2->departure_time, &casted_flight2->flight_duration);

    return helper_get_datetime(arrival_date_f1, arrival_time_f1) - helper_get_datetime(arrival_date_f2, arrival_time_f2);  
}


long helper_flights_code_compare(const void * airport1, const void * airport2){
    const airport * casted_airport1 = * (airport **)airport1;
    const airport * casted_airport2 = * (airport **)airport2;
    return strncmp(casted_airport1->id, casted_airport2->id, MAX_IDENTIFIER);
}

long convert_to_epoch(int d, int m, int y){
    /* Recieves a date and transforms into unix epoch (ignoring leap years) */
    long epoch_time = 0;
    for (a = EPOCH_YEAR; a < y; a++)
    {
        epoch_time += 8760; /* 8760 hours in a normal calendar year */
    }

    for (a = EPOCH_MONTH; a < m; a++)
    {
        if (a == 2)
        {
            /*February*/
            epoch_time += 672; /* 28*24 */
        } else {
            if (a % 2 == 0)
            {
                epoch_time += 720; /* 31*24 */
            } else {
                epoch_time += 744; /* 30*24 */
            }  
        }
    }

    for (a = EPOCH_DAY; a < d; a++)
    {
        epoch_time += 24;
    }
    
    return epoch_time;
}

void arrival_datetime_calculate(date_struct* date_buffer, time_struct* time_buffer, date_struct* departure_date, time_struct* departure_time, time_struct* duration) {
    int departure_year = departure_date->year;
    int departure_month = departure_date->month;
    int departure_day = departure_date->day;
    int departure_hour = departure_time->hour;
    int departure_min = departure_time->min;
    int duration_hour = duration->hour;
    int duration_min = duration->min;
    int carry = 0;

    departure_min += duration_min;
    if (departure_min >= 60)
    {
        carry = 1;
        departure_min = departure_min % 60;
    }
    
    departure_hour += duration_hour + carry;
    carry = 0;
    if (departure_hour >= 24)
    {
        carry = 1;
        departure_hour = departure_hour % 24;
    }

    departure_day += carry;
    carry = 0;

    if (departure_day > 30 && (departure_month == 4 || departure_month == 6 || departure_month == 9 || departure_month == 11))
    {
        carry = 1;
        departure_day = departure_day % 30;
    }
    else if (departure_day > 31 && (departure_month == 1 || departure_month == 3 || departure_month == 5 || departure_month == 7 || departure_month == 8 || departure_month == 10 || departure_month == 12))
    {
        carry = 1;
        departure_day = departure_day % 31;
    }
    else if (departure_day > 29 && departure_month == 2 && (departure_year % 4 == 0))
    {
        carry = 1;
        departure_day = departure_day % 29;
    }
    else if (departure_day > 28 && departure_month == 2 && (departure_year % 4 != 0))
    {
        carry = 1;
        departure_day = departure_day % 28;
    }
    
    departure_month += carry;
    carry = 0;
    if (departure_month > 12)
    {
        carry = 1;
        departure_month = 1;
    }

    departure_year += carry;
    carry = 0;

    date_buffer->day = departure_day;
    date_buffer->month = departure_month;
    date_buffer->year = departure_year;
    time_buffer->hour = departure_hour;
    time_buffer->min = departure_min;
}


int validate_date(state* global, long new_epoch_date) {
    /*  Helper function that guarantees that the date isn't in the past, and isn't more
        than one year in the future.
        Returns 0 if valid, -1 if invalid */
    long current_epoch_date, year_ahead_epoch_date;

    current_epoch_date = convert_to_epoch(global->current_date.day, global->current_date.month, global->current_date.year);
    year_ahead_epoch_date = convert_to_epoch(global->current_date.day, global->current_date.month, global->current_date.year+1);

    if (new_epoch_date < STARTING_EPOCH || new_epoch_date < current_epoch_date || new_epoch_date > year_ahead_epoch_date || new_epoch_date > MAX_EPOCH)
    {
        return -1;
    }
    return 0;
}