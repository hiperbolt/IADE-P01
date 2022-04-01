/* ist1102416 - Tomás Simões */

#ifndef MAIN_H
#define MAIN_H

/* UNDEFINED */
#define MAX_ARGUMENT_SIZE 50
#define MAX_ARGUMENT_NUMBER 100

/* DATE */
/* UNIX EPOCH */
#define EPOCH_DAY 01
#define EPOCH_MONTH 01
#define EPOCH_YEAR 1970
#define EPOCH_HOUR 0
#define EPOCH_MINUTE 0

/* 1 January 2022 */
#define STARTING_EPOCH 455520
#define STARTING_DAY 01
#define STARTING_MONTH 01
#define STARTING_YEAR 2022

/* 31 December 2023 */
#define MAX_EPOCH 473016 
#define MAX_DAY 31
#define MAX_MONTH 12
#define MAX_YEAR 2023

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
typedef struct
{
    int day;
    int month;
    int year;
} date_struct;

typedef struct
{
    int hour;
    int min;
} time_struct;

typedef struct {
    char id[MAX_IDENTIFIER];
    char country[MAX_COUNTRY_CHARS];
    char city[MAX_CITY_CHARS];
} airport;

typedef struct {
    char flight_code[MAX_FLIGHT_CODE_CHARS];
    airport * departure_airport;
    airport * arrival_airport;
    date_struct departure_date; /* DDMMAAAA */
    time_struct departure_time;   /* DDMMAAAA */
    time_struct flight_duration;
    int max_passengers;
} flight;

typedef struct
{
    airport airports[MAX_AIRPORTS];
    int airports_counter;
    flight flights[MAX_FLIGHTS];
    int flights_counter;
    date_struct current_date;

} state;

/* main.c */
int handler();
int add_airport();
int list_airport();
int add_list_flights();
int departure_flights();
int arrival_flights();
int advance_date();
void bubble_sort(void *arr, unsigned int type_size, unsigned int number_elements, long (*comparison)(const void *, const void *));
airport* helper_find_airport();
int helper_find_departing_flights();
void convert_date();
void convert_time();
void date_to_human();
void time_to_human();
long helper_get_datetime();
long helper_flights_departure_datetime_compare();
long helper_flights_arrival_datetime_compare();
long helper_flights_code_compare(const void * airport1, const void * airport2);
long convert_to_epoch(int d, int m, int y);
void arrival_datetime_calculate();
int validate_date();
#endif
