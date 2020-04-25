#include "img_head.h"

//1D decimated discrete wavelet transform - outputs signal to txt file
void img_dwt(int rows, int cols, double *img_array, char *file) {
	wave_object obj;
	wt_object wt;
	//decomp levels (add as argument?)
	int J = 1;
	int i = 0;

	//Initialize the wavelet
	char *name = "db4";
	obj = wave_init(name);
	wt = wt_init(obj, "dwt", rows*cols, J);// Initialize the wavelet transform object
	setDWTExtension(wt, "sym");// Options are "per" and "sym". Symmetric is the default option
	setWTConv(wt, "direct");
	
	dwt(wt, img_array);// Perform DWT
	//DWT output can be accessed using wt->output vector. Use wt_summary to find out how to extract appx and detail coefficients
	
	//create file
	FILE *f = fopen(file, "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	
	//print into file
	for (i = 0; i < wt->outlength; ++i) {
		fprintf(f, "%g\n",wt->output[i]);
	}
	
	fclose(f);
	printf("%s created!\n", file);
	
	wave_free(obj);
	wt_free(wt);
}

//2D decimated discrete wavelet transform - returns transformed array
double *img_dwt2(int rows, int cols, double *img_array, int rDimensions, int idwt) {
	
	wave_object obj;
	wt2_object wt;
	int i, k, J, N;
	double *wavecoeffs,*oup;
	double	*dim;
	double *cLL;
	int ir, ic;

    N = rows*cols;

	char *name = "db2";
	obj = wave_init(name);// Initialize the wavelet
	//inp = (double*)calloc(N, sizeof(double));
	oup = (double*)calloc(N, sizeof(double));
	dim = (double*)calloc(2, sizeof(double));
    
	//decomp levels (add as argument?)
	J = 1;

	wt = wt2_init(obj, "dwt", rows,cols, J);


	//init idwt vector
	for (i = 0; i < rows; ++i) {
		for (k = 0; k < cols; ++k) {
			oup[i*cols + k] = 0.0;
		}
	}
	
	//create transformed vector
	wavecoeffs = dwt2(wt, img_array);
	cLL = getWT2Coeffs(wt, wavecoeffs, 1, "D", &ir, &ic);
	//perform inverse wavelet transform
	idwt2(wt, wavecoeffs, oup);
	

	dim[0] = ir;
	dim[1] = ic;	
	
	if(idwt == 1)
	{
		dim[0] = rows;
		dim[1] = cols;
	}
	
	//return dimensions	
	if(rDimensions == 1)
	{
		wave_free(obj);
		wt2_free(wt);
		free(wavecoeffs);
		return dim;  
	}
	
	
	if(idwt == 1)
	{
		printf("Returning Inverse DWT.\n");
		wave_free(obj);
		wt2_free(wt);
		free(wavecoeffs);
		return oup;  
	}
	
	wave_free(obj);
	wt2_free(wt);
	free(wavecoeffs);
	printf("Returning DWT.\n");
	return cLL;
}