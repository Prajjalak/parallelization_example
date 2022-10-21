#!/bin/bash
#PBS -q small
#PBS -N parallelization_test
#PBS -l select=1:ncpus=32:mem=64gb
#PBS -o output
#PBS -e error
ncpus=32
# You must explicitly change to your working directory in PBS
# The PBS_O_WORDIR variable is automatically filled with the path
# to the directory you submit your job from

cd $PBS_O_WORKDIR
echo ""

# The PBS_NODEFILE file contains the compute nodes assigned
# to the job by PBS.  Uncommenting the next line will show them.

cat $PBS_NODEFILE
echo ""

module load mpi/mpich-3.0-x86_64
mpicc -o add.out add.c 
mpicc -o mtrx.out mtrx.c

echo "#################################"
echo "#### Parallalization Example ####"
echo "#################################"
echo "########## Series Sum ###########"
echo "#################################"

echo " "
echo "------------------"
echo "Sequential summing"
echo "------------------"
mpiexec -n 1 ./add.out

echo " "
echo "------------------------------------------------"
echo "Parallel summing :: Number of threads = $ncpus"
echo "------------------------------------------------"
mpiexec -n $ncpus ./add.out

echo " "
echo "#################################"
echo "##### Matrix Multiplication #####"
echo "#################################"

echo " "
echo "-------------------------"
echo "Sequential Multiplication"
echo "-------------------------"
mpiexec -n 2 ./mtrx.out

echo " "
echo "---------------------------------------------------"
echo "Sequential Multiplication :: Number of threads = $ncpus"
echo "---------------------------------------------------"
mpiexec -n $ncpus ./mtrx.out
