#Running singleCell With Sensei Instructions (on Cooley)

1. Sensei must be properly built on your Cooley Account.

2. `EMB_PATH` in `BloodFlow/examples/singleCell/singleCell.sh` needs to be changed to your correct path.

3. Go to `Bloodflow/rbc` and use the following commands: `cp bond_wlc_pow.* ../../lammps/src` \ `angle_rbc.* ../../lammps/src` \ `dihedral_bend.* ../../lammps/src`

4. Make a build directory while in `BloodFlow/examples/singleCell`: `mkdir build`

5. While in the newly created build directory, run the following command (install directory is created when building Sensei): `cmake -DSENSEI_DIR=/path/to/install/lib/cmake/ -C /path/to/BloodFlow/sites/cooley.cmake ../`\
   Next run: `make -j8`

6. Return to `BloodFlow/examples/singleCell` and run: `qsub -n 1 -t 10 -q debug ./singleCell.sh`

PVD Note: 
	Connor and I were able to get both the fluid.pvd and cells.pvd working yesterday. Today I went to clone the repo and run it on my own computer and now I am only getting the cells.pvd. Although, I am still getting all the vti files from the fluid meshes. Connor also noted that he tried to run it this morning and ran into the same issue. Victor advised removing the fluid meshes from the xml file and run it with only the cells. The simulation still outputed the cells.pvd and all of its vtp files. 
