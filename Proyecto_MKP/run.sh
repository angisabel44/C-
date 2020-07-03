#!/bin/bash

FILES=All-MKP-Instances/*
ALPHA=(0.25 0.2 0.1 0.01)
ITER=(32 62 128 256)

for f in $FILES
do
	ARCHIVOS=$f/*
	for i in $ARCHIVOS
	do
		echo "Carpeta $(basename $i)"
		INSTANCIAS=$i/*
		for j in $INSTANCIAS
		do
			echo "Corriendo instancia $(basename $j)"
			./MKP $j
		done
	done
done