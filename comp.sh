#!/bin/bash
./waduzitdo-comp $1 -o /tmp/temp.asm
nasm -f elf /tmp/temp.asm -o /tmp/aaa.obj
ld /tmp/aaa.obj -o $(echo $1 | sed 's/\.wz//' | sed 's/^.*\///') -m elf_i386

