/* Dmitriy Bernsteyn         */
/* Suzanne Curry             */ 
/* Robert Pollock            */
/* Matthew Weingarten        */ 
/* Senior Project            */ 
/* Summer 2002 - Fall 2015 */ 
/* Advisor: Dr. Schmalz      */ 

#include "img_head.h"

   /* global variables */
   int chosen_arithmetic;      /* the chosen arithmetic routine */
   int gray_max;
   char *fftOut = NULL;
   char *inpImg = NULL;

/***************************************************************************** 
 **                                  main                                   ** 
 *****************************************************************************/ 
   int main (int argc,char *argv[]) 
   { 
      int status;                 /* used in waitpid */
      int cid, i, j, k, l;        /* child id, loop variable, rows, colums */
      int img_needed = 2;         /* determine if 1 or 2 images are needed */  
      double *img_array1 = NULL;  /* img array for input image 1 */
      double *img_array2 = NULL;  /* img array for input image 2 */
      double *img_array3 = NULL;  /* img array for output image */
      int rows1, cols1;           /* # of rows and columns for img_array 1 */
      int rows2, cols2;           /* # of rows and columns for img_array 2 */
      int rows3, cols3;           /* # of rows and columns for img_array 3 */
      double max, min, range;     /* max, min, and range for the images */
      double *rmsetable = NULL;   /* table that stores rmsegrid results */
      int threshold;              /* threash hold value */
      int n_comp1, n_comp2;       /* number of patches for ctruth */
	//  string output				  /*name of output text file for dwt*/ 
   
    /* check to see if correct number of arguments, 1st check if there are arguments */ 
     if (argc <= 1)
     {
        printf("no arguments provided\n");
        help_menu();
     }
     else if (strcmp(ARGV_ARITHMETIC, "help") == 0)
	 {
         help_menu();
	 }
     /* check to see if first argument is valid and assign correct routine */ 
     else if (strcmp(ARGV_ARITHMETIC, "imgadd") == 0)
	 {
         chosen_arithmetic = ADD;
	 }
     else if (strcmp(ARGV_ARITHMETIC, "imgsub") == 0)
	 {
         chosen_arithmetic = SUBTRACT; 
	 }
	 else if (strcmp(ARGV_ARITHMETIC, "imgmul") == 0)
	 {	  
         chosen_arithmetic = MULTIPLY;
	 }
     else if (strcmp(ARGV_ARITHMETIC, "imgdiv") == 0)
	  {	  
         chosen_arithmetic = DIVIDE;
	  }		 
     else if (strcmp(ARGV_ARITHMETIC, "imgsin") == 0)
	  {
         chosen_arithmetic = SINE;
	  }
     else if (strcmp(ARGV_ARITHMETIC, "imglog") == 0)
	  {	  
         chosen_arithmetic = LOG;
	  }
     else if (strcmp(ARGV_ARITHMETIC, "rmse") == 0)
	  {
         chosen_arithmetic = RMSE;
	  }
     else if (strcmp(ARGV_ARITHMETIC, "rmsegrid") == 0)
	  {
         chosen_arithmetic = RMSEGRID;
	  }
     else if (strcmp(ARGV_ARITHMETIC, "rmsegrid01") == 0)
	  {
         chosen_arithmetic = RMSEGRID01;
	  }
     else if ((strcmp(ARGV_ARITHMETIC, "gcon") == 0) || (strcmp(ARGV_ARITHMETIC, "amax") == 0) || (strcmp(ARGV_ARITHMETIC, "amin") == 0) || (strcmp(ARGV_ARITHMETIC, "mmax") == 0) || (strcmp(ARGV_ARITHMETIC, "mmin") == 0))
     {   
			chosen_arithmetic = CON;
	  }
     else if (strcmp(ARGV_ARITHMETIC, "gray") == 0)
	 {
			chosen_arithmetic = GRAY;
	 }
     else if (strcmp(ARGV_ARITHMETIC, "thold") == 0)
	 {
			chosen_arithmetic = THOLD;
	 }
     else if (strcmp(ARGV_ARITHMETIC, "ftruth") == 0)
	 {
			chosen_arithmetic = FTRUTH;
	 }
     else if (strcmp(ARGV_ARITHMETIC, "ctruth") == 0)
	 {
			chosen_arithmetic = CTRUTH;
	 }
     else if (strcmp(ARGV_ARITHMETIC, "comp") == 0)
	 {
			chosen_arithmetic = COMP;
	 }
     else if (strcmp(ARGV_ARITHMETIC, "rmseann") == 0)
	 {
			chosen_arithmetic = RMSEANN;
	 }
     else if (strcmp(ARGV_ARITHMETIC, "fft") == 0)
	 {
			chosen_arithmetic = FFT;
	 }
     else if (strcmp(ARGV_ARITHMETIC, "fftspect") == 0)
	 {
			chosen_arithmetic = FFTDENS;
	 }
     else if (strcmp(ARGV_ARITHMETIC, "wht") == 0)
	 {
			chosen_arithmetic = WHT;
	 }
	 else if (strcmp(ARGV_ARITHMETIC, "whtrmse") == 0)
	 {
			chosen_arithmetic = WHTRMSE;
	 }
	 else if(strcmp(ARGV_ARITHMETIC, "dwt") == 0)
	 {
			chosen_arithmetic = DWT;
	 }
	 else if(strcmp(ARGV_ARITHMETIC, "dwt2") == 0)
	 {
			chosen_arithmetic = DWT2;
	 }
	 else if(strcmp(ARGV_ARITHMETIC, "idwt2") == 0)
	 {
			chosen_arithmetic = IDWT2;
	 }
     else //error
     {
         printf("%s is not a valid argument",ARGV_ARITHMETIC);
         help_menu();
     }
      /* check if correct number of arguments are provided */
     if (chosen_arithmetic == ADD || chosen_arithmetic == SUBTRACT || chosen_arithmetic == MULTIPLY || chosen_arithmetic == DIVIDE || chosen_arithmetic == THOLD)
     { 
         if (argc != NUM_ARGUMENTS) //error
         { 
            printf("Wrong number of arguments for %s\n",ARGV_ARITHMETIC);
            help_menu();
            exit(1); 
         }
         else if (chosen_arithmetic == THOLD)
         {
            img_needed = 1;
            sscanf(ARGV_THRESHOLD,"%i",&threshold);
         }
     }
     else if (chosen_arithmetic == SINE || chosen_arithmetic == LOG || chosen_arithmetic == FTRUTH || chosen_arithmetic == RMSE || chosen_arithmetic == GRAY || chosen_arithmetic == CTRUTH |chosen_arithmetic == COMP)
      {
         if (argc != (NUM_ARGUMENTS-1)) //error
         { 
            printf("Wrong number of arguments for %s\n",ARGV_ARITHMETIC);
            help_menu(); 
            exit(1); 
         } 
         if (chosen_arithmetic != RMSE && chosen_arithmetic != FTRUTH && chosen_arithmetic != CTRUTH)
		   {
            img_needed = 1;
		   }
      }
     else if ((chosen_arithmetic == RMSEGRID) || (chosen_arithmetic == RMSEGRID01))
     {
         if (argc != (NUM_ARGUMENTS + 1)) //error
         {
            printf("Wrong number of arguments for rmsegrid\n");
            help_menu();
         }
     }
     else if(chosen_arithmetic == RMSEANN || chosen_arithmetic == WHTRMSE)
     {
			if(argc != NUM_ARGUMENTS) //error
			{
				printf("Wrong number of arguments for rmseann.\n");
				help_menu();
		 	}
     }
     else if (chosen_arithmetic == CON)
     {
         img_needed = 1;
         if (setup_convulsion(argc,argv) != NOERROR) //error
         {
            printf("convulsion setup failed\n");
            help_menu();
            exit(1);
         }
     }
     else if((chosen_arithmetic == FFT) || (chosen_arithmetic == FFTDENS) || (chosen_arithmetic == WHT))
     {
	      if(argc == 4)
		   {
		   	fftOut = argv[3];
		   }
		  	else if(argc < 3 || argc > 4) //error
		  	{
				printf("Wrong number of arguments.\n");
				help_menu();
				exit(1);
		  	}
		  	inpImg = argv[2];
		  	img_needed = 1;
     }
	 else if(chosen_arithmetic == DWT)
	 {
			//logan owens
		 	//needs four arguments
			if(argc != 4) //error
			{
				printf("Wrong number of arguments for dwt.\n");
				help_menu();
		 	}		
			img_needed = 1;
	 }
	 else if(chosen_arithmetic == DWT2)
	 {
		 	//needs four arguments
			if(argc != 4) //error
			{
				printf("Wrong number of arguments for dwt2.\n");
				help_menu();
		 	}
			img_needed = 1;
			
	 }
	 else if(chosen_arithmetic == IDWT2)
	 {
		 	//needs four arguments
			if(argc != 4) //error
			{
				printf("Wrong number of arguments for dwt2.\n");
				help_menu();
		 	}
			img_needed = 1;
			
	 }
		 
	 
	  /*printf("%u\n", img_needed);*/
     /* checks if filenames are in correct format */
     if (check_images (img_needed,argv) == ERROR)
     {
         printf("Function check_images failed\n");
         exit(1);
     }
     
	 /* convert the image file to a pgm format, so it can be read in */
     convert_image(img_needed,argv);
	 
     /* read in images and store them to arrays for editing */ 
     img_array1 = read_image(&rows1, &cols1,"temp_in1.pgm");
	 
     /* set up dimensions for the output img_array3 */ 
     rows3 = rows1;
     cols3 = cols1;
     /* read in image array 2 only if needed  */ 
     if (img_needed == 2) 
     {
         /*printf("reading img 2\n");*/
         img_array2 = read_image(&rows2, &cols2, "temp_in2.pgm");
      	/* change img_array3 dimensions if img_array2's dimensions are smaller */
         if (rows2 < rows1)
		   {
            rows3 = rows2;
		   }
         if (cols2 < cols1)
		   {
            cols3 = cols2;
		   }
         /* change the image array to only area in common if one image areas are diffrent */
         if (((rows1 != rows2) || (cols1 != cols2)) && (chosen_arithmetic != WHTRMSE))
         {
            printf("%s and %s sizes are diffrent, only the area in common is going to be used\n", ARGV_IMAGE_ONE,ARGV_IMAGE_TWO);
            if ((rows1 != rows3) || (cols1 != cols3)) //row 2 or column 2 is smaller
            {
               resize_img (rows1, cols1, img_array1, rows3, cols3, img_array1);
            }  
            if ((rows2 != rows3) || (cols2 != cols3)) //row 1 or column 1 is smaller
            {
               resize_img (rows2, cols2, img_array2, rows3, cols3, img_array2);
            }
            rows1 = rows2 = rows3;
            cols2 = cols2 = cols3;
        }
     }
     printf("Image area used = (%i x %i), Gray: [0, %u]\n",rows3, cols3, gray_max);
     /* convert image based on threshhold value */
     if (chosen_arithmetic == THOLD)
     {
         img_array3 = conv_thold(rows3,cols3,img_array1,threshold);
      	/*
         for(i=0;i<rows3*cols3;i++)
         {
            if (img_array3[i] != 0 && img_array3[i] != 255)
               printf(" img_array3[%i] == %f",i,img_array3[i]);
         }
         */
         if (write_image(rows3, cols3, img_array3,"temp_out.pgm") != NOERROR)  //error writing image
         { 
            printf("Failed to write to temp_out.pgm\n");             
            exit(1); 
         }         
     }
     else if (chosen_arithmetic == COMP)
     {
         comp_setup();
         img_array3 = find_comp(rows3,cols3,img_array1);
         n_comp1 = renum_comp (rows3,cols3,img_array3);
         chng_bkgrnd(rows3,cols3,img_array3,(n_comp1+1));
         if (normalize(rows3, cols3, img_array3) != 0) //error
         { 
            printf("Failed to normalize image\n");
            exit(1);          
         } 
         if (write_image(rows3, cols3, img_array3,"temp_out.pgm") != 0) //error
         { 
            printf("Failed to write to temp_out.pgm\n");             
            exit(1); 
         }         	
     }
     else if (chosen_arithmetic == CTRUTH)
     {
         comp_setup();
         img_array1 = find_comp(rows3,cols3,img_array1);
         img_array2 = find_comp(rows3,cols3,img_array2);
         n_comp1 = renum_comp (rows3,cols3,img_array1);
         n_comp2 = renum_comp (rows3,cols3,img_array2);
         do_ctruth (rows3,cols3,img_array1,img_array2,n_comp1,n_comp2);
     }
     else if (chosen_arithmetic == FTRUTH)
     {
         printf("Image: %s ,  Truth: %s\n",ARGV_IMAGE_ONE,ARGV_IMAGE_TWO);
         if (ftruthcalc(rows3,cols3,img_array1,img_array2) != NOERROR) //error executing ftruth function
         {
            printf("ftruthcalc function failed\n");
            exit(1);
         }
     }
     /* turn an image into a gray scale image */ 
     else if (chosen_arithmetic == GRAY)
     {
         if (write_image(rows3, cols3, img_array1,"temp_out.pgm") != NOERROR) //error writing image 
         { 
            printf("Failed to write to temp_out.pgm\n");             
            exit(1); 
         }         	
     }
     /* perform geometric convultions and linear convultions */
     else if (chosen_arithmetic == CON)
     {
         img_array3 = convoltion(rows3,cols3,img_array1);
         if (normalize(rows3, cols3, img_array3) != 0) //error
         { 
            printf("Failed to normalize image\n");
            exit(1);          
         } 
         if (write_image(rows3, cols3, img_array3,"temp_out.pgm") != 0) //error
         { 
            printf("Failed to write to temp_out.pgm\n");             
            exit(1); 
         }         	
      }
      /* read in M & N for rmse, rmsegrid, and rmsegrid01 */
      else if ((chosen_arithmetic == RMSEGRID) || (chosen_arithmetic == RMSE) || (chosen_arithmetic == RMSEGRID01))
      {
         rmsesetup(argv, rows3, cols3);
         if (chosen_arithmetic == RMSE)
         {
            /* print RMSE for the 4 quadrents and the entire image*/
            rmsetable = rmse_grid(rows3, cols3, img_array1, img_array2);
            printf("Upper left rmse:   %.2f  Upper right rmse:  %.2f\n",rmsetable[0],rmsetable[1]);
            printf("Bottom left rmse:  %.2f  Bottom right rmse: %.2f\n",rmsetable[2],rmsetable[3]);
         }
         else if (chosen_arithmetic == RMSEGRID)
         {
            disp_table(rmsetable = rmse_grid(rows3, cols3, img_array1, img_array2));
         }  
         else //chosen arithmetic is RMSEGRID01
         {
            rmsetable = rmse_grid(rows3, cols3, img_array1, img_array2);
            rmse_norm(rmsetable, rows3, cols3, img_array1, img_array2);
            disp_table(rmsetable);
         }
         printf("Entire image's RMSE = %.2f\n\n",rmse(rows3, cols3, img_array1, img_array2));
      }
      else if(chosen_arithmetic == RMSEANN)
      {
         rmse_annu_setup(argv, rows3, cols3);      
         rmsetable = rmse_annu(rows3, cols3, img_array1, img_array2);
         dispRingTable(rmsetable, rows3, cols3);
      }
      else if(chosen_arithmetic == FFT)
      {
			img_fft(rows1, cols1, img_array1);
      }
      else if (chosen_arithmetic == WHT)
      {
      	img_wht(rows1, cols1, img_array1);
      }
      else if (chosen_arithmetic == WHTRMSE)
      {
		int blocks = atoi(argv[4]);
      	img_wht_rmse(rows1, cols1, img_array1, img_array2, blocks);
      }
      else if(chosen_arithmetic == FFTDENS)
      {
			img_fft_graphps(rows1, cols1, img_array1);
      }
      /* for SINE or LOG: one image is read in, then normalized, then taken the sin or log */ 
      /* after the sin and log functions is done on the normalized image, it is normalized to 0-255 */ 
      else if (chosen_arithmetic == SINE || chosen_arithmetic == LOG) 
      {
         img_array3 = malloc((rows3*cols3)*sizeof(double));
         if(img_array3 == (double*) NULL) //error allocating space
		 	{
		   	printf("Error allocating space.\n");
				exit(1);
		 	}
         /* find min, max and the range values */ 
         for (i = 0, min = MAXVALUE, max = MINVALUE; i < rows3*cols3; i++)
         { 
            if (img_array1[i] > max)
				{			
               max = img_array1[i];
				}			   
            if (img_array1[i] < min)
				{			
               min = img_array1[i];
				}
         }
         range = max - min; 
         /* new values normalized to pi/2 for sine, and 1-255 for the log */ 
         for (i = 0; i < rows3*cols3; i++) 
         { 
            img_array3[i] = (img_array1[i] - min)/range;
            if (chosen_arithmetic == SINE) 
            { 
            /* sin image can be normalized to 0-255 by multiplying by 255 */ 
               img_array3[i] = sin (img_array3[i] * (PI/2)) * MAXVALUE; 
            } 
            else  /* chosen_arithmetic == LOG */ 
            { 
               img_array3[i] = log10(img_array3[i] * MAXVALUE + 1); 
            } 
         } 
         /* sine is already normalized to 0-255, log still needs to be normalized to 0-255 */ 
         if (chosen_arithmetic == LOG) 
         { 
            normalize(rows3,cols3,img_array3);
         } 
         if (write_image(rows3, cols3, img_array3,"temp_out.pgm") != 0) //error writing image
         { 
            printf("Failed to write to temp_out.pgm\n");             
            exit(1); 
         }     
      } 
      /* for ADD or SUBTRACT, 2 images are combined using composite utility */ 
      else if (chosen_arithmetic == ADD || chosen_arithmetic == SUBTRACT)
      { 
         /* cid = child id number */ 
         cid = fork(); 
         /* parent process, do nothing */ 
         if (cid > 0)
         {
         
         }
         /* child process */ 
         else if (cid == 0) 
         { 
            if (chosen_arithmetic == ADD)
		      {
               int status = execl("/usr/bin/composite", "composite", "temp_in1.pgm", "temp_in2.pgm", "-compose", "plus", "temp_out.pgm", (char*)NULL);
			   	if(status == -1) //error executing
			   	{
						printf("Error executing.\n");
						exit(1);
			   	}
			   }
            else //chosen_arithmetic == subtract
			   {
               int status = execl("/usr/bin/composite", "composite", "temp_in1.pgm", "temp_in2.pgm", "-compose", "subtract", "temp_out.pgm", (char*)NULL);
			   	if(status == -1) //error executing
			   	{
						printf("Error executing.\n");
						exit(1);
			   	}
			   }
         }
		 
         else 
         { 
            printf("Second fork failed\n"); 
            int status = kill(KILLALL, SIGINT);
				if(status == -1) //error killing
				{
					printf("Error killing process.\n");
					exit(1);
				}
         } 
         /* wait for all processes to finish before continuing */ 
         if (waitpid(WAITANY, &status, NOOP) <= NOTOK) 
         { 
            printf("waitpid failed.\n"); 
            kill(KILLALL, SIGINT); 
				if(status == -1) //error killing
				{
					printf("Error killing process.\n");
					exit(1);
				}
         } 
      	/* store the .pgm file to the img_array3 the output array for RMSE */
         img_array3 = read_image(&rows3, &cols3, "temp_out.pgm");
      } 
      /* for MULTIPLE or DIVIDE: two images are read in, then the values obtained are mutiplied */ 
      /* the mutiplied values are then normalized and written into temp_in1.pgm file */ 
      else if (chosen_arithmetic == MULTIPLY || chosen_arithmetic == DIVIDE)
      {
         /* allocate memory for output img_array3 */
         img_array3 = malloc((rows3*cols3)*sizeof(double));
		 	if(img_array3 == (double*) NULL) //error allocating space
		 	{
				printf("Error allocating space.\n");
				exit(1);
		 	}
         for (i=0; i < rows3*cols3; i++) 
         { 
            if (chosen_arithmetic == MULTIPLY)
            {
               img_array3[i] = (img_array1[i] * img_array2[i]);
            }
            else /* chosen_arithmetic == DIVIDE */
            { 
               if (img_array2[i] != 0)
			   	{
                  img_array3[i] = (img_array1[i]  / img_array2[i]);
			   	}				  
               else /* img_array2 = 0, cannot divide by 0 */
			   	{
                  img_array3[i] = 0;
			   	}
            }
         } 
         if (normalize(rows3, cols3, img_array3) != 0) //error
         { 
            printf("Failed to normalize image\n");
            exit(1);          
         } 
         if (write_image(rows3, cols3, img_array3,"temp_out.pgm") != 0) //error
         { 
            printf("Failed to write to temp_out.pgm\n");             
            exit(1); 
         }    
      }
	  else if(chosen_arithmetic == DWT)
	  {
		  char* file = argv[3];
		  img_dwt(rows1, cols1, img_array1, file);
	  }
	  else if(chosen_arithmetic == DWT2)
	  {
		  double* dimensions;
		  
		  dimensions = img_dwt2(rows1, cols1, img_array1, 1, 0);
		  
		  rows3 = dimensions[0];
		  cols3 = dimensions[1];
		  printf("Output Image Area = (%d x %d)\n", rows3, cols3);
		  
		  img_array3 = (double*)calloc(rows3*cols3, sizeof(double));
		  img_array3 = img_dwt2(rows1, cols1, img_array1, 0, 0);
		  
		 if (normalize(rows3, cols3, img_array3) != 0) //error
         { 
            printf("Failed to normalize image\n");
            exit(1);          
         } 		 
         if (write_image(rows3, cols3, img_array3,"temp_out.pgm") != 0) //error
         { 
            printf("Failed to write to temp_out.pgm\n");             
            exit(1); 
         }
	  }
	  else if(chosen_arithmetic == IDWT2)
	  {
		  double* dimensions;
		  
		  dimensions = img_dwt2(rows1, cols1, img_array1, 1, 1);
		  
		  rows3 = dimensions[0];
		  cols3 = dimensions[1];
		  printf("Output Image Area = (%d x %d)\n", rows3, cols3);
		  
		  img_array3 = (double*)calloc(rows3*cols3, sizeof(double));
		  img_array3 = img_dwt2(rows1, cols1, img_array1, 0, 1);
		  
		 if (normalize(rows3, cols3, img_array3) != 0) //error
         { 
            printf("Failed to normalize image\n");
            exit(1);          
         } 		
         if (write_image(rows3, cols3, img_array3,"temp_out.pgm") != 0) //error
         { 
            printf("Failed to write to temp_out.pgm\n");             
            exit(1); 
         } 
	  }
	  
      /* If output image was used convert it back to specified format */
      if (chosen_arithmetic != DWT && chosen_arithmetic != RMSE && chosen_arithmetic != RMSEGRID && chosen_arithmetic != RMSEGRID01 && chosen_arithmetic != FTRUTH && chosen_arithmetic != CTRUTH && chosen_arithmetic != RMSEANN && chosen_arithmetic != FFT && chosen_arithmetic != FFTDENS && chosen_arithmetic != WHT && chosen_arithmetic != WHTRMSE)
      { 
         cid = fork();
         /* parent process, do nothing */ 
         if (cid > 0)
         {
         
         } 
         /* child process */ 
         else if (cid == 0) 
         { 
         /* if used only 1 image use the 2nd argument else use 3rd to name output file */ 
            if (img_needed == 1)
			   {
               int status = execl("/usr/bin/convert", "convert", "temp_out.pgm", ARGV_IMAGE_TWO, (char*)NULL);
			   	if(status == -1) //error executing
			   	{
						printf("Error executing.\n");
						exit(1);
					}
				}
            else
			   {
            	int status = execl("/usr/bin/convert", "convert", "temp_out.pgm", ARGV_IMAGE_THREE, (char*)NULL);
			   	if(status == -1) //error executing
			   	{
						printf("Error executing.\n");
						exit(1);
			   	}
			   }
         } 
         else //error
         { 
            printf("Third fork failed\n"); 
            int status = kill(KILLALL, SIGINT); 
				if(status == -1) //error killing
				{
					printf("Error killing process.\n");
					exit(1);
				}
         } 
         /* wait for all processes to finish before continuing on */ 
         if (waitpid(WAITANY, &status, NOOP) <= NOTOK) 
         { 
            printf("Waitpid failed.\n"); 
            int status = kill(KILLALL,SIGINT);
				if(status == -1) //error killing
				{
					printf("Error killing process.\n");
					exit(1);
				}			
         } 
      }
	  
      /* remove all temp files */ 
      status = remove("temp_in1.pgm");
	   if(status == -1) //error removing file
	   {
		 printf("Error removing file.\n");
		 exit(1);
	   }
      if ((img_needed != 1) && (chosen_arithmetic != RMSE) && (chosen_arithmetic != RMSEGRID) && (chosen_arithmetic != RMSEGRID01))
	   {
         status = remove("temp_in2.pgm");
		   if(status == -1) //error removing file
		   {
				printf("Error removing file.\n");
				exit(1);
		 	}
	  	}
      if (chosen_arithmetic != IDWT2 && chosen_arithmetic != DWT2 && chosen_arithmetic != DWT && chosen_arithmetic != RMSE && chosen_arithmetic != RMSEGRID && chosen_arithmetic != RMSEGRID01 && chosen_arithmetic != FTRUTH && chosen_arithmetic != CTRUTH && chosen_arithmetic != RMSEANN && chosen_arithmetic != FFT && chosen_arithmetic != FFTDENS && chosen_arithmetic != WHT && chosen_arithmetic != WHTRMSE)
	   {
         status = remove("temp_out.pgm");
		 	if(status == -1) //error removing file
			{
				printf("Error removing file.\n");
				exit(1);
		 	}
	   }
   	/* free allocated memory */
      if (img_array1 != NULL && chosen_arithmetic != WHTRMSE)
	  	{
         free(img_array1);
	  	}
      if (img_array2 != NULL && chosen_arithmetic != WHTRMSE)
	  	{
         free(img_array2);
	  	}
      if (img_array3 != NULL && chosen_arithmetic != DWT2)
	  	{
         free(img_array3);
	  	}
      if  (rmsetable != NULL)
	  	{
         free(rmsetable);
	  	}
   } 
   void resize_img (int old_rows, int old_cols, double *img_old, int new_rows, int new_cols, double *img_new)
   {
      int i,j,k,l;
      if (img_new != NULL)
      {
         free(img_new);
      }
      img_new = malloc((old_rows*old_cols)*sizeof(double));
	  	if(img_new == (double*) NULL) //error allocating space
	  	{
			printf("Error allocating space.\n");
			exit(1);
	  	}
      for(i=0;i<old_cols;i++)
      {
         if (i < new_rows)
         {
            if (i == 0)
			{
               k=0;
			}
            else
			{
               k++;
			}
            for(j=0;j<old_cols;j++)
            {
               if (j < new_cols)
               {
                  if (j == 0)
				  		{
                     l=0;
				  		}
                  else
				  		{
                     l++;
				  		}
                  img_new[k*new_cols+l] = img_old[i*old_cols+j];
               }
            }
         }
      }
   }
