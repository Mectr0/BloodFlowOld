/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "fix_rls.h"
#include "atom.h"
#include "update.h"
#include "memory.h"
#include "error.h"
#include "force.h"

using namespace LAMMPS_NS;
using namespace FixConst;

//enum{PF_CALLBACK,PF_ARRAY};

/* ---------------------------------------------------------------------- */

FixRLS::FixRLS(LAMMPS *lmp, int narg, char **arg) :
  Fix(lmp, narg, arg)
{
  if (narg < 4) error->all(FLERR,"Illegal fix RLS command");
  
    // perform initial allocation of atom-based array
  // register with Atom class
  //napply = force->inumeric(FLERR,arg[3]);
  //rate = force->inumeric(FLERR,arg[3]);
  rate = force->numeric(FLERR,arg[3]);
  
  fexternal = NULL;
  grow_arrays(atom->nmax);
  atom->add_callback(0);
  
}

/* ---------------------------------------------------------------------- */

FixRLS::~FixRLS()
{
  // unregister callbacks to this fix from Atom class

  atom->delete_callback(id,0);
  memory->destroy(fexternal);
}

/* ---------------------------------------------------------------------- */

int FixRLS::setmask()
{
  int mask = 0;
  mask |= POST_FORCE;
  mask |= MIN_POST_FORCE;
  mask |= END_OF_STEP;
  return mask;
}

/* ---------------------------------------------------------------------- */

void FixRLS::init()
{
  int *mask = atom->mask;
  int nlocal = atom->nlocal;
  for (int i = 0; i < nlocal; i++)
      if (mask[i] & groupbit) {
        fexternal[i][0]=0.;
        fexternal[i][1]=0.;
        fexternal[i][2]=0.;
      }
}

/* ---------------------------------------------------------------------- */

void FixRLS::end_of_step()
{/*
  int *mask = atom->mask;
  int nlocal = atom->nlocal;
  for (int i = 0; i < nlocal; i++)
      if (mask[i] & groupbit) {
        fexternal[i][0]=0.;
        fexternal[i][1]=0.;
        fexternal[i][2]=0.;
      }*/
}
/* ---------------------------------------------------------------------- */

void FixRLS::setup(int vflag)
{
  post_force(vflag);
}

/* ---------------------------------------------------------------------- */

void FixRLS::min_setup(int vflag)
{
  post_force(vflag);
}

/* ---------------------------------------------------------------------- */

void FixRLS::post_force(int vflag)
{
  bigint ntimestep = update->ntimestep;

  // invoke the callback in driver program
  // it will fill fexternal with forces


  // add forces from fexternal to atoms in group
/*
  if (ntimestep % napply == 0) {
    double **f = atom->f;
    int *mask = atom->mask;
    int nlocal = atom->nlocal;

    for (int i = 0; i < nlocal; i++)
      if (mask[i] & groupbit) {
        f[i][0] += fexternal[i][0];
        f[i][1] += fexternal[i][1];
        f[i][2] += fexternal[i][2];
      }
  }*/
}

/* ---------------------------------------------------------------------- */

void FixRLS::min_post_force(int vflag)
{
  post_force(vflag);
}



/* ----------------------------------------------------------------------
   memory usage of local atom-based array
------------------------------------------------------------------------- */

double FixRLS::memory_usage()
{
  double bytes = 3*atom->nmax * sizeof(double);
  return bytes;
}

/* ----------------------------------------------------------------------
   allocate atom-based array
------------------------------------------------------------------------- */

void FixRLS::grow_arrays(int nmax)
{
  memory->grow(fexternal,nmax,3,"rls:fexternall");
}

/* ----------------------------------------------------------------------
   copy values within local atom-based array
------------------------------------------------------------------------- */

void FixRLS::copy_arrays(int i, int j, int delflag)
{
  fexternal[j][0] = fexternal[i][0];
  fexternal[j][1] = fexternal[i][1];
  fexternal[j][2] = fexternal[i][2];
}

/* ----------------------------------------------------------------------
   pack values in local atom-based array for exchange with another proc
------------------------------------------------------------------------- */

int FixRLS::pack_exchange(int i, double *buf)
{
  buf[0] = fexternal[i][0];
  buf[1] = fexternal[i][1];
  buf[2] = fexternal[i][2];
  return 3;
}

/* ----------------------------------------------------------------------
   unpack values in local atom-based array from exchange with another proc
------------------------------------------------------------------------- */

int FixRLS::unpack_exchange(int nlocal, double *buf)
{
  fexternal[nlocal][0] = buf[0];
  fexternal[nlocal][1] = buf[1];
  fexternal[nlocal][2] = buf[2];
  return 3;
}


