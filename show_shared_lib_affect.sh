#!/bin/bash
echo "no shared lib"
gcc -static hello.c
ls -l a.out
size a.out
echo "use shared lib"
gcc hello.c
ls -l a.out
size a.out

echo "you can see the size have been decreased more when using shared lib"
