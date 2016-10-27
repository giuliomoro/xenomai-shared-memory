#!/bin/bash
gcc -I. shm.c `xeno-config --skin posix --cflags` `xeno-config --skin posix --ldflags`  `xeno-config --skin native --ldflags` -o shm
