/* Dmitriy Bernsteyn         */
/* Suzanne Curry             */ 
/* Robert Pollock            */
/* Matthew Weingarten        */    
/* Senior Project            */ 
/* Summer 2002 - Fall 2015   */ 
/* Advisor: Dr. Schmalz      */ 

#include "img_head.h"


   /* local global variables */
   int grows, gcols;
   int targetk,targetl; /* target (i,j) value for template target pixel */
   int *template;
   int ident=0; /* identity value if template is out of bounds   */
   int type;

   /* normalizes the img array to [0,255] */
   int normalize(int rows, int cols, double* img_array) 
   { 
      double max, min, range;  /* max, min and range values for img_array */
      int i;                   /* loop variable                           */
      double d;                /* temp variable used to normalize         */
      /* find min, max and the range values */ 
      for (i = 0,min = img_array[0],max = img_array[0]; i < (rows*cols); i++)
      { 
         if (img_array[i] > max)
		 	{		 
            max = img_array[i];
		 	}			
         if (img_array[i] < min)
		 	{		 
            min = img_array[i];
		 	}
      }
      range = max - min;
      /*printf("range %f = max %f - min %f\n",range,max,min);*/
      /* new values normalized to [0, 255] */ 
      for (i = 0; i < (rows*cols); i++) 
      {
         /*printf("img_array[%i] = %f     ",i,img_array[i]);*/
         if (range != 0)
		 	{		 
            img_array[i] = (((img_array[i] - min)/range) * MAXVALUE);
		 	}
         else if (img_array[i] > MAXVALUE)
		 	{
            img_array[i] = MAXVALUE;
		 	}
         else if (img_array[i] < MINVALUE)
		 	{
            img_array[i] = 0.0;
		 	}
         /*printf("norm img_array[%i] = %f uc = %i \n",i,img_array[i],(unsigned char)img_array[i]);*/
      }
      return(NOERROR); 
   }

   /* check arguments and set up options for geometric and linear convlusion */
   int setup_convulsion(int argc, char** argv)
   {
      int i;     /* loop variable                       */
      FILE *fp;  /* file pointer to file w the tamplate */
      if (strcmp(ARGV_ARITHMETIC, "gcon") == 0)
	  	{
         type = GCON;
	  	}
      if (strcmp(ARGV_ARITHMETIC, "amax") == 0)
	  	{
         type = AMAX;
	  	}
      if (strcmp(ARGV_ARITHMETIC, "amin") == 0)
	  	{
         type = AMIN;
	  	}
      if (strcmp(ARGV_ARITHMETIC, "mmax") == 0)
	  	{
         type = MMAX;
	  	}
      if (strcmp(ARGV_ARITHMETIC, "mmin") == 0)
	  	{
         type = MMIN;
	  	}
      /* check wich template option gcon has */
      if (argc > (NUM_ARGUMENTS-1))
      {
         if (strcmp(ARGV_CONV_OPTION, "3x3") == 0)
         {
            /* set up gcon template parameters */
            grows = gcols = 3;
            targetk = targetl = 1;
            if (argc != ((grows * gcols) + NUM_ARGUMENTS)) //error with arguments
            {
               printf("Wrong number of arguments for 3x3 option\n");
               return(ERROR);
            }
            else
            {
               /* read in the template */
               template = malloc((grows * gcols)*sizeof(int));
			   	if(template == (int*) NULL) //error allocating space
			   	{
						printf("Error allocating space.\n");
						return(ERROR);
			   	}
               for(i=0;i<(grows * gcols);i++)
			   	{
                  sscanf(argv[i+NUM_ARGUMENTS],"%i",&template[i]);
			   	}
            }
         }
         else if (strcmp(ARGV_CONV_OPTION, "5x5") == 0)
         {
            /* set up parameters for 5x5 template */
            grows = gcols = 5;
            targetk = targetl = 2;
            if (argc != ((grows * gcols) + NUM_ARGUMENTS)) //error
            {
               printf("Wrong number of arguments for 5x5 option\n");
               return(ERROR);
            }
            else
            {
               /* read in values into 5x5 template */
               template = malloc((grows * gcols)*sizeof(int));
			   	if(template == (int*) NULL) //error allocating space
			   	{
						printf("Error allocating space.\n");
						return(ERROR);
			  		}
               for(i=0;i<(grows * gcols);i++)
			   	{
                  sscanf(argv[i+NUM_ARGUMENTS],"%i",&template[i]);
			   	}
            }	
         }
         else if (strcmp(ARGV_CONV_OPTION, "file") == 0)
         {
            if (argc != (NUM_ARGUMENTS + 5)) //error
            {
               printf("Wrong number of arguments for gcon file option\n");
               return(ERROR);
            }
            else  /* correct number of arguments */
            {
               if ((fp = fopen(ARGV_FILE, "r")) == NULL) //error reading
               { 
                  printf("Can't open %s\n", ARGV_FILE); 
                  return(ERROR);
               } 
               else
               {
                  sscanf(ARGV_FILE_M,"%i",&grows);
                  sscanf(ARGV_FILE_N,"%i",&gcols);
                  sscanf(ARGV_TARGET_K,"%i",&targetk);
                  sscanf(ARGV_TARGET_L,"%i",&targetl);
                  /* verify if input arguments are correct, if not correct them */
                  if (grows < 0)
				  		{
                     printf("Number of rows cannot be negative, will be set to %i\n",grows=0);
				  		} 
                  if (gcols < 0)
				  		{
                     printf("Number of columns cannot be negative, will be set to %i\n",grows=0);
				  		}
                  if (targetk < 0)
				  		{
                     printf("Target k is less then 0, it will be set to %i\n",targetk=0);
				  		}
                  else if (targetk > grows)
				  		{
                     printf("Target k is greater then %i, it will be set to %i\n",grows,targetk=grows);
				  		}
                  if (targetl < 0)
				  		{
                     printf("Target l is less then 0, it will be set to %i\n",targetl=0);
				  		}
                  else if (targetl > gcols)
				  		{
                     printf("Target l is greater then %i, it will be set to %i\n",gcols,targetk=gcols);
				  		}
                  /* read in values into file template */
                  template = malloc((grows * gcols)*sizeof(int));
				  		if(template == (int*) NULL) //error allocating space
				  		{
							printf("Error allocating space.\n");
							return;
			      	}
                  for(i=0;i<(grows * gcols);i++)
                  {
                     /* insert EOF statement here */
                     fscanf(fp,"%i",&template[i]);
                  }
               }
            }
         }
         else
         {
            printf("Invalid option for gcon only 3x3, 5x5, and file options are valid\n");
            return(ERROR);
         }
      }
      else
      {
         printf("Wrong number of arguments for gcon\n");
         return(ERROR);
      }
      return(NOERROR);
   }

   /* this function can perform gcon and amax convultions depending on the type specified */
   double* convoltion (int rows, int cols, double *img_array)
   {
      int i, j, k, l;               /* loop variables                        */
      double value,final_value=0;   /* used to store values into new_array   */
      double tvalue,avalue;         /* template value, img_array value       */
      double *new_array;            /* output image array stores the result  */
      /* allocate memory for the new array */
      new_array = malloc((rows*cols)*sizeof(double));
      if(new_array == (double*) NULL) //error allocating space
	  	{
			printf("Error allocating space.\n");
			return;
	  	}
      for (i=0;i<rows;i++)
      {
         for(j=0;j<cols;j++)
         {
            for (k=0;k<grows;k++)
            {
               for(l=0;l<gcols;l++)
               {
                  tvalue = (double)template[k*gcols+l]; /* in bounds get value from template */
                  /* check to see if template value is in bounds when layed over image pixel */
                  if  ((((i-targetk) < 0) && (k < targetk)) || (((i+grows-targetk) > rows) && (k > targetk)) || (((j-targetl) < 0) && (l < targetl)) || (((j+gcols-targetl) > cols) && (l > targetl)))
                  {
                     avalue = ident; /* out of bounds set value to identity */
                  }
                  else
                  {
                     avalue = img_array[(i-targetk+k)*cols+(j-targetl+l)]; /* in bounds get value from image */
                  }
               	/*printf("(i,j)=(%i,%i) (k,l)=(%i,%i) sum(%.0f) + img[%i](%.0f) * tval[%i](%.0f) ",i,j,k,l,sum,
               (i-targetk+k)*cols+(j-targetl+l),img_array[(i-targetk+k)*cols+(j-targetl+l)],k*gcols+l,tvalue);*/
               	/* compute the sum by multiplaying array value and template value */
                  if (type == GCON)
                  {
                     if ((k==0) && (l==0))
					 		{
                        final_value = avalue * tvalue;
					 		}
                     else
					 		{					 
                        final_value = final_value + avalue * tvalue;
					 		}
                  }
                  else if (type == AMAX)
                  {
                     if ((k==0) && (l==0))
					 		{
                        final_value = avalue + tvalue;
							}
                     else 
                     { 
                        value = avalue + tvalue;
                        if (final_value < value)
								{
                           final_value = value;
								}
                     }
                  }
                  else if (type == AMIN)
                  {
                     if ((k==0) && (l==0))
					 		{
                        final_value = avalue + tvalue;
					 		}
                     else 
                     { 
                        value = avalue + tvalue;
                        if (final_value > value)
								{
                           final_value = value;
								}
                     }
                  }
                  else if (type == MMAX)
                  {
                     if ((k==0) && (l==0))
					 		{
                        final_value = avalue * tvalue;
				     		}
                     else 
                     { 
                        value = avalue * tvalue;
                        if (final_value < value)
								{
                           final_value = value;
								}
                     }
                  }
                  else if (type == MMIN)
                  {
                     if ((k==0) && (l==0))
					 		{
                        final_value = avalue * tvalue;
					 		}
                     else 
                     { 
                        value = avalue * tvalue;
                        if (final_value > value)
								{
                           final_value = value;
								}
                     }
                  }
                  /*printf(" === %.0f\n",sum);*/
               }
            } /* end template loops */
         	/* store and reset the sum to output image array */
            new_array[i*cols+j] = final_value;
         }
      }  /* end image loops */
      /* if template values are all 0, set last pixel in image to 1 to prevent conversion utility error */
      if (type==GCON || type==MMAX || type==MMIN)
	  	{
         for(i=0;i<grows*gcols;i++)
         {
            if (template[i] != 0)
				{
               break;
				}
            else if (template[grows*gcols-1] == 0)
				{
               new_array[rows*cols-1] = 1;
				}
         }
	  	}
      return(new_array);
   }

   /* this function converts image to a monochrome image based on threshold value */
   double* conv_thold  (int rows, int cols, double *img_array, int threshold)
   {
      double *new_array;
      int i;
      /* allocate space for new_array */
      new_array = malloc((rows*cols)*sizeof(double));
      if(new_array == (double*) NULL) //error allocating space
	  	{
			printf("Error allocating space.\n");
			return;
	  	}
      for (i = 0; i < (rows*cols); i++) 
      {
         /* if img value is greater then threshold, set to white; else, set to black */
         if (img_array[i] > threshold)
		 	{
            new_array[i] = WHITE;
		 	}
         else
		 	{
            new_array[i] = BLACK;
		 	}	
      }
      /* prevents an all 0 array */
      if (threshold > MAXVALUE)
	  	{
         new_array[(rows*cols - 1)] = WHITE;
	  	}
      return(new_array); 
   }

   /* this function builds a truth table for 2 monochrome images */
   int ftruthcalc (int rows, int cols, double *img_array, double *truth_img)
   {
      int i;               /* loop variable                */
      int Ncorrdetect=0;   /* number of correct detections */
      int Nmissdetect=0;   /* number of miss detections    */
      int Ncorrmiss=0;     /* number of correct misses     */
      int Nfalsealarm=0;   /* number of false allarms      */
      int error_msg1 = TRUE;
      int error_msg2 = TRUE;
      for (i=0; i<(rows*cols); i++) 
      {
         /* if img_array is grayscale and not monochrome */
         if (!(img_array[i]==BLACK || img_array[i]==WHITE))
         {
            if (i != 0 && error_msg1 == TRUE)
            {
               printf("Some of the values in img_array are not 0 or 255\n");
               error_msg1 = FALSE;
            }
            if (img_array[i] < 128)
				{
               img_array[i] = MINVALUE;
				}
            else
				{
               img_array[i] = MAXVALUE;
				}
         }
          /* if truth_img is grayscale and not monochrome */
         if (!(truth_img[i]==BLACK || truth_img[i]==WHITE))
         {
            if (i != 0 && error_msg2 == TRUE)
            {
               printf("Some of the values in truth_img are not 0 or 255\n");
               error_msg2 = FALSE;
            }
            if (truth_img[i] < 128)
				{
               truth_img[i] = MINVALUE;
				}
            else
				{
               truth_img[i] = MAXVALUE;
				}
            /*if (i!= 0)
               return(ERROR);*/
         }
         if (img_array[i] == WHITE && truth_img[i] == WHITE)
		 	{		 
            Ncorrdetect ++;
		 	}
         else if (img_array[i] == BLACK && truth_img[i] == WHITE)
		 	{
            Nmissdetect ++;
		 	}
         else if (img_array[i] == BLACK && truth_img[i] == BLACK)
		 	{
            Ncorrmiss ++;
		 	}
         else if (img_array[i] == WHITE && truth_img[i] == BLACK)
		 	{
            Nfalsealarm ++;
		 	}
      }
      printf("\n\n                 TRUTH TABLE\n\n");
      printf("Ncorrdetect  = %6i    Fcorrdetect  = %f\n",Ncorrdetect,(double)Ncorrdetect/(rows*cols));
      printf("Nfalsealarm  = %6i    Ffalsealarm  = %f\n",Nfalsealarm,(double)Nfalsealarm/(rows*cols));
      printf("Ncorrmiss    = %6i    Fcorrmiss    = %f\n",Ncorrmiss,  (double)Ncorrmiss  /(rows*cols));
      printf("Nmissdetect  = %6i    Fmissdetect  = %f\n",Nmissdetect,(double)Nmissdetect/(rows*cols));
      printf("\nTotal Pixels = %6i   Total Detected = %f\n",Ncorrdetect+Nfalsealarm+Ncorrmiss+Nmissdetect, (double)(Ncorrdetect+Nfalsealarm+Ncorrmiss+Nmissdetect)/(rows*cols));
      return(NOERROR);
   }

   void comp_setup ()
   {
      /* set up variables for convolution */
      type = AMAX;
      targetk = targetl = 1;
      grows = gcols = 3;
      template = malloc((grows * gcols)*sizeof(int));
	  	if(template == (int*) NULL) //error allocating space
	  	{
			printf("Error allocating space.\n");
			return;
	  	}
      ident = BLACK;
      template[0] = -99999999, template[1] = 0, template[2] = -99999999;
      template[3] = 0,         template[4] = 0, template[5] = 0;
      template[6] = -99999999, template[7] = 0, template[8] = -99999999;
   }

   double* find_comp (int rows, int cols, double *truth_img)
   {
      double *new_img;
      double *temp_img;
      int i,j,k;
      int imgsnoteq = TRUE;
      int max_iterations = 5000;
      /* allocate room for image arrays */
      new_img  = malloc((rows*cols)*sizeof(double));
	  	if(new_img == (double*) NULL) //error allocating space
	  	{
			printf("Error allocating space.\n");
			return;
	  	}
      temp_img = malloc((rows*cols)*sizeof(double));
	  	if(temp_img == (double*) NULL) //error allocating space
	  	{
			printf("Error allocating space.\n");
			return;
	  	}
      for(i=0;i<rows;i++)
      {
         for(j=0;j<cols;j++)
         {
         /* Map which areas are used, and which once are not */
            if (truth_img[i*cols+j] >= 128)
				{
               truth_img[i*cols+j] = NOTUSED;
				}
            else
				{
               truth_img[i*cols+j] = USED;
				}
            /* set up c array, Black when outside of patches */
            if (truth_img[i*cols+j] == NOTUSED)
				{
               new_img[i*cols+j] = BLACK;
				}
            else
				{
               new_img[i*cols+j] = (double)(i*cols+j+1);
				}			   
            temp_img[i*cols+j] = BLACK;
         }
      }
      /* until images are = perform amax convoltions */
      for (k=0;(imgsnoteq == TRUE);k++)
      {
         /* set temp_img = img c */
         for(i=0;i<rows;i++)
         {
            for(j=0;j<cols;j++)
            {
               temp_img[i*cols+j]=new_img[i*cols+j];
            }
         }
         /* perform amax */
         new_img =  convoltion (rows,cols,new_img);
         /* reset areas out side of boundaries to BLACK */
         for(i=0;i<rows;i++)
         {
            for(j=0;j<cols;j++)
            {
               if (truth_img[i*cols+j] == NOTUSED)
               {
                  new_img[i*cols+j] = BLACK;
               }
            }
         }
         for(i=0;i<rows;i++)
         {
            for(j=0;j<cols;j++)
            {
               /* reset the values if outside used area */
               if (truth_img[i*cols+j] == NOTUSED)
			   	{
                  new_img[i*cols+j] = BLACK;
			   	}
               /* check to see if same as before performing amax */
               if (truth_img[i*cols+j] == USED && temp_img[i*cols+j] == new_img[i*cols+j])
               {
                  imgsnoteq = FALSE;
               }
               /* if not the same break out of loop and repeat amax */
               else if (truth_img[i*cols+j] == USED && temp_img[i*cols+j] != new_img[i*cols+j])
               {
                  imgsnoteq = TRUE;
                  i=rows;
                  j=cols;
                  break;
               }
            }
         }
        /* set exit point if iteration become higher then max iterations */
         if (k == max_iterations)
		 	{
            break;
		 	}
      }
      /* free storage area for temo_img */
      free(temp_img);
      return(new_img);
   }

   int renum_comp (int rows, int cols, double *img_array)
   {
      int i,j;                         /* loop variables        */
      int num_values=0;                /* # of diffrent values  */
      double temp;                     /* temporary value       */
      linkedlist *list,*list2, *head;  /* linked list variables */
      /* initilalize head node for linked list */
      head = malloc(sizeof(linkedlist));
	  	if(head == (linkedlist*) NULL) //error allocating space
	  	{
			printf("Error allocating space.\n");
			return(ERROR);
	  	}
      head->next = NULL;
      head->val = -1;
      head->occur = 0;
      /* intitalize temp image */
      for(i=0,list=head;i<rows*cols;i++)
      {
         if (img_array[i] != 0)
         {
            for (list=head; list != NULL; list=list->next)
            {
               /* emptry space in array found */
               if (list->val == -1)
               {
                  num_values++;
                  /* store value and increment occurence */
                  list->val = img_array[i];
                  list->occur++;
               	  /* allocate and initialize next node */
                  list->next = malloc(sizeof(linkedlist));
				  		if(list->next == (linkedlist*) NULL) //error allocating space
				  		{
							printf("Error allocating space.\n");
							return(ERROR);
				  		}
                  list=list->next;
                  list->next = NULL;
                  list->val = -1;
                  list->occur = 0;
               }
               /* value already stored in linked list */
               else if (list->val == img_array[i])
               {
                  list->occur++;
                  break;
               }
            }
         }
      }
      /* using bubble sort, rearrange values in linked list numerically */
      for (list=head; list->next != NULL; list=list->next) 
      {
         for (list2=list; list2->next != NULL; list2=list2->next)
         {
            if (list2->val < list->val)
            {
               temp = list->val;         
               list->val = list2->val;
               list2->val = temp;
            }
         }
      }
      /* display the statistics about pixels in output image */ 
      for (list=head,i=1; list->next != NULL; list=list->next,i++)
      {
      	/* renumber all pixels of that value to 1 ... N */
         for(j=0;j<rows*cols;j++)
         {
            if (img_array[j] == list->val)
            {
               img_array[j] = (double)i;
            }
         }
      }
      return(num_values);
   }

   void chng_bkgrnd (int rows, int cols, double *img_array, int new_color)
   {
      int i;
      for(i=0;i<rows*cols;i++)
      {
         if (img_array[i] == BLACK)
         {
            img_array[i] = (double)new_color;
         } 
      }
   }
   
   int do_ctruth (int rows, int cols, double *img_array, double *truth_img, int n_patch1,int n_patch2)
   {
      int i, j;   /* loop variables */
      int num1, num2;
      int detections=0, falsealarms=n_patch1;
      double p_detections, p_falsealarms;
      for(num2=n_patch2;num2>0;num2--)
      {
         for(i=0;i<rows*cols;i++)
         {
            if (truth_img[i] == num2 && img_array[i] != 0)
            {
               detections++;
               break;
            } 
         }
      }
      for(num1=n_patch1;num1>0;num1--)
      {
         for(i=0;i<rows*cols;i++)
         {
            if (img_array[i] == num1 && truth_img[i] != 0)
            {
               falsealarms--;
               break;
            } 
         }
      }
      if (n_patch2 != 0)
	  	{
         p_detections  = detections /(double)n_patch2;
	  	}
      else
	  	{
         p_detections = (double)detections;
	  	}
      p_falsealarms = falsealarms/(double)(rows*cols);
      printf("\n\n       Ctruth Table\n\n");
      printf("Number of components in test image  = %i  \n",n_patch1);
      printf("Number of components in truth image = %i\n\n",n_patch2);
      printf("N_d = %i     Pd  = %.2f\n",detections,p_detections);
      printf("N_fa = %i    Rfa = %.8f\n",falsealarms,p_falsealarms);
   }
