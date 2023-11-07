FILE* filename(char file_name[]);


void postal(char searched_zipcode[], FILE* in, FILE* out);


void county(char county_name[], FILE* in, FILE* out);


void place(char place_name[], FILE* in, FILE* out);


void distance(char zipcode1[], char zipcode2[], FILE* in, FILE* out);

float haversineFormula(float lat1, float lat2, float long1, float long2);

void readFileLine(FILE* in, char zipcode[], double *laditude, double *longitude, 
    char country_abbreviation[], char place_name[], 
    char place_address[], char restOfLine[]);