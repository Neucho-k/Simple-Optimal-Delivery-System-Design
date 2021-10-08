/* Solution to comp20005 Assignment 1, 2021 semester 1.

   Authorship Declaration:

   (1) I certify that the program contained in this submission is completely
   my own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or payment,
   may not be submitted for assessment in this subject.  I understand that
   submitting for assessment work developed by or in collaboration with
   other students or non-students constitutes Academic Misconduct, and
   may be penalized by mark deductions, or by other penalties determined
   via the University of Melbourne Academic Honesty Policy, as described
   at https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded
   as Student General Misconduct (interfering with the teaching activities
   of the University and/or inciting others to commit Academic Misconduct).
   I understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions
   or sought benefit from such actions.

   Signed by: Lichuan Li, 1074497
   Dated:     27/4/2021

*/


#include<stdio.h>
#include<stdlib.h>
#include<math.h>

// ****************************************************************************

#define NO_HEADLN 1
#define MAX_COL 5
#define MAX_ROW 999
#define SPEED 17.30 
#define FUR_MM 5
#define NO_DRONES 26
#define MAX_RTS 24 * 60

typedef int intarr1d_t[MAX_ROW]; 
typedef int intarr2d_t[MAX_ROW][MAX_COL - 2]; 
typedef double dblarr1d_t[MAX_ROW]; 
typedef int intarr26x2_t[26][2]; 

int read_data(intarr2d_t dt1, dblarr1d_t dist, intarr1d_t lst_ttaken, 
	           intarr1d_t mmfm); 
int time_revhh(int mmmfm); 
int time_revmm(int mmmfm); 
int SEL_drone_stg2(intarr26x2_t dep_rts, int rfd, int rts);
int SEL_drone_stg3(intarr26x2_t dep_rts, int ttaken, int rfd, int num_drone); 
int sorting(intarr26x2_t dep_rts, int num_drone, int ttaken, int rfd);

void remove_hl(int n); 
void distance(double x, double y, int n_row, dblarr1d_t dist); 
void t_flight(dblarr1d_t dist, intarr1d_t lst_ttaken, int n_row); 
void stage_1(intarr2d_t dt1, int n_row, dblarr1d_t dist); 
void stage_2(intarr2d_t dt1, intarr1d_t lst_ttaken, intarr1d_t mmfm, 
	          char drone[NO_DRONES], int n_row); 
void time_trf(intarr1d_t mmfm, int hh, int mm, int n_row);
void stage_3(intarr2d_t dt1, intarr1d_t mmfm, intarr1d_t lst_ttaken,  
	          char drone[NO_DRONES], int n_row);


// ****************************************************************************
// The main funciton

int 
main (int argc, char *argv[]) {
    intarr1d_t lst_ttaken, mmfm;
    intarr2d_t od_n_t; 
    dblarr1d_t dist;
    int n_row;
    char drone[NO_DRONES] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                              'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                              'U', 'V', 'W', 'X', 'Y', 'Z'}; 

    // Removing the header line and reading data into arrays
    remove_hl(NO_HEADLN);
    n_row = read_data(od_n_t, dist, lst_ttaken, mmfm);

    // Doing Stages
    stage_1(od_n_t, n_row, dist);
    stage_2(od_n_t, lst_ttaken, mmfm, drone, n_row);
    stage_3(od_n_t, mmfm, lst_ttaken, drone, n_row);

    return 0; 
}


// ****************************************************************************
// Removing the headder line of the imported tsv file 

void
remove_hl(int n) {
    int c;

    while ((c=getchar()) != EOF) {
	    if (c == '\n') { 
            n--;
        }

        if (n == 0) { 
            return; 
            } 
    }

    // Something went wrong if it gets to here
    printf("Something went wrong, please check the input.");
    exit(EXIT_FAILURE);
}


// ****************************************************************************
// Reading the data from .tsv file and processing essential data into 
// arrays. (Processing data here saves time, as less repeated loop needed 
// to be ran for later read and malnipulate.) 

int
read_data(intarr2d_t od_n_t, dblarr1d_t dist, intarr1d_t lst_ttaken, 
	       intarr1d_t mmfm) {
    int v1, v2, v3, i, n_row;
    double v4, v5;

    i = 0; n_row =0;
    while (scanf("%d%d%d%lf%lf", &v1, &v2, &v3, &v4, &v5) == 5) {

        // Inserting the first three values from the .tsv file into the od_n_t
        // array respectively, correspoding to order, rfdhh, rfdmm. 
        *(od_n_t[i]) = v1;
        *(od_n_t[i] + 1) = v2;
        *(od_n_t[i] + 2) = v3;
        
        // Calculating the distance (Unit: km)
        distance(v4, v5, n_row, dist);

        // Calculating the time of taken for each order (Unit: minutes)
        t_flight(dist, lst_ttaken, n_row);
        
        // Combing the rfdhh and rfdmm as the total minutes from midnight 
        // (Unit: minutes)
        time_trf(mmfm, v2, v3, n_row);
        
        // Incrementation of number of times i and number of row n_row. 
        // Number of rows is also equivalent to the number of meals. 
        i++;
        n_row++;

    }
          
    return n_row;
}


// ****************************************************************************
// Defining function distance(), calculating the distance between the kitchen
// and the address. (Unit: km)

void
distance(double x, double y, int n_row, dblarr1d_t dist) {
    dist[n_row] = sqrt(pow(x,2) + pow(y,2));
}


// ****************************************************************************
// Defining function t_flight(), calculating the time taken for delivery
// from the kitchen to the address. (Unit: minutes)

void 
t_flight(dblarr1d_t dist, intarr1d_t lst_ttaken, int n_row) {
    lst_ttaken[n_row] = ceil(*(dist + n_row) / SPEED * 60);   
}


// ****************************************************************************
// Defining function time_trf, combing the rfdhh and rfdmm as the total 
// minutes from midnight. (Unit: minutes)

void 
time_trf(intarr1d_t mmfm, int hh, int mm, int n_row){
        mmfm[n_row] = hh * 60 + mm;
}


// ****************************************************************************
/* Defining function stage_1. the required stage 1 output:
    -- The first and last of the meals' details
     -- Order number
     -- Time of 'ready for delivery'
     -- Distance from the kitchen to the address
*/

void 
stage_1(intarr2d_t dt1, int n_row, dblarr1d_t dist) {
    int i;
    
    printf("S1, %d meals to be delivered\n", n_row);
    for (i = 0; i < n_row; i+=n_row - 1) {
        printf("S1, order %5d, rfd %02d:%02d, distance %2.1f km\n", 
                *(dt1[i] + 0), *(dt1[i] + 1), *(dt1[i] + 2), *(dist + i));
    }

    // Blank line
    printf("\n");
}


// ****************************************************************************
/* Defining function stage_2, the required stage 2 output:
    -- All the meals' details
     -- Order number
     -- Time of 'ready for delivery'
     -- Drone sent
     -- Drone departure time
     -- Delivery time to the customer
     -- Time of 'readty to service'
     -- Short summary of how many drones are sent
*/

void 
stage_2(intarr2d_t dt1, intarr1d_t lst_ttaken, intarr1d_t mmfm, 
	     char drone[NO_DRONES], int n_row) {
    int i, rfd, del, rts, nth_drone, num_drones, delhh, delmm, rtshh, rtsmm;
    int dep_rts[26][2] = {{0},{0}}; 
    
    num_drones = 0;

    for (i = 0; i < n_row; i++){
        rfd = *(mmfm + i); 
        del = rfd + *(lst_ttaken + i); 
        rts = del + *(lst_ttaken + i) + FUR_MM; 
        
        // Drone selection
        nth_drone = SEL_drone_stg2(dep_rts, rfd, rts);

        if (num_drones < nth_drone) {
            num_drones = nth_drone;
        }

        // Time reversing
        delhh = time_revhh(del);
        delmm = time_revmm(del);
        rtshh = time_revhh(rts);
        rtsmm = time_revmm(rts);
        
        printf("S2, order %d, rfd %02d:%02d, drone %c, dep %02d:%02d, "
        	"del %02d:%02d, rts %02d:%02d\n", *(dt1[i] + 0), *(dt1[i] + 1), 
            *(dt1[i] + 2), drone[nth_drone], *(dt1[i] + 01), *(dt1[i] + 02), 
            delhh, delmm, rtshh, rtsmm);
    }

    // Short summayr
    printf("S2, a total of %d drones are required\n\n", num_drones + 1);
}


// ****************************************************************************
// Defining function SEL_drone_stg2, inserting departure time and time of 
// 'ready to service' of selected drone into array dep_rts. And the function 
// returns an integer of nth drone which is selected. 

int
SEL_drone_stg2(intarr26x2_t dep_rts, int rfd, int rts) {
    int n;

    for (n = 0; n < NO_DRONES; n++){

        if (rfd >= *(dep_rts[n] + 1)) {
            *(dep_rts[n] + 0) = rfd;
            *(dep_rts[n] + 1) = rts;
            break;
        }
    }

    return n;
}

// ****************************************************************************
// Defining function time_revhh, reversing the total time from midnight
// to the hour number and hence return the number

int
time_revhh(int mmmfm) {
    int result;

    result = mmmfm / 60;

    return result;
}

// ****************************************************************************
// Defining function time_revmm, reversing the total time from midnight
// to the minute number and hence return the number

int
time_revmm(int mmmfm) {
    int result;

    result = mmmfm % 60;

    return result;
}


// ****************************************************************************
// Defining function stage_3

void 
stage_3(intarr2d_t dt1, intarr1d_t mmfm, intarr1d_t lst_ttaken, 
	     char drone[NO_DRONES], int n_row) {
    int n, rfd, ttaken, rts, nth_drone, num_drones, dephh, depmm, delhh, 
         delmm, rtshh, rtsmm;
    int dep_rts[26][2] = {{0},{0}};

    num_drones = 0;

    // The nth order to process
    for (n = 0; n < n_row; n++) {
        rfd = *(mmfm + n); 
        ttaken =  *(lst_ttaken + n); 
        
        // Drone selection 
        // Delivery times are more than 30 mins, drone is sent immediately
        if (ttaken > 30) {
            rts = rfd + 2 * ttaken + FUR_MM; 
            nth_drone = SEL_drone_stg2( dep_rts, rfd, rts);
        }
        // Delivery time is less than 30 mins
        else {
            nth_drone = SEL_drone_stg3(dep_rts, ttaken, rfd, num_drones);
        }

        // Count the number of drones used
        if (num_drones < nth_drone) {
            num_drones = nth_drone;
        }

        // Time reversing;
        dephh = time_revhh(*(dep_rts[nth_drone] + 0));
        depmm = time_revmm(*(dep_rts[nth_drone] + 0)); 

        delhh = time_revhh(*(dep_rts[nth_drone] + 0) + ttaken);
        delmm = time_revmm(*(dep_rts[nth_drone] + 0) + ttaken);
        
        rtshh = time_revhh(*(dep_rts[nth_drone] + 1));
        rtsmm = time_revmm(*(dep_rts[nth_drone] + 1));

        // Printing the ouputs
        printf("S3, order %d, rfd %02d:%02d, drone %c, dep %02d:%02d, "
        	"del %02d:%02d, rts %02d:%02d\n", *(dt1[n] + 0), *(dt1[n] + 1), 
            *(dt1[n] + 2), drone[nth_drone], dephh, depmm, delhh, delmm, 
            rtshh, rtsmm); 
    }

    // Printing the summary of stage 3
    printf("S3, a total of %d drones are required\n", num_drones + 1);
    printf("\nta daa!\n");
}


// ****************************************************************************
// Defining function SEL_drone_stg3, inserting departure time and time of 
// 'ready to service' of selected drone into array dep_rts. And the function 
// returns an integer of nth drone which is selected. 

int
SEL_drone_stg3(intarr26x2_t dep_rts, int ttaken, int rfd, int num_drone){
    int n;

    n = sorting(dep_rts, num_drone, ttaken, rfd);

    // Departure time equals to the rts if to wati a drone back
    if (rfd < *(dep_rts[n] + 1)) {
        *(dep_rts[n] + 0) = *(dep_rts[n] + 1);
    }
    // Departure time equals to the rfd if a new drone is sent immediatly 
    else {
        *(dep_rts[n] + 0) = rfd; 
    }
    // Inserting rts to the dep_rts array        
    *(dep_rts[n] + 1) = *(dep_rts[n] + 0) + 2 * ttaken + FUR_MM;

    return n;
}

// Sorting the rts and return the nth with smallest rts
int
sorting(intarr26x2_t dep_rts, int num_drone, int ttaken, int rfd) {
    int n, result, tmp, tie;

    result = -1;
    tmp = MAX_RTS;
    tie = 0;

    for (n = num_drone; n >= 0; n--){
        // Check if satify the thiry-minutes rule and select the drone which 
        // comes back early
        if(ttaken + *(dep_rts[n] + 1) <= rfd +30 && *(dep_rts[n] + 1) <= tmp ){
            result = n;
            tmp = *(dep_rts[n] + 1);

        }

        // If 'tie' condition is met, select the lowest-labelled drone
        if (*(dep_rts[n] + 1) <= rfd) {
            tie++;
        
            if (tie >= 2) {
                result = n;
                tmp = *(dep_rts[n] + 1);
            }
        }
    }

    // A new drone is sent
    if (result == -1){
        result = num_drone + 1;
    }

    return result; 
}