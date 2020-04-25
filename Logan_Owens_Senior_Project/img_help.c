/* Dmitriy Bernsteyn         */
/* Suzanne Curry             */ 
/* Robert Pollock            */ 
/* Matthew Weingarten        */
/* Senior Project            */ 
/* Summer 2002 - Fall 2015 */ 
/* Advisor: Dr. Schmalz      */ 

#include "img_head.h"

   void help_menu ()
   {
      printf("\n                      Following opperations are availible:\n");
      printf("\n                         image manipulation routines:\n");
      printf("     1) img.exe imgadd    <img_in1> <img_in2> <img_out>\n");
      printf("     2) img.exe imgsub    <img_in1> <img_in2> <img_out>\n");
      printf("     3) img.exe imgmul    <img_in1> <img_in2> <img_out>\n");
      printf("     4) img.exe imgdiv    <img_in1> <img_in2> <img_out>\n");
      printf("     5) img.exe imgsin    <img_in1> <img_out>\n");
      printf("     6) img.exe imglog    <img_in1> <img_out>\n");
      printf("     7) img.exe gray      <img_in1> <img_out>\n");
      printf("     8) img.exe thold     <img_in1> <img_out> ThreshHoldValue\n");   
   	  printf("     9) img.exe comp      <img_in1> <img_out>\n");
   	  printf("    10) img.exe ftruth    <img_in1> <truth_img>\n");
      printf("    11) img.exe ctruth    <img_in1> <truth_img>\n");
      printf("    12) img.exe wht       <img_in>\n");
	  printf("    13) img.exe whtrmse   <img_in1> <img_in2>\n");
      printf("                <op> can be one of the following: gcon, amax, amin, mmax, or mmin\n");
      printf(" 14-18) img.exe <op>     <img_in1> <img_out> 3x3 {t0,t1,...,t9  integers}\n");
      printf("                                             5x5 {t0,t1,...,t25 integers}\n");
      printf("                                            file <template> Mrows Ncols TargetRow TargetCol\n");
      printf("\n                         image property routines:\n");
      printf("    19) img.exe rmse       <img_in1> <img_in2>\n");
      printf("    20) img.exe rmsegrid   <img_in1> <img_in2>  Mrows  Ncols\n");
      printf("    21) img.exe rmsegrid01 <img_in1> <img_in2>  Mrows  Ncols\n");
      printf("    22) img.exe rmseann    <img_in1> <img_in2>  Nrings\n");
      printf("    23) img.exe fft        <img_in> [outputFileRoot]\n");
      printf("    24) img.exe fftspect   <img_in> [outputFile]\n");
	  printf("\n                         discrete wavelet transform routines:\n");
	  printf("    25) img.exe dwt   <img_in> <txt_out>\n");
	  printf("    26) img.exe dwt2  <img_in> <img_out>\n");
	  printf("    27) img.exe idwt2 <img_in> <img_out>\n");
	  printf("\n                         miscellaneous routines:\n");
      printf("    28) img.exe help\n");
      printf("\n\n");
      exit(1);
   }


