/* Craig Holmquist
Matthew Weingarten
Senior Project CIS4914
IAM Image Library
9/27/04
*/

#include "img_head.h"

#if !defined(min)
#define min(x, y) (x > y ? y : x)
#endif

double centerX, centerY;
int rings;

/*basic checks on passed in arguments*/
void rmse_annu_setup(char **argv, int rows3, int cols3)
{
	int maxRings = min(rows3, cols3) / 2;
	sscanf(ARGV_RING, "%i", &rings);
	if (rings > maxRings) //error
	{
		printf("Not enough pixels for N=%i, N is going to be %i.\n",rings,maxRings);
		rings = maxRings;
	}
	if (rings <= 0) //error
	{
		printf("N cannot be <= 0, N is going to be set to 1.\n");
		rings = 1;
	}
}

/*find the boundaries of each ring*/
double *rmse_ringRadii(int rings, int rows, int cols)
{
	double *ringRadii;
	double difference;
	int i;
	ringRadii = malloc(sizeof(double) * rings);
   if(ringRadii == (double*) NULL) //error allocating space
   {
		printf("Error allocating space.\n");
		return;
   }	
	ringRadii[rings - 1] = (double) min(rows, cols) / 2.0;
	difference = ringRadii[rings - 1] / rings;
	/*printf("%f\n", ringRadii[rings - 1]);*/
	for(i = rings - 1; i > 0; i--)
	{
		ringRadii[i - 1] = ringRadii[i] - difference;
		/*printf("%f\n", ringRadii[i - 1]);*/
	}
	return ringRadii;
}

/*distance from (x,y) to center of image*/
double distance(double x1, double y1)
{
	double dx, dy;
	dx = (centerX - x1);
	dy = (centerY - y1);
	return sqrt((dx * dx) + (dy * dy));
}

/*which ring does this pixel belong to?*/
int correctRing(double dist, double *ringRadii, int rings)
{
	int ret = -1;
	int i;
	for(i = 0; i < rings; i++)
	{
		if(dist <= ringRadii[i])
		{
			ret = i;
			break;
		}
	}
	return ret;
}

void avgsum_annu(int rows, int cols, double *img_array, double **avg, double **sum)
{
	double *ringRadii = rmse_ringRadii(rings, rows, cols);
	int *ppr = malloc(sizeof(int) * rings);
	if(ppr == (int*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		return;
	}
	int i;
	*avg = malloc(sizeof(double) * rings);
	if(*avg == (double*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		return;
	}
	*sum = malloc(sizeof(double) * rings);
   if(*sum == (double*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		return;
	}
	centerX = cols / 2.0;
	centerY = rows / 2.0;
   memset(ppr, 0, sizeof(int) * rings);
   for(i = 0; i < rings; i++)
   {
       (*sum)[i] = 0.0;
   }
   for(i = 0; i < (rows * cols); i++)
   {
        int ringNo;
        double dist = distance((double) (i % cols), (double) (i / cols));
        ringNo = correctRing(dist, ringRadii, rings);
        /*printf("%u %u %d\n", i % rows, i / rows, ringNo);*/
        if(ringNo >= 0)
        {
            (*sum)[ringNo] += img_array[i];
            (ppr[ringNo])++;
        }
        /*otherwise, not in any ring*/
	}
	for(i = 0; i < rings; i++)
	{
		(*avg)[i] = (*sum)[i] / ppr[i];
	}
	free(ppr);
	free(ringRadii);
	return;
}

double *rmse_annu(int rows, int cols, double *img_array1, double *img_array2)
{
	double *ringRadii = rmse_ringRadii(rings, rows, cols); /*holds the radius of each ring*/
	double *ret = malloc(sizeof(double) * rings); /*holds the running sum for each ring*/
	if(ret == (double*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		return;
	}
	int *ppr = malloc(sizeof(int) * rings); /*number of pixels per ring*/
	if(ppr == (int*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		return;
	}
	int i;
	centerX = cols / 2.0;
	centerY = rows / 2.0;
	memset(ppr, 0, sizeof(int) * rings);
	for(i = 0; i < rings; i++)
	{
		ret[i] = 0.0;
	}
	for(i = 0; i < (rows * cols); i++)
	{
		int ringNo;
		double dist = distance((double) (i % cols), (double) (i / cols));
		ringNo = correctRing(dist, ringRadii, rings);
		/*printf("%u %u %d\n", i % rows, i / rows, ringNo);*/
		if(ringNo >= 0)
		{
			ret[ringNo] += (img_array1[i] - img_array2[i]) * 
			(img_array1[i] - img_array2[i]);
			
			(ppr[ringNo])++;
		}
		/*otherwise, not in any ring*/
	}
	for(i = 0; i < rings; i++)
	{
		if(ppr[i] != 0)
		{
			ret[i] = sqrt(ret[i] / ppr[i]);
		}
		else
		{
			ret[i] = 0.0;
		}
	}
	free(ringRadii);
	free(ppr);
	return ret;
}

void dispRingTable(double *rmsetable, int rows3, int cols3)
{
	int i;
	printf("\tNrings = %d\n", rings);
	printf("\tWidth  = %.2f\n", (double) min(rows3, cols3) / rings);
	for(i = 0; i < rings; i++)
	{
		printf("\tRMSE for Ring %u\t\t= %.2f\n", i, rmsetable[i]);
	}
}
