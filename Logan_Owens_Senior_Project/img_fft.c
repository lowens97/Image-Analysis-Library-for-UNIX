#include "img_head.h"
#include "img_cpx.h"

/* next integer >= x that is a power of 2 */
#define NEXT_P2(x) ((int) pow(2.0, ceil( LN2(x) ) ) )

extern int rings;
extern char *fftOut;
extern char *inpImg;

void bfft(double *, int, int);

void graphDensity(double *mag, int rings)
{
	char demFile[] =
"set title \"Power Spectrum\"\n\
set data style line\n\
set key below\n\
set xrange[0:1]\n\
set xlabel \"1 / ring number (Nrings = %u)\"\n\
set ylabel \"log(Magnitude)\"\n\
plot 'temp_gpl.tr' using 1:2\n\
pause -1 \"Hit return to continue\"\n";
	FILE *fp;
	int i;
	pid_t p;
   fp = fopen("temp_gpl.dem", "w");
   if(!fp) //error with opening file
   {
       printf("Unable to open .dem file.\n");
       return;
   }
	fprintf(fp, demFile, rings);
	int status = fclose(fp);
	if(status != 0) //error closing file
	{
		printf("Error closing file.\n");
		return;
	}
   fp = fopen("temp_gpl.tr", "w");
   if(!fp) //error opening file
   {
       printf("Unable to open trace file.\n");
       int status = unlink("temp_gpl.dem");
		 if(status == -1) //error unlinking
		 {
			printf("Unable to unlink file.\n");
			return;
		 }
		 return;
   }
	for(i = rings - 1; i >= 0; i--)
	{
		fprintf(fp, "%f\t%f\n", 1.0 / (i + 1), log(mag[i]));
	}
	status = fclose(fp);
	if(status != 0) //error closing file
	{
		printf("Error closing file.\n");
		return;
	}
	p = fork();
	if(p == 0) //child process
	{
		execl("/usr/bin/gnuplot", "gnuplot", "temp_gpl.dem", (char*) NULL);
	}
	else //parent process
	{
		int d;
		int status = waitpid(p, &d, 0);
		if(status == -1) //error waiting
		{
			printf("Error waiting on previous display to die.\n");
			return;
		}
	} 
	status = unlink("temp_gpl.dem");
	if(status == -1) //error unlinking
	{
		printf("Unable to unlink file.\n");
		return;
	}
	status = unlink("temp_gpl.tr");
	if(status == -1) //error unlinking
	{
		printf("Unable to unlink file.\n");
		return;
	}
}

/*flips the input image vertically and horizontally*/
void flipImage(int rows, int cols, double **row)
{
	int upperR, lowerR;
	upperR = 0;
	lowerR = rows - 1;
	for(; upperR <= lowerR; upperR++, lowerR--)
	{
		double *upper = row[upperR];
		double *lower = row[lowerR];
		int i,j;
		for(i = 0, j = cols - 1; i < cols; i++, j--) //swaps
		{
			double temp;
			temp = upper[i];
			upper[i] = lower[j];
			lower[j] = temp;
		}
	}
	return;
}

/*flips each quadrant of the image*/
void centrateImage(int rows, int cols, double *img)
{
	double **rowPointers;
	int i;
	int newCols;
	int newRows;
	printf("Centering image...\n");
	rowPointers = malloc(sizeof(double *) * rows);
	if(rowPointers == (double**) NULL)
	{
		printf("Error allocating space.\n");
		return;
	}
	for(i = 0; i < rows; i++)
	{
		rowPointers[i] = img + (i * cols);
	}
	/*upper left quadrant*/
	flipImage(rows / 2, cols / 2, rowPointers);
	/*lower left quadrant*/
	newRows = (rows & 1 ? (rows / 2) + 1 : (rows / 2) ); //checks for divisibility by 2
	flipImage(newRows, cols / 2, rowPointers + (rows / 2));
	for(i = 0; i < rows; i++)
	{
		rowPointers[i] += (cols / 2);
	}
	/*upper right quadrant*/
	newCols = (cols & 1 ? (cols / 2) + 1 : (cols / 2) ); //checks for divisibility by 2
	flipImage(rows / 2, newCols, rowPointers);
	/*lower right quadrant*/
	flipImage(newRows, newCols, rowPointers + (rows / 2));
	return;
}

void normalizeImageArray(double *d, int n)
{
	//scales image array to a grayscale
	double max, min;
	double scale;
	//double mean;
	int i;
	printf("Scaling image to standard grayscale range...\n");
	//printf("init: %u %e\n", n, d[0]);
	max = d[0];
	min = d[0];
	//mean = d[0];
	for(i = 1; i < n; i++)
	{
		/*if(!finite(d[i]))
		{
			printf("Warning: NaN in data set!\n");
		}*/
		if(d[i] < min)
		{
			/*printf("New min: %f\n", d[i]);*/
			min = d[i];
		}
		else if(d[i] > max)
		{
			/*printf("New max: %f\n", d[i]);*/
			max = d[i];
		}
		/*mean += d[i];*/
	}
	if(max != min)
	{
		scale = 255.0 / (max - min);
	}
	else
	{
		scale = 0.0;
	}
	//printf("Image max: %f\nImage min: %f\n", max, min);
	for(i = 0; i < n; i++)
	{
		/*printf("%f\n", d[i]);*/
		d[i] = (d[i] - min) * scale;
	}
	return;
}

void transposeImage(int rows, int cols, double *in, double *out)
{
	//transposes image
	int i;
	for(i = 0; i < (rows * cols); i++)
	{
		int x, y;
		y = i / cols;
		x = i % cols;
		out[(x * 2 * rows) + (y * 2)] = in[(x * 2) + (y * 2 * cols)];
		out[((x * 2 * rows) + (y * 2)) + 1] = in[(x * 2) + (y * 2 * cols) + 1];
	    //printf("%u\n", i);
 	}
	return;
}

/*void writeFFTImages(Complex *c, int rows, int cols)
{
	double *newImg;
	int i;
	newImg = malloc(rows * cols * sizeof(double));
	if(newImg == (double*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		return;
	}
	/*first, real part
	for(i = 0; i < (rows * cols); i++)
	{
		/*printf("%e %e\n", (c + i)->mag, (c + i)->angle);
		newImg[i] = cpxReal(c + i);
	}
	normalizeImageArray(newImg, rows * cols);
	if(write_image(rows, cols, newImg, "fft_cr.pgm") == ERROR)
	{
		printf("Error writing real part.\n");
		exit(1);
	}
    /*imag part
    for(i = 0; i < (rows * cols); i++)
    {
        newImg[i] = cpxImag(c + i);
    }
	normalizeImageArray(newImg, rows * cols);
    if(write_image(rows, cols, newImg, "fft_ci.pgm") == ERROR)
    {
        printf("Error writing imaginary part.\n");
        exit(1);
    }
    /*magnitude
    for(i = 0; i < (rows * cols); i++)
    {
        newImg[i] = (c + i)->mag;
    }
	normalizeImageArray(newImg, rows * cols);
    if(write_image(rows, cols, newImg, "fft_d.pgm") == ERROR)
    {
        printf("Error writing magnitude.\n");
        exit(1);
    }
    /*phase
    for(i = 0; i < (rows * cols); i++)
    {
        newImg[i] = (c + i)->angle;
    }
	normalizeImageArray(newImg, rows * cols);
    if(write_image(rows, cols, newImg, "fft_e.pgm") == ERROR)
    {
        printf("Error writing phase.\n");
        exit(1);
    }
	free(newImg);
}*/

void zeroDouble(double *d, int n)
{
	//zeroes the array
	int i;
	for(i = 0; i < n; i++) 
	{
		d[i] = 0.0;
	}
	return;
}

#define FFT_OUTPUT_FILES 4

void displayFFTImage(double *d, int rows, int cols)
{
	//uses ImageMagick's display routine to display the image
   //	double *d = malloc(rows * cols * sizeof(double));
	// if(d == (double*) NULL) //error allocating space
	// {
	//		printf("Error allocating space.\n");
	//		return;
	//	}
   //	memcpy((void *) d, (void *) img, rows * cols * sizeof(double));
	normalizeImageArray(d, rows * cols);
	centrateImage(rows, cols, d);
	write_image(rows, cols, d, "temp_out.pgm");
	pid_t pid = fork();
	if(pid == 0) //child process
	{
		int status = execl("/usr/bin/display", "display", "temp_out.pgm", (char*) NULL);
		if(status == -1) //error executing
		{
			printf("Error executing display.\n");
			return;
		}
	}
	else //parent process
	{
		int loc;
		char response[256];
		printf("Should this image be displayed in log(mag) format? ");
		fgets(response, sizeof(response), stdin);
		if(strcmp(response, "y") == 0 || strcmp(response, "yes") == 0 || strcmp(response, "Y") == 0 || strcmp(response, "YES") == 0)
		{
			int i;
			int status = kill(pid, SIGKILL);
			if(status == -1) //error killing
			{
				printf("Error killing parent process.\n");
				return;
			}
			for(i = 0; i < (rows * cols); i++)
			{
				d[i] = log(d[i] + 1.0);
			}
			normalizeImageArray(d, rows * cols);
			status = waitpid(pid, &loc, 0); //wait for previous display to die
			if(status == -1) //error waiting
			{
				printf("Error waiting for previous display to die.\n");
				return;
			}
			status = unlink("temp_out.pgm");
			if(status == -1) //error unlinking
			{
				printf("Error unlinking.\n");
				return;
			}
			write_image(rows, cols, d, "temp_out.pgm");
			pid = fork();
			if(pid == 0) //child process
			{
				status = execl("/usr/bin/display", "display", "temp_out.pgm", (char*) NULL);
				if(status == -1) //error executing
				{
					printf("Error executing.\n");
					return;
				}
			}
		} /* end if yes */
		printf("Close display box (click <<X>>) to continue...\n");
		int status = waitpid(pid, &loc, 0);
		if(status == -1) //error waiting
		{
			printf("Error waiting.\n");
			return;
		}
		status = unlink("temp_out.pgm");
		if(status == -1) //error unlinking
		{
			printf("Error unlinking.\n");
			return;
		}
	}
}

Complex *img_fft_worker(int rows, int cols, double *img)
{
	//pads image array with zeros where necessary and then calculates the FFT on
	//each column of the array
	int outRows = NEXT_P2(rows);
	int outCols = NEXT_P2(cols);
	int size = outRows * outCols * 2 * sizeof(double);
	double *imgA, *imgBt;
	Complex *c;
	int i;
	printf("FFT Area: %u x %u\n", outRows, outCols);
	printf("Computing row FFT...\n");
	imgA = malloc(size);
	if(imgA == (double*)NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		return;
	}
	for(i = 0; i < rows; i++)
	{
		realToDouble(img + (cols * i), cols, imgA + (2 * i * outCols), outCols);
	}
	for(; i < outRows; i++)
	{
		zeroDouble(imgA + (2 * i * outCols), 2 * outCols);
	}
	/*for(i = 0; i < (outRows * outCols); i++)
	{
		printf("%u: %e\n", i, imgA[i]);
	}*/
	for(i = 0; i < outRows; i++)
	{
		bfft(imgA + (2 * i * outCols), outCols, 1);
	}
	printf("Computing column FFT...\n");
    /*for(i = 0; i < (outRows * outCols); i++)
    {       
        printf("%u: %e\n", i, imgA[i]);
    }*/
	imgBt = malloc(size);
	if(imgBt == (double*)NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		return;
	}
	transposeImage(outRows, outCols, imgA, imgBt);
	/*for(i = 0; i < outCols; i++)
	{
		printf("%e", imgBt[i]);
	}
	printf("\n");*/
   for(i = 0; i < outCols; i++)
   {
       bfft(imgBt + (2 * i * outRows), outRows, 1);
   }
   /*for(i = 0; i < outRows; i++)
   {
       printf("%e ", imgBt[i]);
   }
   printf("\n");
	printf("%e\n", imgBt[0]);*/
	transposeImage(outCols, outRows, imgBt, imgA);
	free(imgBt);
	c = malloc(sizeof(Complex) * outRows * outCols);
	if(c == (Complex*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		return;
	}
	doubleToCpx(imgA, c, outRows * outCols);
	free(imgA);
	printf("Finished FFT computation.\n");
    /*	writeFFTImages(c, outRows, outCols);
	displayFFTImages();*/
	return c;
}

void img_fft(int rows, int cols, double *img)
{
	//called from the main method, the driver method for FFT
   int outRows, outCols;
   Complex *c = img_fft_worker(rows, cols, img);
	double *d;
	int i;
   outRows = NEXT_P2(rows); //computes next power of 2
   outCols = NEXT_P2(cols); //computes next power of 2
   /*  writeFFTImages(c, outRows, outCols);
   displayFFTImages();*/
	d = malloc(sizeof(double) * outRows * outCols);
	if(d == (double*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		return;
	}
	printf("\nDisplaying real part...\n");
	for(i = 0; i < (outRows * outCols); i++)
	{
		d[i] = cpxReal(c + i);
	}
	displayFFTImage(d, rows, cols);
	printf("\nDisplaying imaginary part...\n");
   for(i = 0; i < (outRows * outCols); i++)
   {
       d[i] = cpxImag(c + i);
   }
   displayFFTImage(d, rows, cols);
   printf("\nDisplaying magnitude...\n");
   for(i = 0; i < (outRows * outCols); i++)
   {
       d[i] = c[i].mag;
   }
   displayFFTImage(d, rows, cols);
	printf("\nDisplaying phase...\n");
   for(i = 0; i < (outRows * outCols); i++)
   {
       d[i] = c[i].angle;
   }
   displayFFTImage(d, rows, cols);
	free(d);
	free(c);
}

void outputGraphData(double *d, int rings, char *file)
{
	int i;
	FILE *fp;
	printf("Writing data to %s...\n", file);
	fp = fopen(file, "w");
	if(fp == NULL) //error opening file
	{
		printf("Unable to open %s.\n", file);
		return;
	}
	fprintf(fp, "FFT Spectrum, from image %s, Nrings = %u\n", inpImg, rings);
	for(i = rings - 1; i >= 0; i--)
	{
		fprintf(fp, "%f, %f\n", 1.0 / (i + 1), log(d[i]));
	}
	int status = fclose(fp);
	if(status != 0) //error closing file 
	{
		printf("Error closing file.\n");
		return;
	}
}

void img_fft_graphps(int rows, int cols, double *img)
{
	Complex *c;
	int i;
	int outRows = NEXT_P2(rows);
	int outCols = NEXT_P2(cols);
	double *outImg;
	double *dummyImg;
	double *avg;
	double *sum;
	outImg = malloc(sizeof(double) * outRows * outCols);
	if(outImg == (double*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		return;
	}
   dummyImg = malloc(sizeof(double) * outRows * outCols);
	if(dummyImg == (double*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		return;
	}
	zeroDouble(dummyImg, outRows * outCols);
    c = img_fft_worker(rows, cols, img);
	for(i = 0; i < (outRows * outCols); i++)
	{
		outImg[i] = (c + i)->mag;
	}
	free(c);
	normalizeImageArray(outImg, outRows * outCols);
   /* write_image(outRows, outCols, outImg, "fr_outt.pgm");*/
	centrateImage(outRows, outCols, outImg);
   /* write_image(outRows, outCols, outImg, "fr_out.pgm");*/
	rings = min(outCols / 2, outRows / 2);
   /*	printf("%u\n", rings);*/
    /*	rmseOut = rmse_annu(outRows, outCols, outImg, dummyImg);
	dispRingTable(rmseOut, outRows, outCols);*/
	avgsum_annu(rows, cols, outImg, &avg, &sum);
	graphDensity(sum, rings);
	if(fftOut != NULL)
	{
		outputGraphData(sum, rings, fftOut);
	}
	free(outImg);
	free(dummyImg);
	free(avg);
	free(sum);
	return;
}

void bfft(double data[], int nn, int isign)
/*
 * This is the Danielson and Lanczos implementation of the fast Fourier
 * transform as described in Numerical Recipes, Press et al in section 12.2.
 * It has been tested by comparing with THE ORIGINAL COOLEY-TUKEY TRANSFORM,
 * which is a Fortran 4 implementation of the same code. Bob Coldwell
 * converted the Fortran to C.  The data is assumed to have real values in
 * data[0],data[2],data[4],... and imaginary values in data[1],data[3],...
 * This is not the same as Press's four1 which starts with a real in data[1]
 * transform[k]=sum(data(j)*exp(isign*2*pi*sqrt(-1)*j*k/nn)) The sum is over
 * all j from 0 to nn-1 nn must be a power of 2.  The dimension of the data
 * is 2*nn; that is, it has nn real values and nn imaginary values.  The
 * transform is placed in data replacing the original values.
 */
{
	double wr, wi, wpr, wpi, wtemp, theta;
	double tempr, tempi;
	int m, n, i, j, istep, mmax;
	n = 2 * nn;
	j = 1;
	for (i = 1; i < n; i += 2)
	{
		if (j > i)
		{
			tempr = data[j - 1];
			tempi = data[j];
			data[j - 1] = data[i - 1];
			data[j] = data[i];
			data[i - 1] = tempr;
			data[i] = tempi;
		}
		m = n / 2;
was1:
		if (m >= 2 && j > m)
		{
			j = j - m;
			m = m / 2;
			goto was1;
		}
		j = j + m;
	}
	/*
	 * Here begins the Danielson-Lanczos section (outer loop executed
	 * FFT00380 Log2 (NN) times
	 */
	mmax = 2;
	while (n > mmax)
	{
		istep = 2 * mmax;
		theta = isign * (PI2 / mmax);
		wtemp = sin(0.5 * theta);
		wpr = -2.0 * wtemp * wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		for (m = 1; m < mmax; m += 2)
		{
			for (i = m; i <= n; i += istep)
			{
				j = i + mmax;
				tempr = wr * data[j - 1] - wi * data[j];
				tempi = wr * data[j] + wi * data[j - 1];
				data[j - 1] = data[i - 1] - tempr;
				data[j] = data[i] - tempi;
				data[i - 1] += tempr;
				data[i] += tempi;
			}
			wr = (wtemp = wr) * wpr - wi * wpi + wr;
			wi = wi * wpr + wtemp * wpi + wi;
		}
		mmax = istep;
	}
}
