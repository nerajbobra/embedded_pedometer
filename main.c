#include "stdint.h"
#include "count_steps.h"
#include "stdio.h"  //using this for printing debug outputs
#include "math.h"   //using this for converting the CSV data from float to int

int main(int argc, const char * argv[]) {
    
#define NUM_SAMPLES_IN_CSV_FILE 400
    
    //hold the data from the CSV file in a fifo-like data structure where the accelerometer data looks like
    //[x1,y1,z1,x2,y2,z2...x400,y400,z400]
    int8_t acc[NUM_SAMPLES_IN_CSV_FILE*3] = {0};
    
    //load the CSV data. read the first line (header) separately
    FILE *f   = fopen("/Users/Neraj/Neraj/pedometer/build/Debug/peddata.csv", "r");
    char s[6] = {0};
    fscanf(f, "%s,%s,%s,%s,%s",&s[0],&s[1],&s[2],&s[3],&s[4]);
    
    uint16_t i    = 0;
    float    n[3] = {0};
    float    temp = 0;
    
    //scaling factor to convert the decimal data to int8 integers. calculated in matlab by taking the absolute value of all the data
    //and then calculating the max of that data. then divide that by 127 to get the scaling factor
    float scale_factor = 55.3293;
    while( fscanf(f, "%f,%f,%f", &n[0], &n[1], &n[2]) > 0 ) // read a line at a time
    {
        temp     = roundf(n[0]*scale_factor);
        acc[i++] = (int8_t)temp;
        
        temp     = roundf(n[1]*scale_factor);
        acc[i++] = (int8_t)temp;
        
        temp     = roundf(n[2]*scale_factor);
        acc[i++] = (int8_t)temp;
        
    }
    
    //pass data to step counting algorithm, 4 seconds at a time (which is the WINDOW_LENGTH). put the data into a temporary buffer each loop
    int8_t   data[NUM_TUPLES*3] = {0};
    uint8_t  num_segments       = NUM_SAMPLES_IN_CSV_FILE/(SAMPLING_RATE*WINDOW_LENGTH);
    uint16_t j                  = 0;
    uint8_t  num_steps          = 0;
    for (i = 0; i < num_segments; i++) {
        for (j = SAMPLING_RATE*WINDOW_LENGTH*i*3; j < SAMPLING_RATE*WINDOW_LENGTH*(i+1)*3; j++) {
            data[j-SAMPLING_RATE*WINDOW_LENGTH*i*3] = acc[j];
        }
        num_steps += count_steps(data);
    }
    printf("num steps: %i\n", num_steps);
}
