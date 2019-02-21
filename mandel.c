/* ---------------------------- */
/* David Gago 41710             */
/* Nuno Mendonça 41623          */
/* ---------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ppm.h"
#include <cilk/cilk.h>

#define XCENTER -2
#define YCENTER -1.5
#define SCALE 0.003
#define COLORMAP "mca.map"

unsigned char *matrix;

int main (int argc, char **argv)
{
    int imgsize, n;
    double scale;

    /* help */
    if (argc!=4)
    {
        printf ("%s <ntasks> <imgsize> <file.ppm>\n\n", argv[0]);
        exit(0);
    }
  
    /* setup */
    imgsize = atoi (argv[2]);
    assert (imgsize >= 1024);
    scale = SCALE * 1024 / imgsize;
    matrix = (unsigned char*) calloc (imgsize*imgsize, sizeof (unsigned char));
    assert (matrix != NULL);
  
    /* mandelbrot */
	void fillMap(int start, int range)
	{
	 	int x,y,lp;
		double ax,ay;
		double a1,b1,a2,b2;
		unsigned long int index;

		for (x=start; x<start+range; x++)
			for (y=0; y<imgsize-1; y++)
			{
				ax=XCENTER+x*scale;
				ay=YCENTER+y*scale;
				a1=ax; b1=ay; lp=0;
				while ((lp<=255) && (a1*a1+b1*b1<=4))
				{
				    lp++;
				    a2=a1*a1-b1*b1+ax;
				    b2=2*a1*b1+ay; 
				    a1=a2; 
				    b1=b2;
				}
				if (lp>255)
				    lp=0;
				index = x+(y*imgsize);
				matrix[index]=lp;
			}
	}
	/*
	for (x=0; x<imgsize-1; x++)
		for (y=start; y<start+range; y++)

	for (x=start; x<start+range; x++)
		for (y=0; y<imgsize-1; y++)
			*/

	
	int cores = atoi(argv[1]);
	for (n = 0; n < cores; n++)
		cilk_spawn fillMap(imgsize*n/cores,imgsize/cores);
	cilk_sync;

    /* output to file */
    output_ppm (argv[3], COLORMAP, matrix, imgsize, imgsize, 255);

    free (matrix);
    return 1;
}



