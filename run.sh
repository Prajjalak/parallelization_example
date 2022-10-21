#!/bin/bash

threads=4

echo "#################################"
echo "#### Parallalization Example ####"
echo "#################################"
echo "########## Series Sum ###########"
echo "#################################"

mpicc add.c -o add.out
mpicc mtrx.c -o mtrx.out

echo " "
echo "------------------"
echo "Sequential summing"
echo "------------------"
mpiexec -n 1 add.out

echo " "
echo "------------------------------------------------"
echo "Parallel summing :: Number of threads = $threads"
echo "------------------------------------------------"
mpiexec -n $threads add.out

echo " "
echo "#################################"
echo "##### Matrix Multiplication #####"
echo "#################################"

echo " "
echo "-------------------------"
echo "Sequential Multiplication"
echo "-------------------------"
mpiexec -n 2 mtrx.out

echo " "
echo "---------------------------------------------------"
echo "Sequential Multiplication :: Number of threads = $threads"
echo "---------------------------------------------------"
mpiexec -n $threads mtrx.out
