/*
 * YAPP: Yet Another Plotting Package.
 *
 *      This uses the C interface to GNUPLOT using unix pipes (originally
 *      written by Nicolas Devillard) - 
 *      See https://github.com/longradix/gnuplot_i for the most recent
 *      implementation
 *      
 *      NOTE: this version is the placeholder until actually implemented
 */

#include <stdinc.h>
#include "gnuplot_i.c"       // needs to be installed in $NEMOLIB

extern string yapp_string;

//local stream yappstr = NULL;
//local string yapp_dev;
local real xp, yp;
local int ncolors = 16;      /* kind of faking pgplot */

local gnuplot_ctrl *handle;

int plinit(string pltdev, real xmin, real xmax, real ymin, real ymax)
{
  dprintf(0,"yapp_gnuplot\n");
  handle = gnuplot_init();
  if (1) {
    gnuplot_setterm(handle, "wxt", 600, 400);
  } else {
    gnuplot_cmd(handle, "set terminal png size 600, 400");
    gnuplot_cmd(handle, "set output \"yapp.png\"");
  }
  //gnuplot_setterm(handle, "dumb", 150, 40);
  
  //yappstr = stropen(yapp_string,"w");
  //fprintf(yappstr,"plinit %5.2f %5.2f %5.2f %5.2f %s\n",xmin,xmax,ymin,ymax,yapp_string);
  
  xp = yp = 0.0;
}

int plswap() 
{
  //fprintf(yappstr,"plswap\n");
}

real plxscale(real x, real y)
{
  //fprintf(yappstr,"plxscale %5.2f %5.2f\n",x,y);
}

real plyscale(real x, real y)
{
  //fprintf(yappstr,"plyscale %5.2f %5.2f\n",x,y);
}

int plltype(int lwid, int lpat)
{
  // gnuplot_setstyle(handle, "lines");
}

int plline(real x, real y)
{
  //fprintf(yappstr,"plline %5.2f %5.2f\n",x,y);
}

int plmove(real x, real y)
{
  xp = x;
  yp = y;
}

int plpoint(real x, real y)
{
  //gnuplot_cmd(handle, "set \n");
  //fprintf(yappstr,"plpoint %5.2f %5.2f\n",x,y);
}

int plcircle(real x, real y, real r)
{
  //fprintf(yappstr,"plcircle %5.2f %5.2f %5.2f\n",x,y,r);    
}

int plcross(real x, real y, real s)
{
  //fprintf(yappstr,"plcross %5.2f %5.2f %5.2f\n",x,y,s);
}

int plbox(real x, real y, real s)
{
  //fprintf(yappstr,"plbox %5.2f %5.2f %5.2f\n",x,y,s);
}

int pljust(int jus)
{
  //fprintf(yappstr,"pljust %d\n",jus);
}

int pltext(string msg, real x, real y, real hgt, real ang)
{
  //fprintf(yappstr,"pltext %5.2f %5.2f %5.2f %5.2f %s\n",x,y,hgt,ang,msg);
}

int plflush() 
{
  //fprintf(yappstr,"plflush\n");
}

int plframe()
{
  gnuplot_resetplot(handle);
}

int plstop()
{
    gnuplot_close (handle);
    //fprintf(yappstr,"plstop\n");
    //strclose(yappstr);
    //yappstr=NULL;
}

int plncolors()
{
  return ncolors;
}

void plcolor(int color)
{
  //fprintf(yappstr,"color %d\n",color);
}

void plpalette(real *r, real *g, real *b, int n)
{
    /* cannot handle them */
    //fprintf(yappstr,"# pallete %d\n",n);
}


int pl_matrix(real *frame,int nx,int ny,real xmin,real ymin, real cell,
	  real fmin,real fmax,real findex, real blankval)
{
  //fprintf(yappstr,"pl_matrix\n");
}

int pl_screendump(string fname)
{
  //fprintf(yappstr,"pl_screendump\n");
}

int pl_getpoly(float *x, float *y, int n)
{
  //fprintf(yappstr,"pl_getpoly\n");
    return 0;
}

int pl_interp(string cmd)
{
  //fprintf(yappstr,"pl_interp\n");
}

int pl_cursor(float *x, float *y, char *c)
{
    warning("no pl_cursor");
}
