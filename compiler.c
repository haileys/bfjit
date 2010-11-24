#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "compiler.h"

compile_result compile(const char* bf, void* mem)
{
	char* exec = (char*)malloc(65536);
	char* exec_ptr = exec;
	size_t len = strlen(bf);
	
	int mem_addr		= (int)mem;
	int putchar_addr	= (int)putchar;
	int getchar_addr	= (int)getchar;
	
	int* jmp_stack = (int*)malloc(sizeof(int)*1024);
	
	*exec++ = 0x55; // push ebp
	*exec++ = 0x89; // mov ebp, esp
	*exec++ = 0xE5;
	*exec++ = 0x81; // and esp, -16
	*exec++ = 0xE4;
	*exec++ = 0xF0; // =>  -16
	*exec++ = 0xFF; // 
	*exec++ = 0xFF; // 
	*exec++ = 0xFF; // 
	*exec++ = 0x81; // sub esp, 16
	*exec++ = 0xEC; //
	*exec++ = 0x10; // =>  16
	*exec++ = 0x00; //
	*exec++ = 0x00; //
	*exec++ = 0x00; //
	
	*exec++ = 0xBB; // mov ebx
	*(int*)exec = mem_addr;
	exec += 4;
	
	for(int i = 0; i < len; i++)
	{
		switch(bf[i])
		{
		case '>':
			*exec++ = 0x43; // inc ebx
			break;
		case '<':
			*exec++ = 0x4B; // dec ebx
			break;
		case '+':
			*exec++ = 0x8A; // mov al, [ebx]
			*exec++ = 0x03; // 
			*exec++ = 0xFE; // inc al
			*exec++ = 0xC0; // 
			*exec++ = 0x88; // mov [ebx], al
			*exec++ = 0x03; // 
			break;
		case '-':
			*exec++ = 0x8A; // mov al, [ebx]
			*exec++ = 0x03; // 
			*exec++ = 0xFE; // dec al
			*exec++ = 0xC8; // 
			*exec++ = 0x88; // mov [ebx], al
			*exec++ = 0x03; // 
			break;
		case '[':
			*jmp_stack++ = (int)exec; // store location of current thingy to be filled in later.
			*exec++ = 0x8A; // mov al, [ebx]
			*exec++ = 0x03; // 
			*exec++ = 0xA8; // TEST al, 0xFF
			*exec++ = 0xFF; //
			*exec++ = 0x0F; // JZ ?
			*exec++ = 0x84;
			*exec++ = 0x00; // ?
			*exec++ = 0x00; // ?
			*exec++ = 0x00; // ?
			*exec++ = 0x00; // ?
			break;
		case ']':
			--jmp_stack;
			int jmp_to = *jmp_stack--;
			*exec++ = 0xB9; // mov ecx, jmp_to
			*(int*)exec = jmp_to;
			exec += 4;
			*exec++ = 0xFF; // jmp ecx
			*exec++ = 0xE1; //
			// fill in current exec location offset to matching bracket
			int offset = (((int)exec)-jmp_to)-10;
			*(int*)(jmp_to+6) = offset;
			break;
		case '.':
			*exec++ = 0x8B; // mov eax, [ebx]
			*exec++ = 0x03; // 
			
			*exec++ = 0x89; // mov [esp], eax
			*exec++ = 0x04; // 
			*exec++ = 0x24; // 
			
			*exec++ = 0xB9; // mov ecx putchar_addr
			*(int*)exec = putchar_addr;
			exec += 4;
			*exec++ = 0xFF; // call ecx
			*exec++ = 0xD1; //
				
			break;
		case ',':			
			*exec++ = 0xB9; // mov ecx putchar_addr
			*(int*)exec = getchar_addr;
			exec += 4;
			*exec++ = 0xFF; // call ecx
			*exec++ = 0xD1; //
			*exec++ = 0x88; // mov [ebx], al
			*exec++ = 0x03; // 
				
			break;
		}
	}
		
	*exec++ = 0xC9; // leave
	*exec++ = 0xC3; // ret
	
	compile_result ret;
	ret.length = exec - exec_ptr;
	ret.code.buff = exec_ptr;
	
	return ret;
}