#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "zipData.h"
#define PI 3.14159265


FILE* filename(char file_name[]) {
    FILE *in = fopen(file_name, "r");

    return in;
}


void postal(char searched_zipcode[], FILE* in, FILE* out) {
    rewind(in);
    fprintf(out, "postal %s\n", searched_zipcode);

    int found_postal = 0;

    while (!feof(in)) {
        char zipcode[10];
        double laditude = 0;
        double longitude = 0;
        char country_abbreviation[10];
        char place_name[25] = {'\0'};
        char place_address[100] = {'\0'};
        char restOfLine[25] = {'\0'};

        readFileLine(in,zipcode, &laditude, &longitude, 
            country_abbreviation, place_name, place_address, restOfLine);


        if (strcmp(zipcode, searched_zipcode) == 0) {
             fprintf(out, "\t%s\t%s\t%s\t%s\t\t%0.4lf\t%0.4lf\t\t%s\n", 
                country_abbreviation, zipcode, place_name, 
                place_address, laditude, longitude, restOfLine);

            found_postal = 1;
        }
    }

    if (found_postal == 0) {
        fprintf(out, "Sorry, postal code %s was not found.\n", searched_zipcode);
    }
}


void place(char searched_place_name[], FILE* in, FILE* out) {
    rewind(in);
    fprintf(out, "place %s\n", searched_place_name);

    int found_place = 0;

    while (!feof(in)) {
        char zipcode[10];
        double laditude = 0;
        double longitude = 0;
        char country_abbreviation[10];
        char place_name[25] = {'\0'};
        char place_address[50] = {'\0'};
        char restOfLine[25] = {'\0'};

        readFileLine(in,zipcode, &laditude, &longitude, 
            country_abbreviation, place_name, place_address, restOfLine);

        if (strstr(place_name, searched_place_name) != NULL) {
            fprintf(out, "\t%s\t%s\t%s\t%s\t\t%lf\t%lf\t\t%s\n", 
                country_abbreviation, zipcode, place_name, 
                place_address, laditude, longitude, restOfLine);

            found_place = 1;
        }
    }

    if (found_place == 0) {
        fprintf(out, "Sorry, place name %s was not found.\n", searched_place_name);
    }
}


void county(char county_name[], FILE* in, FILE* out) {
    rewind(in);
    fprintf(out, "county %s\n", county_name);

    int found_place = 0;

    while (!feof(in)) {
        char zipcode[10];
        double laditude = 0;
        double longitude = 0;
        char country_abbreviation[10];
        char place_name[25] = {'\0'};
        char place_address[50] = {'\0'};
        char restOfLine[25] = {'\0'};

        readFileLine(in,zipcode, &laditude, &longitude, 
            country_abbreviation, place_name, place_address, restOfLine);

        if (strstr(place_address, county_name) != NULL) {
            fprintf(out, "\t%s\t%s\t%s\t%s\t\t%lf\t%lf\t\t%s\n", 
                country_abbreviation, zipcode, place_name, 
                place_address, laditude, longitude, restOfLine);

            found_place = 1;
        }
    }

    if (found_place == 0) {
        fprintf(out, "Sorry, county %s was not found.\n", county_name);
    }
}


void distance(char zipcode1[], char zipcode2[], FILE* in, FILE* out) {
    rewind(in);
    fprintf(out, "distance %s %s\n", zipcode1, zipcode2);

    int found_zip_1 = 0;
    int found_zip_2 = 0;
    float lat1 = 0;
    float lat2 = 0;
    float long1 = 0; 
    float long2 = 0;

    int i = 0;

    while (!feof(in)) {
        char zipcode[10];
        double laditude = 0;
        double longitude = 0;
        char country_abbreviation[10];
        char place_name[25] = {'\0'};
        char place_address[50] = {'\0'};
        char restOfLine[25] = {'\0'};

        readFileLine(in,zipcode, &laditude, &longitude, 
            country_abbreviation, place_name, place_address, restOfLine);
    
        
        if (strcmp(zipcode, zipcode1) == 0 && found_zip_1 == 0) {
             fprintf(out, "\t%s\t%s\t%s\t%s\t\t%lf\t%lf\t\t%s\n", 
                country_abbreviation, zipcode, place_name, 
                place_address, laditude, longitude, restOfLine);

            lat1 = laditude;
            long1 = longitude;

            found_zip_1 = 1;
        }
        if (strcmp(zipcode, zipcode2) == 0 && found_zip_2 == 0) {
             fprintf(out, "\t%s\t%s\t%s\t%s\t\t%lf\t%lf\t\t%s\n", 
                country_abbreviation, zipcode, place_name, 
                place_address, laditude, longitude, restOfLine);

            lat2 = laditude;
            long2 = longitude;

            found_zip_2 = 1;
        }
        i++;
    }

    if (found_zip_1 == 0) {
        fprintf(out, "Sorry, postal code %s was not found.\n", zipcode1);
    } 
    if (found_zip_2 == 0) {
        fprintf(out, "Sorry, postal code %s was not found.\n", zipcode2);
    }
    if (found_zip_1 != 0 && found_zip_2 != 0)
    {
        fprintf(out, "\tDistance: %f km\n", haversineFormula(lat1, lat2, long1, long2));
    }
}


float haversineFormula(float lat1, float lat2, float long1, float long2) {
    const float earth_rad = 6371000;
    
    const float a = lat1 * PI / 180;
    const float b = lat2 * PI / 180;
    const float c = (lat2-lat1) * PI / 180;
    const float d = (long2-long1) * PI / 180;

    const float haversine = sin(c/2) * sin(c/2) + cos(a) * cos(b) *  sin(d/2) * sin(d/2);
    const float num = 2 * atan2(sqrt(haversine), sqrt(1-haversine));

    float dist = earth_rad * num;

    return dist / 1000;
}


void readFileLine(FILE* in, char zipcode[], double *laditude, double *longitude, 
    char country_abbreviation[], char place_name[], char place_address[], char restOfLine[]) {

    fscanf(in, "%[^\t]", country_abbreviation);
    getc(in);
    fscanf(in, "%[^\t]", zipcode);
    fgetc(in);
    fscanf(in, "%[^\t]", place_name); //this line is broken b/c theres no number before the lat and long
    fscanf(in, "%[^\t]", country_abbreviation);

    char place_address_part[50] = {'\0'};

    while (!isdigit(place_address_part[0])) {
        fgetc(in);

        fscanf(in, "%[^\t]", place_address_part);
        strcat(place_address, place_address_part);
        strcat(place_address, "\t");
    }

        fscanf(in, "%[^\t]", country_abbreviation);


    fscanf(in, "%lf\t", laditude);
    fscanf(in, "%lf\t", longitude);
    fscanf(in, "%[^\n]\n", restOfLine);    
    
    /*
    fprintf(stdout, "country abreviation is: %s\n", country_abbreviation);
    fprintf(stdout, "zipcode is: %s\n", zipcode);
    fprintf(stdout, "place is: %s\n", place_name);
    fprintf(stdout, "place address is: %s\n", place_address);
    fprintf(stdout, "laditude is: %lf\n", *laditude);
    fprintf(stdout, "longitude is: %lf\n", *longitude);
    fprintf(stdout, "rest of line is: %s\n\n", restOfLine);
    */
}
