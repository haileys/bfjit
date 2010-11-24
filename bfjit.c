#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

#define BF_MEM_SIZE 2048

int main()
{	
	unsigned char* mem = (unsigned char*)malloc(BF_MEM_SIZE);
	memset(mem, 0, BF_MEM_SIZE);
	
	const char* bf = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";
	
	compile_result compiled = compile(bf, mem);
	
	FILE* f = fopen("output.bin", "w");
	fwrite(compiled.code.buff, compiled.length, 1, f);
	fclose(f);
	
	compiled.code.execute();
	return 0;
}