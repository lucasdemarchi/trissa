#!/bin/bash

for i in src/*.cc src/*.h src/players/*.cc src/players/*.h
do
	astyle -n --style=linux "$i";
	astyle -n --indent=spaces=4 "$i";
done;
