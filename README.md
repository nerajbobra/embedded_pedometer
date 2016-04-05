# embedded_pedometer
Accelerometer-based pedometer algorithm for embedded applications

This code implements a step counting algorithm using X,Y,Z accelerometer data. It is written in
C and is completely fixed point, with the intention of running in real time in an embedded system.
The core of the algorithm is the autocorrelation function, which can be used to find the periodicity
of a noisy signal in the time domain. An overview of the algorithm is as follows:
1. calculate the magnitude of the X,Y,Z accelerometer data
2. apply a low pass filter to the magnitude data
3. remove the mean
4. apply the autocorrelation
5. calculate the derivative and find the first zero crossing from positive to negative,
which corresponds to the first positive peak in the autocorrelation
6. using the result from the previous step, hone in on the exact autocorrelation peak index
7. calculate some basic statistics about the autocorrelation peak to determine if it corresponds
to actual walking, or just noise (no walking)
8. if the autocorrelation peak is valid, calculate the number of steps based on the calculated 
frequency of autocorrelation

The entry point of the algorithm is the count_steps function in count_steps.c. The data format
expected is a pointer to a buffer of int8_t accelerometer data (i.e, -128 to +127) stored in
sample order (i.e., [X1,Y1,Z1,X2,Y2,Z2,...Xn,Yn,Zn]). As a reference, a data file is attached
with sample accelerometer data. 

There are several #define parameters defined in count_steps.c and count_steps.h that should
be adjusted based on your data. SAMPLING_RATE is the sampling rate of the accelerometer data.
WINDOW_LENGTH is the length of the window of data that is analyzed by the algorithm, which
is optimally 4 seconds long. NUM_TUPLES is the number of triplets of accelerometer data, so
to get a WINDOW_LENGTH of 4 seconds with a SAMPLING_RATE of 20Hz, NUM_TUPLES should be set to
80. The only other value to adjust is NUM_AUTOCORR_LAGS, which determines the maximum frequency
that can be calculated. It should be set such that it corresponds to roughly 0.4Hz.




