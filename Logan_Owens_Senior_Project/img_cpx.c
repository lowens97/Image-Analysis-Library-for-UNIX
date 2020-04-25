#include "img_head.h"
#include "img_cpx.h"

void cpxCreate(Complex *c, double a, double b)
{
	//creates a complex number based on a and b
	c->mag = cpxMagnitude(a, b);
	c->angle = cpxAngle(a, b);
}

void cpxCreateImag(Complex *c, double b)
{
	//creates a purely imaginary number
	if(b < 0.0)
	{
		c->mag = -b;
		c->angle = -PI / 2;
	}
	else
	{
		c->mag = b;
		c->angle = PI / 2;
	}
}

void cpxCreateReal(Complex *c, double a)
{
	//creates a purely real number
	if(a < 0.0)
	{
		c->mag = -a;
		c->angle = PI;
	}
	else
	{
		c->mag = a;
		c->angle = 0.0;
	}
}

void cpxCopy(Complex *dest, Complex *src)
{
	//clones a complex number
	dest->mag = src->mag;
	dest->angle = src->angle;
}

double cpxMagnitude(double a, double b)
{
	//returns magnitude of complex number
	return sqrt((a * a) + (b * b));
}

double cpxAngle(double a, double b)
{
	//returns angle of complex number
	double ret;
	if(a == 0.0) //strictly imaginary
	{
		ret = (b < 0.0 ? -PI / 2 : PI / 2);
	}
	else if(b == 0.0) //strictly real
	{
		ret = (a < 0.0 ? PI : 0.0);
	}
	else if(a < 0.0) //go to quadrants II and III
	{
		ret = atan(b / a) + PI;
	}
	else //quadrants I and IV
	{
		ret = atan(b / a);
	}
	return ret;
}

double cpxReal(Complex *x)
{
	//returns real part of complex number
	return x->mag * cos(x->angle);
}

double cpxImag(Complex *x)
{
	//returns imaginary part of complex number
	return x->mag * sin(x->angle);
}

/* x = x + y*/
void cpxAdd(Complex *x, Complex *y)
{
	double a1, b1;
	a1 = cpxReal(x);
	b1 = cpxImag(x);
	a1 += cpxReal(y);
	b1 += cpxImag(y);
	x->mag = cpxMagnitude(a1, b1);
	x->angle = cpxAngle(a1, b1);
}

/* x = x - y */
void cpxSub(Complex *x, Complex *y)
{
	double a1, b1;
	a1 = cpxReal(x);
	b1 = cpxImag(x);
	a1 -= cpxReal(y);
	b1 -= cpxImag(y);
	x->mag = cpxMagnitude(a1, b1);
	x->angle = cpxAngle(a1, b1);
}

/* x = x * y */
void cpxMul(Complex *x, Complex *y)
{
	x->mag *= y->mag;
	x->angle += y->angle;
}

/* x = x / y */
void cpxDiv(Complex *x, Complex *y)
{
	x->mag /= y->mag;
	x->angle -= y->angle;
}

/* x = x ^ p */
void cpxPower(Complex *x, double p)
{
	x->mag = pow(x->mag, p);
	x->angle *= p;
}

void cpxNormalizeAngle(Complex *x)
{
	//standardizes angle of complex number
	if(x->angle < 0.0)
	{
		while(x->angle < -PI2)
		{
			x->angle += PI2;
		}
	}
	else
	{
		while(x->angle > PI2)
		{
			x->angle -= PI2;
		}
	}
}

void doubleToCpx(double *d, Complex *c, int n)
{
	//converts a double to a complex number
	int i;
	for(i = 0; i < n; i++)
	{
		cpxCreate(c + i, d[2 * i], d[(2 * i) + 1]);
	}
}

void realToDouble(double *d, int n, double *ret, int retN)
{
	 //converts an array of real numbers to doubles, used for FFT
    int i;
    for(i = 0; i < n; i++)
    {
        ret[2 * i] = d[i];
        ret[(2 * i) + 1] = 0.0;
    }
    for(; i < retN; i++)
    {
        ret[2 * i] = 0.0;
        ret[(2 * i) + 1] = 0.0;
    }
}

void cpxToDouble(Complex *c, int n, double *ret, int retN)
{
	 //converts an array of complex numbers to doubles
	 int i;
    for(i = 0; i < n; i++)
    {
        ret[2 * i] = cpxReal(c + i);
        ret[(2 * i) + 1] = cpxImag(c + i);
    }
    for(; i < retN; i++)
    {
        ret[2 * i] = 0.0;
        ret[(2 * i) + 1] = 0.0;
    }
}
