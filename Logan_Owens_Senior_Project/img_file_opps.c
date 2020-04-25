/* Dmitriy Bernsteyn         */
/* Suzanne Curry             */ 
/* Robert Pollock            */ 
/* Matthew Weingarten        */
/* Senior Project            */ 
/* Summer 2002 - Fall 2015 */ 
/* Advisor: Dr. Schmalz      */ 

#include "img_head.h"

   extern int chosen_arithmetic;
   extern int gray_max;

   /* obtain and check string length of each image file name + file extention */ 
   int check_images (int img_needed, char **argv)
   { 	
      int length_one;              /* length of image one                       */
      int length_two;              /* length of image two                       */
      int length_three;            /* length of image three                     */
      if ((length_one = strlen(ARGV_IMAGE_ONE)) < MIN_LENGTH) //error 
      { 
         printf("Image one does not have proper name format with extension.\n"); 
         return(ERROR); 
      }
      else if (img_needed > 1 && ((length_two = strlen(ARGV_IMAGE_TWO)) < MIN_LENGTH)) //error
      { 
         printf("Image two does not have proper format with extension.\n"); 
         return(ERROR); 
      } 
      /* only check the third argument if 2 images are used */
      else if (img_needed == 2 && chosen_arithmetic != FTRUTH && chosen_arithmetic != RMSE  && chosen_arithmetic != RMSEGRID && chosen_arithmetic != RMSEGRID01 && chosen_arithmetic != RMSEANN && chosen_arithmetic != CTRUTH && chosen_arithmetic != WHTRMSE)
	   {
         if ((length_three = strlen(ARGV_IMAGE_THREE)) < MIN_LENGTH) //error
         {
            printf("Image three does not have proper format with extension.\n"); 
            return(ERROR); 
         }
	   } 
      return(NOERROR);
   }
   /* use convert command to convert all input images to .pgm format */    
   void convert_image (int img_needed, char **argv) 
   {  
      int i;
      int cid, status;
      for(i=0; i < img_needed; i++) 
      { 
         /* fork into new process, cid = child id number */          
         cid = fork(); 
         /* parent process will have the child process id number, which is not zero */ 
         /* if cid is not zero, then it is the parent process, do nothing. */ 
         if(cid != 0)
         {
         
         } 
         /* cid =0: child process; n = 0: the first child, process image 1*/ 
         else if ((cid == 0) && (i == 0))
		   {
            int status = execl("/usr/bin/convert", "convert", ARGV_IMAGE_ONE, "temp_in1.pgm", (char*)NULL);
				if(status == -1) //error executing
				{
					printf("Error executing.\n");
					return;
				}
		 	}
         /* cid = 0: child process; n = 1: the second child, process image 2*/ 
         else if ((cid == 0) && (i == 1) && (img_needed == 2))
		 	{
            int status = execl("/usr/bin/convert", "convert", ARGV_IMAGE_TWO, "temp_in2.pgm", (char*)NULL);
				if(status == -1) //error executing
				{
					printf("Error executing.\n");
					return;
				}
		 	}
         else  /* error checking */ 
         { 
            printf("first fork failed\n"); 
            int status = kill(KILLALL, SIGINT);
				if(status == -1) //error killing
				{
					printf("Error killing process.\n");
					exit(1);
				}
         } 
      } 
	  printf("Converting images to PGM format...\n");
     /* wait for all processes to finish before continuing on with the code */ 
     for (i=0; ((i < img_needed) && (cid > 0)); i++)
     { 
        if (waitpid(WAITANY, &status, NOOP) <= NOTOK) 
        { 
           printf("waitpid for input image failed\n"); 
           int status = kill(KILLALL, SIGINT);
			  if(status == -1) //error killing
			  {
				  printf("Error killing process.\n");
				  exit(1);
			  }
        } 
     }
	  printf("Finished conversion.\n");
    }

   /* stores the .pgm file into a pointer to an unsigned char array */
   double *read_image(int* rows, int* cols, char *filename)
   { 
      int binary;               /* states if file is binary or ASCII */
      int max;                  /* stores max array value */
      int i;                    /* loop variable */
      unsigned char *char_array; /* image array */
      double *img_array;
      char line[LINESIZE];      /* stores array the a line from .pgm file */
      FILE *fp;                 /* file pointer */
      /* check if the file can be opened */ 
      if ((fp = fopen(filename, "r")) == NULL) 
      { 
         printf("Can't open %s\n", filename); 
         exit(1); 
      }
      fgets(line, LINESIZE, fp); 
      if (strncmp(line, "P5", 2)) 
         binary = FALSE; 
      else if (strncmp(line, "P2", 2))
         binary = TRUE;
      else
      {
         printf("%s is not a pgm file\n",filename);
         exit(1);
      }
      //skip the comments and blank lines
      fgets(line, LINESIZE, fp); 
      while ((line[0] == '#') || (line[0] == '\0') || (line[0] == '\n'))
      {
         fgets(line, LINESIZE, fp);
      } 
      //get the # of rows and height 
      //printf("%u\n", (unsigned int) line[0]);
      sscanf(line, "%d %d", cols, rows);
      if((*rows == 0) || (*cols == 0)) //error
      {
		  printf("%s is invalid; rows and/or columns cannot be 0.\n", filename);
        exit(1);
      }
      //get the max value from file
      fgets(line, LINESIZE, fp);
      sscanf(line, "%d", &max);
      if(max > 255)
      {
		  printf("%s's max value is greater than 255.\n", filename);
          printf("These PGMs are currently unsupported.\n");
      }
      gray_max = max;
      //allocate room for array char is used to read in the array from file, 
      //img_array is used by program, to get better percision                   
      char_array = malloc(((*rows)*(*cols))*sizeof(unsigned char)); 
	   if(char_array == (unsigned char*) NULL) //error allocating space
	   {
			printf("Error allocating space.\n");
			exit(1);
	   }
      img_array  = malloc(((*rows)*(*cols))*sizeof(double)); 
	   if(img_array == (double*) NULL) //error allocating space
	   {
			printf("Error allocating space.\n");
			exit(1);
	   }
      //get all values from graymap, store the values in the array 
      if (binary == TRUE)
      {
         fread((void*)char_array, sizeof(unsigned char), (*rows)*(*cols), fp);
      } 
      else  //file is in ASCII and not binary
      {
         for (i = 0; i < (*rows)*(*cols); i++)
         { 
            fscanf(fp, "%hhu", &char_array[i]);
         }
      }
	   //converting from unsigned char to double
      for (i = 0; i < (*rows)*(*cols); i++) 
      { 
         img_array[i] = (double)(char_array[i]);
      } 
      //free char_array and close file once done with them
      free(char_array);
      int status = fclose(fp);
	   if(status != 0) //error closing file
	   {
	  	 printf("Error closing file.\n");
		 exit(1);
	   }
      //for(i=0;i<((*rows)*(*cols));i++)
         //printf("img[%i]=%i ",i,img_array[i]);
	   //printf("%u %u\n", *rows, *cols);
      return (img_array); 
   } 

   /* writes from an unsingned char array to a .pgm file */
   int write_image(int rows, int cols, const double *img_array, char *filename) 
   { 
      int max;                    /* max value for img_array             */
      int i;                      /* loop variable                       */
      char line[LINESIZE];        /* array to store the line read in     */
      unsigned char *char_array;  /* used to store the img_array to file */
      FILE *fp;                   /* file pointer to a .pgm file         */
      /* allocate room for character array */
      char_array = malloc((rows*cols)*sizeof(unsigned char));
	   if(char_array == (unsigned char*) NULL) //error allocating space
	  	{
			printf("Error allocating space.\n");
			return(ERROR);
	  	}
     /* converting from double to unsigned char */
      for (i = 0; i < (rows*cols); i++) 
      { 
         /*printf("write img_array[%i] = %f   ",i,img_array[i]);*/
         if ((img_array[i] < 0.0) || (img_array[i] >= 256.0))
         {
            printf("array value out of bounds: %f\n", img_array[i]);
            return(ERROR);
         }
         else
            char_array[i] = (unsigned char) floor(img_array[i]);
         /*printf("char_Array = %i\n",char_array[i]);*/
      } 
      /* write to file and replace old values */ 
      if ((fp = fopen(filename, "w")) == NULL)
      { 
         printf("Can't write to file\n"); 
         return(ERROR); 
      } 
      /* indicates .pgm file is an ASCII file */
      fprintf(fp, "P5\n"); 
      /* skip comment lines */ 
      fgets(line, LINESIZE, fp); 
      while (line[0] == '#') 
	   {
         fgets(line, LINESIZE, fp); 
	   }	 
      /* print to file the rows and height */ 
      fprintf(fp, "%d %d \n",cols, rows); 
      /* obtain new max value */ 
      max = (int)char_array[0]; 
      for (i = 1; i < (rows*cols); i++)
	   {	  
         if ((int)char_array[i] > max)
		   {
            max = (int)char_array[i];
		   }
	   }		 
      /* print to file the new max */ 
      fprintf(fp, "%d \n", max); 
      /* write new values over the old */ 
      fwrite((void*)char_array, sizeof(unsigned char), rows*cols, fp);
      /* free char_array */
      free(char_array);
      int status = fclose(fp);
	   if(status != 0) //error closing file
	   {
			printf("Error closing file.\n");
			exit(1);
	   }
      return(NOERROR); 
   }
