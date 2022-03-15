/* ist1102416 - Tomás Simões */

#ifndef MAIN_H
#define MAIN_H

/* DATE */
#define STARTING_DATE 1640995200 /* 1 January 2022 00:00:00 */
#define MAX_DATE 1704067199 /* 31 December 2023 23:59:59 */

/* AIRPORTS */
#define MAX_AIRPORTS 40
#define MAX_IDENTIFIER 3
#define MAX_COUNTRY_CHARS 30
#define MAX_CITY_CHARS 50

/* FLIGHTS */
#define MAX_FLIGHTS 30000
#define MIN_FLIGHT_CODE 1
#define MAX_FLIGHT_CODE 9999
#define MIN_MAX_PASSENGERS 10
#define MAX_MAX_PASSENGERS 100

/* main.c */
int handler();
int add_airport();
int list_airport();
int add_list_flights();
int departure_flights();
int arrival_flights();
int advance_date(); 

/* data structs */

typedef struct {
    char id[MAX_IDENTIFIER];
    char country[MAX_COUNTRY_CHARS];
    char city[MAX_CITY_CHARS];
} airport;

typedef struct {
    char flight_code[6];
    airport departure_airport;
    airport arrival_airport;
    unsigned long departure_time;
    unsigned long arrival_time;
    int flight_duration;
    int max_passengers;
} flight;


#endif