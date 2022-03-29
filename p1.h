/* ist1102416 - Tomás Simões */

#ifndef MAIN_H
#define MAIN_H

/* UNDEFINED */
#define MAX_ARGUMENT_SIZE 50
#define MAX_ARGUMENT_NUMBER 100

/* DATE */
#define STARTING_DATE 01012022 /* 1 January 2022 - DDMMAAAA */
#define MAX_DATE 31122023 /* 31 December 2023 - DDMMAAAA */

/* AIRPORTS */
#define MAX_AIRPORTS 40
#define MAX_IDENTIFIER 4
#define MAX_COUNTRY_CHARS 30
#define MAX_CITY_CHARS 50

/* FLIGHTS */
#define MAX_FLIGHTS 30000
#define MAX_FLIGHT_CODE_CHARS 7
#define MIN_FLIGHT_CODE 1
#define MAX_FLIGHT_CODE 9999
#define MIN_MAX_PASSENGERS 10
#define MAX_MAX_PASSENGERS 100

/* data structs */

typedef struct {
    char id[MAX_IDENTIFIER];
    char country[MAX_COUNTRY_CHARS];
    char city[MAX_CITY_CHARS];
} airport;

typedef struct {
    char flight_code[MAX_FLIGHT_CODE_CHARS];
    airport * departure_airport;
    airport * arrival_airport;
    int departure_date; /* DDMMAAAA */
    int departure_time;   /* DDMMAAAA */
    int flight_duration;
    int max_passengers;
} flight;

typedef struct
{
    airport airports[MAX_AIRPORTS];
    int airports_counter;
    flight flights[MAX_FLIGHTS];
    int flights_counter;
    int date;

} state;

/* main.c */
int handler();
int add_airport();
int list_airport();
int add_list_flights();
int departure_flights();
int arrival_flights();
int advance_date();
int binary_search();
int bubble_sort();
airport* helper_find_airport();
int helper_find_departing_flights();
int convert_date();
int convert_time();
void date_to_human();
void time_to_human();
int helper_get_int_len();
#endif
