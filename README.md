<div align=center>
  <h1>waduzitdo compiler</h1>
  <img src=https://img.shields.io/badge/Language:-C-white />
  <img src=https://img.shields.io/badge/Language:-waduzitdo-yellow />
  <img src=https://img.shields.io/badge/System:-GNU/Linux-blue />
</div>

### Description
**Waduzitdo** is an esoteric programming language, invented initially by **Larry Kheriati**, a contributor of the **BYTE** magazine, in 1978. It was inspired by [PILOT](https://en.wikipedia.org/wiki/PILOT), a programming language designed to help people learn to code.
Unlike PILOT, however, it in **not** Turing complete. It is a **finite state transducer** - something like [DFA](https://en.wikipedia.org/wiki/Deterministic_finite_automaton) except it changes state based on finite tape of inputs instead of just one input *([wiki](https://en.wikipedia.org/wiki/Finite-state_transducer))*.
It sadly wasn't very popular and remained pretty niche, though. Visit its [esolang wiki](https://esolangs.org/wiki/Waduzitdo) page for more info. You can find the language documentation there, too. My compiler is written in C on Linux and compiles to NASM.

### Compiletime dependencies
To compile the program, you need
 - **gnu make**
 - **gcc**
 - **C standard library** (tested with glibc)

To check if file exists, the program also uses the `unistd.h` header, which is UNIX exclusive. I have also added a preprocessor directive to include appropriate Windows library, however since I dont have a Windows machine available at this point, it hasn't been tested.

### Runtime dependencies
There are essentially none (apart from working terminal), however if you want to somehow use the output, you need the **[Netwide assembler](https://nasm.us/) (NASM)** and a proper **linker** *(GNU ld will do the trick)*. Also, I have provided a shell script, the `comp.sh` to fasten the process of actually generating
binaries, that uses the **BASH** shell.

### Compilation instruction
Clone the repo
```
git clone https://github.com/lasermtv07/waduzitdo-compiler/
```
and run the makefile
```
make
```
Then, optionally, add the program (`waduzitdo-comp`) to path or copy it to `/dev/usr/bin`, which is a folder that is defaultly in PATH.

### Usage
#### the binary
The binary has really only two options, though I tried to make it usable even if both arguments are not specified:
- `-i` which specifies the next argument as a compilation source file
- `-o` which specifies the next instruction as output target

#### the script
The `comp.h` script takes `.wz` file as an argument, compiles to assembly, saves the output to a temporary file in `/tmp`, wheree it also assembles and links the program and copies the binary to the location where the compilation was started.

---
*(c) lasermtv07, 2024 released under UNLICENSE.*
