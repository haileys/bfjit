#ifndef COMPILER_H
#define COMPILER_H

typedef struct compile_result {
	size_t length;
	union { 
		char* buff;
		void(*execute)();
	} code;
} compile_result;

compile_result compile(const char* bf, void* mem);

#endif