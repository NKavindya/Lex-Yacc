#ifndef MACHINE_CODE_H
#define MACHINE_CODE_H

#include <stdio.h>

/* Generate relocatable machine code from assembly file */
int generate_relocatable_code(const char *asmPath, const char *outPath);

/* Generate absolute machine code from relocatable code */
int generate_absolute_code(const char *relocPath, const char *outPath, int baseAddress);

#endif

