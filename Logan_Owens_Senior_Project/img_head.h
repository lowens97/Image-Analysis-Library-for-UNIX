/* Dmitriy Bernsteyn         */
/* Suzanne Curry             */
/* Robert Pollock            */
/* Craig Holmquist           */
/* Matthew Weingarten        */
/* Senior Project            */
/* Summer 2002 - Fall 2015   */
/* Advisor: Dr. Schmalz      */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "wavelib/wavelib.h"

#define NUM_ARGUMENTS                 5          /* number of arguments                */ 
#define NUM_ARITHMETIC                6          /* number of arithmetic routines      */ 
#define ARGV_ARITHMETIC               argv[1]    /* arithmetic routine                 */ 
#define ARGV_IMAGE_ONE                argv[2]    /* image one argument                 */ 
#define ARGV_IMAGE_TWO                argv[3]    /* image two argument                 */ 
#define ARGV_IMAGE_THREE              argv[4]    /* image three argument               */
#define ARGV_M                        argv[4]    /* M Argument for rmsegrid            */
#define ARGV_N                        argv[5]    /* N Argument for rmsegrid            */
#define ARGV_RING                     argv[4]    /* R Argument for rmseann*/
#define ARGV_CONV_OPTION              argv[4]    /* contains 3x3 5x5 or file options   */
#define ARGV_FILE                     argv[5]    /* template file                      */
#define ARGV_FILE_M                   argv[6]    /* argument for file rows             */
#define ARGV_FILE_N                   argv[7]    /* argument for file colums           */
#define ARGV_TARGET_K                 argv[8]    /* argument for target k row          */
#define ARGV_TARGET_L                 argv[9]    /* argument for target l column       */
#define ARGV_THRESHOLD                argv[4]    /* threashold argument                */
#define MIN_LENGTH                    5          /* minimum length of a image filename */ 
#define ERROR                         -1         /* return value if error              */
#define NOERROR                       0          /* return value if no errors occured  */ 
#define NOTOK                         0          /* some syscalls fail if ret <= 0     */ 
#define TRUE                          1          /* true value                         */ 
#define FALSE                         0    		 /* false value                        */
#define KILLALL                       0          /* to kill all processes              */ 
#define WAITANY                       0          /* for wait to wait for any child     */ 
#define NOOP                          0          /* no options for waitpid             */
#define MAXVALUE                      255.0      /* Max value for pgm grey scale       */
#define MINVALUE                      0.0        /* Min value for pgm grey scale       */
#define WHITE                         255.0      /* White value for pgm grey scale     */
#define BLACK                         0.0        /* Black value for pgm grey scale     */
#define LINESIZE                      256        /* size of line to be read in         */	
#define NOTUSED                       -1
#define USED                          0
#define PI                            3.1415926  /*    pie value                       */
#define PI2                           (PI * 2)
#define LNOF2                         0.6931471
#define ADD                           1          /*   * * * * * * * * * * * * * * *    */
#define SUBTRACT                      2          /*   *                           *    */
#define MULTIPLY                      3          /*   *                           *    */
#define DIVIDE                        4          /*   *                           *    */
#define SINE                          5          /*   *        Ineger Values      *    */
#define LOG                           6          /*   *       for oppertaions     *    */
#define RMSE                          19         /*   *                           *    */
#define RMSEGRID                      20         /*   *                           *    */
#define RMSEGRID01                    21         /*   *                           *    */
#define CON                           14         /*   *                           *    */
#define GCON                          1          /*   *                           *    */
#define AMAX                          2          /*   *                           *    */
#define AMIN                          3          /*   *                           *    */
#define MMAX                          4          /*   *                           *    */
#define MMIN                          5          /*   *                           *    */
#define GRAY                          7          /*   *                           *    */
#define THOLD                         8          /*   *                           *    */
#define FTRUTH                        10         /*   *                           *    */
#define CTRUTH                        11         /*   *                           *    */
#define COMP                          9          /*   *                           *    */
#define RMSEANN			              22         /*   *                           *    */
#define FFT                           23         /*   *                           *    */
#define FFTDENS                       24         /*   *                           *    */
#define WHT                           12         /*   *                           *    */
#define WHTRMSE						  13         /*   *                           *    */
#define DWT							  25         /*   *                           *    */
#define DWT2						  26         /*   *                           *    */
#define IDWT2						  27         /*   * * * * * * * * * * * * * * *    */
#define LN2(x) (log(x) / LNOF2)
#define min(x, y) (x < y ? x : y)

/* structure prototypes */
   typedef struct linked_list
   {
      struct linked_list *next;
      double val;
      int occur;
   } linkedlist;
   /* function prototypes                */
   /* is implemented in img_main.c */
   int main (int argc,char *argv[]);
   void resize_img(int old_rows, int old_cols, double *img_old, int new_rows, int new_cols, double *img_new);
   /* is implemented in img_manip.c      */
   void chng_bkgrnd(int rows, int cols, double *img_array, int new_color);
   void comp_setup();
   double* conv_thold(int rows, int cols, double *img_array, int threshold);
   double* convoltion(int rows, int cols, double *img_array);
   int do_ftruth(int rows, int cols, double *img_array, double *truth_img);
   int ftruthcalc (int rows, int cols, double *img_array, double *truth_img);
   double* find_comp(int rows, int cols, double *img_array);
   int normalize(int rows, int cols, double *img_array);
   int renum_comp(int rows, int cols, double *img_array);
   int do_ctruth(int rows, int cols, double *img_array, double *truth_img, int n_comp1, int n_comp2);
   int setup_convulsion(int argc, char** argv);
   /* are implemented in img_file_opps.c */
   int check_images(int img_needed, char **argv);
   void convert_image(int img_needed, char **argv);
   double* read_image(int *rows, int *cols, char *filename); 
   int write_image(int rows, int cols, const double *img_array, char *filename); 
	/* are implemented in img_rmse.c */
   void disp_table(double * rmsetable);
   double rmse(int rows, int cols, double *img_array1, double *img_array2);
   double* rmse_grid(int rows, int cols, double *img_array1, double *img_array2);
   int rmse_norm(double* rmse_array,int rows, int cols, double *img_array1, double *img_array2);
   void rmsesetup(char **argv, int rows3, int cols3);
	/* are implemented in img_rmse_annu.c */
	void avgsum_annu(int rows, int cols, double *img_array, double **avg, double **sum);
	int correctRing(double dist, double *ringRadii, int rings);
	void dispRingTable(double *rmsetable, int rows3, int cols3);
	double distance(double x1, double y1);
	double *rmse_annu(int rows, int cols, double *img_array1, double *img_array2);
	void rmse_annu_setup(char **argv, int rows3, int cols3);
	double *rmse_ringRadii(int rings, int rows, int cols);
	/* implemented in img_fft.c */
	void centrateImage(int rows, int cols, double *img);
	void displayFFTImage(double *d, int rows, int cols);
	void flipImage(int rows, int cols, double **row);
	void graphDensity(double *mag, int rings);
	void img_fft(int rows, int cols, double *img);
	void img_fft_graphps(int rows, int cols, double *img);
	void normalizeImageArray(double *d, int n);
	void transposeImage(int rows, int cols, double *in, double *out);
	void zeroDouble(double *d, int n);
	/* implemented in img_wht.c */
	void displayWHTImage(double *d, int rows, int cols);
	double** hadamard (int n);
	void hadamardTransform(double* data, int n);
	void img_wht(int rows, int cols, double *img);
	void img_wht_rmse(int rows, int cols, double *img, double *img1, int blocks);
	double* multiply(double* data, double** hadamard1, int n);
	void realToDouble2(double *d, int n, double* ret, int retN);
	void rmseCalculations(double* error, int rows, int cols);
	/* is implemented in img_help.c */
    void help_menu (void);
	void img_dwt(int rows, int cols, double *img_array, char *file);
	double *img_dwt2(int rows, int cols, double *img_array, int rDimensions, int idwt);
