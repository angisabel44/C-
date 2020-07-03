#!/bin/bash

FILES=gk/*
ALPHA=(0.25 0.2 0.1 0.01)
ITER=(32 62 128 256)

for f in $FILES; do
	echo "Corriendo instancia $(basename $f)"
	./MKP $f
done