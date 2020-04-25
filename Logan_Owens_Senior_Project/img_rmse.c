/* Dmitriy Bernsteyn         */
/* Suzanne Curry             */ 
/* Robert Pollock            */
/* Matthew Weingarten        */ 
/* Senior Project            */ 
/* Summer 2002 - Fall 2015   */ 
/* Advisor: Dr. Schmalz      */ 

#include "img_head.h"

   /* global variables to be imported */
   extern int chosen_arithmetic;
	/* local global variables */
   int gridM, gridN;       /* i,j for blocks in image */
   int gridrow, gridcol;   /* i,j for pixels in each block */

   void rmsesetup(char **argv, int rows3, int cols3)
   {
      if ((chosen_arithmetic == RMSEGRID) || (chosen_arithmetic == RMSEGRID01))
      {
         sscanf(ARGV_M,"%i",&gridM);
         sscanf(ARGV_N,"%i",&gridN);
      }
      else if (chosen_arithmetic == RMSE)
      {
         gridM = 2;
         gridN = 2;
      }
      /* check if gridM and gridN are valid, if not correct them */
      if (gridM > rows3)
	  	{
         printf("Not enough pixels for M=%i, M is going to be %i\n",gridM,gridM=rows3);
	  	}
      if (gridN > cols3)
	  	{
         printf("Not enough pixels for N=%i, N is going to be %i\n",gridN,gridN=cols3);
	  	}
      if (gridM <= 0)
	  	{
         printf("M cannont be <=0, M is going to be set to %i\n",gridM=1);
	  	}
      if (gridN <= 0)
	  	{
         printf("N cannont be <=0, N is going to be set to %i\n",gridN=1);
	  	}
      /* set up actual pixels per block */
      gridrow = rows3 / gridM;
      gridcol = cols3 / gridN;
      if ((rows3%gridM) !=0)
	  	{
         gridrow++;
	 	}
      if ((cols3%gridN) !=0)
	  	{
         gridcol++;
	  	}
      printf("Image broken down into (%ix%i) blocks with (%ix%i) pixels per block\n",gridM,gridN,gridrow,gridcol);
   }
   
   /* this function figures out rmse for images that are passed in */
   double rmse (int rows, int cols, double *img_arrayA, double *img_arrayD)
   {
      int i; /* loop variables */
      double sum=0; /* used in rmse calculations */
      for (i=0;i<(rows*cols);i++)
      {
         sum = sum + ((img_arrayD[i] - img_arrayA[i]) * (img_arrayD[i] - img_arrayA[i]));
         /*printf(" imgD=%i imgA=%i ",img_arrayD[i],img_arrayA[i]);*/
      }
      return (sqrt(sum/(rows*cols)));
   }
   /* this function breaks the images into grids and passes them into rmse function *
   * it stores the results from the rmse function into an array and returns this array */
   double* rmse_grid (int rows, int cols, double *img_array1, double *img_array2)
   {
      int n,m,i, j, k=0,l=0,q=0; /* loop variables */
      double *block_array1, *block_array2; /* blocks that umages are broken into */  
      int temprow, tempcol; /* used when blocks are not filled */
      double *result; /* result rmse array */
      /* allocate memory for blocks and result */
      block_array1 = malloc(gridrow*gridcol*sizeof(double));
	   if(block_array1 == (double*) NULL) //error allocating space
	   {
			printf("Error allocating space.\n");
			return;
	  	}
      block_array2 = malloc(gridrow*gridcol*sizeof(double));
	  	if(block_array2 == (double*) NULL) //error allocating space
	  	{
			printf("Error allocating space.\n");
			return;
	  	}
      result = malloc(gridM*gridN*sizeof(double));
	  	if(result == (double*) NULL) //error allocating space
	  	{
			printf("Error allocating space.\n");
			return;
	  	}
      /* row grid loop */
      for(m=0;m<gridM;m++)
	  	{
         /* col grid loop */
         for(n=0;n<gridN;n++)
         {
            temprow=gridrow;
            tempcol=gridcol;
         	/* row pixels loop */
            for (q=0,i=m*gridrow;i<((m+1)*gridrow);i++,k++)
            {
               /* col pixels loop */
               for(j=n*gridcol;j<((n+1)*gridcol);j++,l++,q++)
               {
                  /* check bowdaries and store block pixels */
                  if (((i*cols+j) <= (rows*cols)) && (i < rows) && (j < cols))
                  {
                     block_array1[q] = img_array1[i*cols+j]; 
                     block_array2[q] = img_array2[i*cols+j];
                  }
                  else /* out of bounds */
                  {
                     /* store bounds reached to use for rmse rows and cols */
                     temprow = k;
                     tempcol = l;
                     j=n*gridcol+gridcol;  /* breaks out of j loop */
                     break;
                  }
               	/* reset boundaries for new block */
                  if (j==n*gridcol)
				  		{
                     l=0;
				  		}					 
               } /* end col pixels loop */
               /* reset boundaries for new block */
               if (i==m*gridrow)
			   	{
                  k=0;
			   	}				  
            } /* end row pixels loop */
            result[m*gridN+n] = rmse(temprow,tempcol,block_array1,block_array2);
         } /* end row and col grid loops */
      /* free blocks once done using them */
	  	}
      free(block_array1);
      free(block_array2);
      return(result);
   }

   /* this function displays the RMSE table on the screen, it breaks it *
   * up into multiple tables if there are more then 10 cols */
   void disp_table(double *rmsetable)
   {
      int i,j,k; /* loop variables */
      int l; /* records length of printed line */
      int tnum,t; /* break the table into multiple tables */
      /* figure out # of tables needed */
      tnum=gridN/10;
      if (gridN%10 >0)
	  	{
         tnum++;
	  	}
      for(t=0;t<tnum;t++)
      {
         for(i=0;i<gridM;i++)
         {  
            if (i==0)
            {
               printf("\n\nRMSE table for M=%i x N=%i:\n\n        ",gridM,gridN);
            	/* prints colum lables every 10 cols */
               for(j=t*10;j<((t+1)*10),(j/10)<tnum,j<gridN;j++)
               {
                  if (j>=((t+1)*10))
				  		{
                     break;
				  		}
                  printf(" col#%i  ",j);
               }
            }   
            printf("\nrow#%i   ",i);
            if (i < 10) /*prints an extra space when rows are < 10 */
				{
               printf(" ");
				}
            /* prints colum values for every 10 cols */
            for(j=t*10;j<((t+1)*10),(j/10)<tnum,j<gridN;j++)
            {
               if (j>=((t+1)*10))
			   	{
                  break;
			   	}
               l=printf("%.2f",rmsetable[i*gridN+j]);
            	/* prints the spacing between the cols */
               for(k=(8-l);k>0;k--)
			   	{
                  printf(" ");
			   	}
               if (j>9)
			   	{
                  printf(" ");
			   	}
            }  /*end col loop */
         } /* end row loop */
      } /* end table loop */
      printf("\n\n");
   }

   /* this function normalizes the rmse_array based on min/max from both of the input images */
   int rmse_norm(double *rmse_array,int rows, int cols, double *img_array1, double *img_array2) 
   { 
      double max, min, range;  /* max, min and range values for img_array */
      int i; /* loop variable */
      double d; /* temp variable used to normalize */
      if (img_array1[0] > img_array2[0])
      {
         min = img_array2[0];
         max = img_array1[0];
      }
      else /* img_array1[0] < img_array2[0] */
      {
         min = img_array1[0];
         max = img_array2[0];
      }
      /* find min, max and the range values */ 
      for (i = 0; i < (rows*cols); i++)
      { 
         if (img_array1[i] > max)
		 	{		 
            max = img_array1[i];
		 	}			
         if (img_array1[i] < min)
		 	{		 
            min = img_array1[i];
		 	}
         if (img_array2[i] > max)
		 	{		 
            max = img_array2[i];
		 	}			
         if (img_array2[i] < min)
		 	{		 
            min = img_array2[i];
		 	}
      }
      range = max - min; 
      /* new values normalized to [0, 1] */ 
      for (i = 0; i < (gridM*gridN); i++)
	  	{	  
         rmse_array[i] = (rmse_array[i] - min)/range;
	  	}		 
      return(NOERROR); 
   }
