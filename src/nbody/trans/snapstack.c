/*
 * SNAPSTACK: stack two N-body systems on top of each other.
 *
 *	22-jan-89  1.1 JEB
 *	 1-jul-90  1.1a added helpvec PJT
 *	 4-feb-93  1.1b nemo_main (also to return 0 exit)  -- PJT
 *			malloc -> allocate
 *	mar94 - ansi
 *	 6-aug-96  1.1d printf -> dprintf
 *	30-dec-97  1.1e ansi 
 *      20-jun-03  1.2  using modern get_snap and put_snap
 */
#include <stdinc.h>
#include <getparam.h>
#include <vectmath.h>
#include <filestruct.h>

#include <snapshot/snapshot.h>
#include <snapshot/body.h>
#include <snapshot/get_snap.c>
#include <snapshot/put_snap.c>


string defv[] = {		/* DEFAULT INPUT PARAMETERS */
    "in1=???\n			  input file name ",
    "in2=???\n			  input file name ",
    "out=???\n			  output file name ",
    "deltar=0.0,0.0,0.0\n	  position of in1 wrt in2 ",
    "deltav=0.0,0.0,0.0\n	  velocity of in1 wrt in2 ",
    "zerocm=true\n		  zero center of mass ",
    "headline=\n		  random verbiage ",
    "VERSION=1.2\n		  20-jun-03 PJT",
    NULL,
};

string usage="stack two N-body systems on top of each other";

extern string *burststring(string,string);

nemo_main()
{
    readdata();
    snapstack();
    writedata();
}

int nbody, nbody1, nbody2;
int bits,  bits1,  bits2;
Body *btab, *btab1, *btab2;
real tsnap, tsnap1, tsnap2;

readdata()
{
    stream instr1, instr2;

    instr1 = stropen(getparam("in1"), "r");
    get_history(instr1);
    instr2 = stropen(getparam("in2"), "r");
    get_history(instr2);

    get_snap(instr1, &btab1, &nbody1, &tsnap, &bits1);
    get_snap(instr2, &btab2, &nbody2, &tsnap, &bits2);

    dprintf(1,"nbody1 = %d    nbody2 = %d\n", nbody1, nbody2);
}

snapstack()
{
    vector deltar, deltav;
    Body *bp;

    setvect(deltar, getparam("deltar"));
    setvect(deltav, getparam("deltav"));

    btab1 = (body *) reallocate(btab1, sizeof(body)*(nbody1+nbody2));
    btab = btab1;
    nbody = nbody1 + nbody2;
    tsnap = tsnap1;
    bits = (bits1 & bits2);
    memcpy(&btab[nbody1],btab2,sizeof(body)*nbody2);
    for (bp=&btab[nbody1]; bp<btab+nbody; bp++) {
      ADDV(Pos(bp),Pos(bp),deltar);
      ADDV(Vel(bp),Vel(bp),deltav);
    }
    if (getbparam("zerocm")) snapcenter();
}

setvect(vec, str)
vector vec;
string str;
{
    string *vcp;
    int i;

    vcp = burststring(str, ", ");
    for (i = 0; i < NDIM; i++)
	vec[i] = (*vcp != NULL ? atof(*vcp++) : 0.0);
}

writedata()
{
    stream outstr;
    int cscode = CSCode(Cartesian, NDIM, 2);

    if (! streq(getparam("headline"), ""))
	set_headline(getparam("headline"));
    outstr = stropen(getparam("out"), "w");
    put_history(outstr);
    put_snap(outstr, &btab, &nbody, &tsnap, &bits);
}

snapcenter() 
{
    real w_i, w_sum;
    vector tmpv, w_pos, w_vel;
    Body *bp;

    w_sum = 0.0;
    CLRV(w_pos);
    CLRV(w_vel);
    for (bp = btab; bp < btab+nbody;  bp++) {
      w_i = Mass(bp);
      w_sum += w_i;
      MULVS(tmpv, Pos(bp), w_i);
      ADDV(w_pos, w_pos, tmpv);
      MULVS(tmpv, Vel(bp), w_i);
      ADDV(w_vel, w_vel, tmpv);
    }
    SDIVVS(w_pos, w_sum);
    SDIVVS(w_vel, w_sum);
 
    for (bp = btab; bp < btab+nbody; bp++) {
        SSUBV(Pos(bp), w_pos);
        SSUBV(Vel(bp), w_vel);
    }
}
