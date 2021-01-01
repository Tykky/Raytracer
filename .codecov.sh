#!/bin/bash
for filename in `find . | egrep '\.cpp'`;
do
  gcov $filename;
done
