/*
 * mdarray: a simple multi-dimensional array (allocator)
 *
 * Note:  MDMAXDIM defines the highest dimension we've implemented (currenlty 7)
 *
 *  5-may-2003   Created           Peter Teuben
 *
 */

#include <nemo.h>
#include <mdarray.h>

mdarray1 allocate_mdarray1(int n1)
{
  return (mdarray1 )allocate(n1*sizeof(real));
}

mdarray2 allocate_mdarray2(int n2,int n1)
{
  mdarray2 x = (mdarray2)allocate(sizeof(mdarray2)*n2);
  int i;
  for (i=0;i<n2;i++)
    x[i] = allocate_mdarray1(n1);
  return x;
}

mdarray3 allocate_mdarray3(int n3,int n2,int n1)
{
  mdarray3 x = (mdarray3)allocate(sizeof(mdarray3)*n3);
  int i;
  for (i=0;i<n3;i++)
    x[i] = allocate_mdarray2(n2,n1);
  return x;
}

mdarray4 allocate_mdarray4(int n4,int n3,int n2,int n1)
{
  mdarray4 x = (mdarray4)allocate(sizeof(mdarray4)*n4);
  int i;
  for (i=0;i<n4;i++)
    x[i] = allocate_mdarray3(n3,n2,n1);
  return x;
}

mdarray5 allocate_mdarray5(int n5,int n4,int n3,int n2,int n1)
{
  mdarray5 x = (mdarray5)allocate(sizeof(mdarray5)*n5);
  int i;
  for (i=0;i<n5;i++)
    x[i] = allocate_mdarray4(n4,n3,n2,n1);
  return x;
}

mdarray6 allocate_mdarray6(int n6,int n5,int n4,int n3,int n2,int n1)
{
  mdarray6 x = (mdarray6)allocate(sizeof(mdarray6)*n6);
  int i;
  for (i=0;i<n6;i++)
    x[i] = allocate_mdarray5(n5,n4,n3,n2,n1);
  return x;
}

mdarray7 allocate_mdarray7(int n7,int n6,int n5,int n4,int n3,int n2,int n1)
{
  mdarray7 x = (mdarray7)allocate(sizeof(mdarray7)*n7);
  int i;
  for (i=0;i<n7;i++)
    x[i] = allocate_mdarray6(n6,n5,n4,n3,n2,n1);
  return x;
}

void free_mdarray1(mdarray1 x, int n1)
{
  free(x);
}

void free_mdarray2(mdarray2 x, int n2, int n1)
{
  int i;
  for (i=0;i<n2;i++)
    free_mdarray1(x[i],n1);
  free(x);
}

void free_mdarray3(mdarray3 x, int n3, int n2, int n1)
{
  int i;
  for (i=0;i<n3;i++)
    free_mdarray2(x[i],n2,n1);
  free(x);
}

void free_mdarray4(mdarray4 x, int n4, int n3, int n2, int n1)
{
  int i;
  for (i=0;i<n4;i++)
    free_mdarray3(x[i],n3,n2,n1);
  free(x);
}

void free_mdarray5(mdarray5 x, int n5, int n4, int n3, int n2, int n1)
{
  int i;
  for (i=0;i<n5;i++)
    free_mdarray4(x[i],n4,n3,n2,n1);
  free(x);
}

void free_mdarray6(mdarray6 x, int n6, int n5, int n4, int n3, int n2, int n1)
{
  int i;
  for (i=0;i<n6;i++)
    free_mdarray5(x[i],n5,n4,n3,n2,n1);
  free(x);
}

void free_mdarray7(mdarray7 x, int n7, int n6, int n5, int n4, int n3, int n2, int n1)
{
  int i;
  for (i=0;i<n7;i++)
    free_mdarray6(x[i],n6,n5,n4,n3,n2,n1);
  free(x);
}



/* =============================================================================== */
#ifdef TESTBED

string defv[] = {
  "dim=10,20,5\n    Dimensions of array A[dim1][dim2][dim3]....",
  "flip=f\n         Reverse traversal through array, for benchmarking",
  "iter=1\n         Number of times to do the work, for benchmarking",
  "free=f\n         Free-Allocate the array each iter? [not implemented]",
  "VERSION=1.0\n    5-may-03 PJT",
  NULL,
};

string usage = "testing nested multidimensional arrays in C";

init1(int *dim,mdarray1 x, bool flip)
{
  int i;

  for (i=0; i<dim[0]; i++)
    x[i] = i;
  
}

work1(int *dim,mdarray1 x, bool flip)
{
  int i;
  real sum=0;

  for (i=0; i<dim[0]; i++)
    sum += x[i]; 
  dprintf(1,"sum1=%g\n",sum);
}


init2(int *dim,mdarray2 x,bool flip)
{
  int i,j;

  if (flip) {
    for (i=0; i<dim[0]; i++)
      for (j=0; j<dim[1]; j++)
	x[j][i] = (real)i+j;
  } else {
    for (j=0; j<dim[1]; j++)
      for (i=0; i<dim[0]; i++)
	x[j][i] = (real)i+j;
  }
}

work2(int *dim,mdarray2 x,bool flip)
{
  int i,j;
  real sum=0;

  if (flip) {
    for (i=0; i<dim[0]; i++)
      for (j=0; j<dim[1]; j++)
	sum += x[j][i];
  } else {
    for (j=0; j<dim[1]; j++)
      for (i=0; i<dim[0]; i++)
	sum += x[j][i];
  }
  dprintf(1,"sum2=%g\n",sum);
}

init3(int *dim,mdarray3 x, bool flip)
{
  int i,j,k;


  if (flip) {
    for (i=0; i<dim[0]; i++)
      for (j=0; j<dim[1]; j++)
	for (k=0; k<dim[2]; k++)
	  x[k][j][i] = (real)i+j+k;
  } else {
    for (k=0; k<dim[2]; k++)
      for (j=0; j<dim[1]; j++)
	for (i=0; i<dim[0]; i++)
	  x[k][j][i] = (real)i+j+k;
  }
}

work3(int *dim,mdarray3 x, bool flip)
{
  int i,j,k;
  real sum=0;

  if (flip) {
    for (i=0; i<dim[0]; i++)
      for (j=0; j<dim[1]; j++)
	for (k=0; k<dim[2]; k++)
	  sum += x[k][j][i];	  
  } else {
    for (k=0; k<dim[2]; k++)
      for (j=0; j<dim[1]; j++)
	for (i=0; i<dim[0]; i++)
	  sum += x[k][j][i];	  
  }
  dprintf(1,"sum3=%g\n",sum);
}

init4(int *dim,mdarray4 x, bool flip)
{
  int i,j,k,l;

  if (flip) {
    for (i=0; i<dim[0]; i++)
      for (j=0; j<dim[1]; j++)
	for (k=0; k<dim[2]; k++)
	  for (l=0; l<dim[3]; l++)
	    x[l][k][j][i] = (real)i+j+k+l;
  } else {
    for (l=0; l<dim[3]; l++)
      for (k=0; k<dim[2]; k++)
	for (j=0; j<dim[1]; j++)
	  for (i=0; i<dim[0]; i++)
	    x[l][k][j][i] = (real)i+j+k+l;
  }
}

work4(int *dim,mdarray4 x, bool flip)
{
  int i,j,k,l;
  real sum=0;

  if (flip) {
    for (i=0; i<dim[0]; i++)
      for (j=0; j<dim[1]; j++)
	for (k=0; k<dim[2]; k++)
	  for (l=0; l<dim[3]; l++)
	    sum += x[l][k][j][i];
  } else {
    for (l=0; l<dim[3]; l++)
      for (k=0; k<dim[2]; k++)
	for (j=0; j<dim[1]; j++)
	  for (i=0; i<dim[0]; i++)
	    sum += x[l][k][j][i];
  }
  dprintf(1,"sum4=%g\n",sum);
}

work5(int *dim,mdarray5 x, bool flip)
{
  int i,j,k,l,m;

  if (flip) {
    for (i=0; i<dim[0]; i++)
      for (j=0; j<dim[1]; j++)
	for (k=0; k<dim[2]; k++)
	  for (l=0; l<dim[3]; l++)
	    for (m=0; m<dim[4]; m++)
	      x[m][l][k][j][i] = i+j+k+l+m;
  } else {
    for (m=0; m<dim[4]; m++)
      for (l=0; l<dim[3]; l++)
	for (k=0; k<dim[2]; k++)
	  for (j=0; j<dim[1]; j++)
	    for (i=0; i<dim[0]; i++)
	      x[m][l][k][j][i] = i+j+k+l+m;
  }
}

work6(int *dim,mdarray6 x, bool flip)
{
  int i,j,k,l,m,n;

  if (flip) {
    for (i=0; i<dim[0]; i++)
      for (j=0; j<dim[1]; j++)
	for (k=0; k<dim[2]; k++)
	  for (l=0; l<dim[3]; l++)
	    for (m=0; m<dim[4]; m++)
	      for (n=0; n<dim[5]; n++)
		x[n][m][l][k][j][i] = i+j+k+l+m+n;
  } else {
    for (n=0; n<dim[5]; n++)
      for (m=0; m<dim[4]; m++)
	for (l=0; l<dim[3]; l++)
	  for (k=0; k<dim[2]; k++)
	    for (j=0; j<dim[1]; j++)
	      for (i=0; i<dim[0]; i++)
		x[n][m][l][k][j][i] = i+j+k+l+m+n;
  }
}

work7(int *dim,mdarray7 x, bool flip)
{
  int i,j,k,l,m,n,o;

  if (flip) {
    for (i=0; i<dim[0]; i++)
      for (j=0; j<dim[1]; j++)
	for (k=0; k<dim[2]; k++)
	  for (l=0; l<dim[3]; l++)
	    for (m=0; m<dim[4]; m++)
	      for (n=0; n<dim[5]; n++)
		for (o=0; o<dim[6]; o++)
		  x[o][n][m][l][k][j][i] = i+j+k+l+m+n+o;
  } else {
    for (o=0; o<dim[6]; o++)
      for (n=0; n<dim[5]; n++)
	for (m=0; m<dim[4]; m++)
	  for (l=0; l<dim[3]; l++)
	    for (k=0; k<dim[2]; k++)
	      for (j=0; j<dim[1]; j++)
		for (i=0; i<dim[0]; i++)
		  x[o][n][m][l][k][j][i] = i+j+k+l+m+n+o;
  }
}

#define MAXDIM 100

nemo_main()
{
  int i,dim[MAXDIM];
  int ndim = nemoinpi(getparam("dim"),dim,MAXDIM);
  bool flip = getbparam("flip"), free=getbparam("free");
  int iter=getiparam("iter");
  
  mdarray1 x1;
  mdarray2 x2;
  mdarray3 x3;
  mdarray4 x4;
  mdarray5 x5;
  mdarray6 x6;
  mdarray7 x7;

  printf("Working with ndim=%d MDArray",ndim);
  for (i=ndim-1; i>=0; i--)
    printf("[%d]",dim[i]);
  printf("\n");

  if (ndim==1) {
    x1 = allocate_mdarray1(dim[0]);
    while (iter-- > 0) {
      init1(dim,x1,flip);
      work1(dim,x1,flip);
    }
    if (free) free_mdarray1(x1,dim[0]);
  } else if (ndim==2) {
    x2 = allocate_mdarray2(dim[1],dim[0]);
    while (iter-- > 0) {
      init2(dim,x2,flip);
      work2(dim,x2,flip);
    }
    if (free) free_mdarray2(x2,dim[1],dim[0]);
  } else if (ndim==3) {
    x3 = allocate_mdarray3(dim[2],dim[1],dim[0]);
    while (iter-- > 0) {
      init3(dim,x3,flip);
      work3(dim,x3,flip);
    }
    if (free) free_mdarray3(x3,dim[2],dim[1],dim[0]);
  } else if (ndim==4) {
    x4 = allocate_mdarray4(dim[3],dim[2],dim[1],dim[0]);
    while (iter-- > 0) {
      init4(dim,x4,flip);
      work4(dim,x4,flip);
    }
    if (free) free_mdarray4(x4,dim[3],dim[2],dim[1],dim[0]);
  } else if (ndim==5) {
    x5 = allocate_mdarray5(dim[4],dim[3],dim[2],dim[1],dim[0]);
    work5(dim,x5,flip);
    if (free) free_mdarray5(x5,dim[4],dim[3],dim[2],dim[1],dim[0]);
  } else if (ndim==6) {
    x6 = allocate_mdarray6(dim[5],dim[4],dim[3],dim[2],dim[1],dim[0]);
    work6(dim,x6,flip);
    if (free) free_mdarray6(x6,dim[5],dim[4],dim[3],dim[2],dim[1],dim[0]);
  } else if (ndim==7) {
    x7 = allocate_mdarray7(dim[6],dim[5],dim[4],dim[3],dim[2],dim[1],dim[0]);
    work7(dim,x7,flip);
    if (free) free_mdarray7(x7,dim[6],dim[5],dim[4],dim[3],dim[2],dim[1],dim[0]);
  } else
    error("dimension %d not supported yet (MDMAXDIM=%d)",ndim,MDMAXDIM);
}


#endif
