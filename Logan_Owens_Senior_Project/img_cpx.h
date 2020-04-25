/* number = mag * e ^ (angle * i) */
typedef struct
{
	double mag; /* magnitude, must always be nonnegative */
	double angle; /* angle in radians */
} Complex;

void cpxCreate(Complex *c, double a, double b);
void cpxCreateImag(Complex *c, double b);
void cpxCreateReal(Complex *c, double a);
void cpxCopy(Complex *dest, Complex *src);
double cpxMagnitude(double a, double b);
double cpxAngle(double a, double b);
double cpxReal(Complex *x);
double cpxImag(Complex *x);
void cpxAdd(Complex *x, Complex *y);
void cpxSub(Complex *x, Complex *y);
void cpxMul(Complex *x, Complex *y);
void cpxDiv(Complex *x, Complex *y);
void cpxPower(Complex *x, double p);
void cpxNormalizeAngle(Complex *x);
void cpxToDouble(Complex *c, int n, double *ret, int retN);
void realToDouble(double *d, int n, double *ret, int retN);
void doubleToCpx(double *d, Complex *c, int n);
