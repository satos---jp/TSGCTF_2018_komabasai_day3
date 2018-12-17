BITS 64


section .note.GNU-stack
; prevent executable stack


; The maximum length of an Intel 64 and IA-32 instruction remains 15 bytes.

section .payload
payload:
	pop rsi
	ret

section .bss
mystack:
	resb 0x1000

section .text
global _start

_start:
	mov rsp,mystack
	mov rdx,0x100
	mov rsi,rsp
	mov rdi,0
	mov rax,0
	syscall
	mov rcx,0x8
loop:
	cmp rcx,0x1000
	je loop_end
	mov qword [payload+rcx],0
	add rcx,0x8
	jmp loop
loop_end:
	mov byte [payload+1],0xc3
	mov word [payload+2],0
	mov dword [payload+4],0
	ret


; nasm prob.s -f elf64 -g -o out.o
; objcopy out.o --set-section-flags .payload=data,code,alloc
; ld out.o -o a.out --section-start .payload=402000 --section-start .bss=404000
; ./rp-lin-x64 --file=a.out --rop=3 --unique

; ./rp-lin-x64 --file=a.out --rop=3 --unique --raw=x64

; read の長さにより、二手目で任意のsyscallが読み出せる
; 0 read
; 1 write
; 2 open
; 11 execve
; 20 getpid
; 63  dup2
