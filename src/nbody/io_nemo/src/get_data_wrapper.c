/* -------------------------------------------------------------- *\
|* $Id$
|*
|* Wrapper of basic NEMO procedure, it makes me life easiest :)
|*
\* -------------------------------------------------------------- */

/* -------------------------------------------------------------- *\
|* Include files
\* -------------------------------------------------------------- */
#include <stdinc.h>
#include <getparam.h>
#include <vectmath.h>		
#include <filestruct.h>

#include <snapshot/snapshot.h>	
#include <snapshot/body.h>


/* -------------------------------------------------------------- *\ 
|* get_data_gen :
|* 
\* -------------------------------------------------------------- */
int get_data_gen(stream instr, char * TypeTag,char * DataType,
		 int size_alloc, int nbody, int dim1,int dim2,void ** genptr)
{
  if (*genptr == NULL)
    *genptr = ( void **) allocate(size_alloc);
  get_data_coerced(instr, TypeTag, DataType,*genptr, nbody,
                   dim1,dim2,0);
  return 0;
}
/* -------------------------------------------------------------- *\ 
|* get_data_time :
|* Get Time from a Nemo snapshot
\* -------------------------------------------------------------- */
int get_data_time(stream instr, char * DataType, int size_type,
		  void ** timeptr)
{
  int status;

  if (get_tag_ok(instr, TimeTag))
    { 
      if (*timeptr == NULL)
	*timeptr = (void **) allocate(size_type);
      get_data_coerced(instr, TimeTag, DataType, *timeptr,0);
      status = 1; 
    }
  else
    status = 0;
  return status;
}
/* -------------------------------------------------------------- *\ 
|* get_data_nbody :
|* Get Nbody from aNEMO snapshot
\* -------------------------------------------------------------- */
int get_data_nbody(stream instr, char * DataType, int size_type, void ** nbodyptr)
{
  int status;

  if (get_tag_ok(instr, NobjTag))
    { 
      if (*nbodyptr == NULL)
	*nbodyptr = (void **) allocate(size_type);
      get_data_coerced(instr, NobjTag, DataType, *nbodyptr,0);
      status = 1; 
    }
  else
    status = 0;
  return status;
}
/* -------------------------------------------------------------- *\ 
|* get_data_mass :
|* Get Masses from a NEMO snapshot
\* -------------------------------------------------------------- */
int get_data_mass(stream instr, char * DataType, int nbody, int size_type,
		  void ** massptr)
{
  int status;

  if (get_tag_ok(instr, MassTag))
    { 
      if (*massptr == NULL)
	*massptr = (void **) allocate(size_type * nbody);
      get_data_coerced(instr, MassTag, DataType, *massptr,
		       nbody, 0);
      status = 1; 
    }
  else
    status = 0;
  return status;
}
/* -------------------------------------------------------------- *\ 
|* get_data_pos :
|* Get Positions from a NEMO snapshot
\* -------------------------------------------------------------- */
int get_data_pos(stream instr, char * DataType, int nbody, int size_type,
		  void ** posptr, int ndim)
{
  int status;

  if (get_tag_ok(instr, PosTag))
    { 
      if (*posptr == NULL) {
	*posptr = (void **) allocate(size_type * nbody *ndim);
      }
      get_data_coerced(instr, PosTag, DataType, *posptr,
		       nbody,ndim,0);
      status = 1; 
    }
  else
    status = 0;
  return status;
}
/* -------------------------------------------------------------- *\ 
|* get_data_vel :
|* Get Velocities from a NEMO snapshot
\* -------------------------------------------------------------- */
int get_data_vel(stream instr, char * DataType, int nbody, int size_type,
		  void ** velptr, int ndim)
{
  int status;

  if (get_tag_ok(instr, VelTag))
    { 
      if (*velptr == NULL)
	*velptr = (void **) allocate(size_type * nbody*ndim);
      get_data_coerced(instr, VelTag, DataType, *velptr,
		       nbody,ndim, 0);
      status = 1; 
    }
  else
    status = 0;
  return status;
}
/* -------------------------------------------------------------- *\ 
|* get_data_phase :
|* Get Phase Space coordinates from a NEMO snapshot
\* -------------------------------------------------------------- */
int get_data_phase(stream instr, char * DataType, int nbody, 
		   int size_type, void ** phaseptr, int ndim)
{
  int status;

  if (get_tag_ok(instr, PhaseSpaceTag))
    { 
      if (*phaseptr == NULL)
	*phaseptr = (void **) allocate(size_type*nbody*2*ndim);

      get_data_coerced(instr, PhaseSpaceTag, DataType, *phaseptr,
		       nbody,2,ndim,0);

      status = 1; 
    }
  else
    status = 0;
  return status;
}


/* -------------------------------------------------------------- *\ 
|* get_data_pot :
|* Get Potential from a NEMO snapshot
\* -------------------------------------------------------------- */
int get_data_pot(stream instr, char * DataType, int nbody, int size_type,
		 void ** potptr)
{
  int status;

  if (get_tag_ok(instr, PotentialTag))
    { 
      if (*potptr == NULL)
	*potptr = (void **) allocate(size_type * nbody);
      get_data_coerced(instr, PotentialTag, DataType, *potptr,
		       nbody, 0);
      status = 1; 
    }
  else
    status = 0;
  return status;
}
/* -------------------------------------------------------------- *\ 
|* get_data_acc :
|* Get Acceleration from a NEMO snapshot
\* -------------------------------------------------------------- */
int get_data_acc(stream instr, char * DataType, int nbody, int size_type,
		 void ** accptr, int ndim)
{
  int status;

  if (get_tag_ok(instr, AccelerationTag))
    { 
      if (*accptr == NULL)
	*accptr = (void **) allocate(size_type * nbody*ndim);
      get_data_coerced(instr, AccelerationTag, DataType, *accptr,
		       nbody,ndim, 0);
      status = 1; 
    }
  else
    status = 0;
  return status;
}
/* -------------------------------------------------------------- *\ 
|* get_data_keys :
|* Get Keys from a NEMO snapshot
\* -------------------------------------------------------------- */
int get_data_keys(stream instr, char * DataType, int nbody, int size_type,
		  void ** keysptr)
{
  int status;

  if (get_tag_ok(instr, KeyTag))
    { 
      if (*keysptr == NULL)
	*keysptr = (void **) allocate(size_type * nbody);
      get_data_coerced(instr, KeyTag, DataType, *keysptr,
		       nbody, 0);
      status = 1; 
    }
  else
    status = 0;
  return status;
}
/* -------------------------------------------------------------- *\ 
|* End of [get_dat_nemo.c]
\* -------------------------------------------------------------- */ 
