#!/bin/bash

for i in src/*.cc src/*.h src/players/*.cc src/players/*.h
do
	astyle -n --style='k&r' "$i";
	astyle -n --indent=tab "$i";

done;
