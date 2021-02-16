.section .data 

	#ifndef TEST
	#define TEST 9
	#endif
		.macro linea
		#if TEST==1
				.int 1,1,1,1
		#elif TEST==2
				.int 0x0fffffff,0x0fffffff,0x0fffffff,0x0fffffff
		#elif TEST==3
				.int 0x10000000,0x10000000,0x10000000,0x10000000
		#elif TEST==4
				.int 0xffffffff,0xffffffff,0xffffffff,0xffffffff
		#elif TEST==5
				.int -1,-1,-1,-1
		#elif TEST==6
				.int 200000000,200000000,200000000,200000000
		#elif TEST==7
				.int 300000000,300000000,300000000,300000000
		#elif TEST==8
				.int 5000000000,5000000000,5000000000,5000000000
		#else
				.error "Definir TEST entre 1..8"
		#endif
		.endm
lista: .irpc i,1234
		linea
.endr

# lista:		.int 1,2,10,  1,2,0b10,  1,2,0x10
longlista:	.int   (.-lista)/4
#resultado:	.quad   0
media:		.int	0
resto:		.int	0

#  formato:		.ascii "resultado \t=	%18lu (uns)\n"
# formato:		.ascii "resultado \t=	%18ld (sgn)\n"
#				.ascii "\t\t = 0x%18lx (hex)\n"
#				.asciz "\t\t = 0x %08x %08x \n"
formato:			.ascii "media \t= %11d \t resto \t = %11d	\n"
				.asciz	"\t = 0x %08x \t	\t = 0x %08x \n"


# opción: 1) no usar printf, 2)3) usar printf/fmt/exit, 4) usar tb main
# 1) as  suma.s -o suma.o
#    ld  suma.o -o suma					1232 B
# 2) as  suma.s -o suma.o				6520 B
#    ld  suma.o -o suma -lc -dynamic-linker /lib64/ld-linux-x86-64.so.2
# 3) gcc suma.s -o suma -no-pie –nostartfiles		6544 B
# 4) gcc suma.s -o suma	-no-pie				8664 B

.section .text
#_start: .global _start
 main: .global  main

	mov     $lista, %rbx
	mov  longlista, %ecx
	call suma		# == suma(&lista, longlista);
	#mov  %eax, resultado
	#mov  %edx, resultado + 4
	mov  %eax, media
	mov  %edx, resto

	mov   %edx,     %ecx	# Imprimir eax
	mov   %eax,     %r8d    # Imprimir edx

	#requiere libC
	mov   $formato, %rdi
	#mov   resultado,%rsi
	#mov   resultado,%rdx

	mov	  media, %esi
	mov   resto, %edx
	mov   media, %ecx
	mov   resto, %r8d

	mov   $0,       %eax
	call printf

	#requiere libC

	mov $0,         %edi
	call _exit 

suma:
	mov  $0, %edi
	mov  $0, %ebp
	mov  $0, %rsi

bucle:
	mov (%rbx,%rsi,4),%eax
	cltd
	add %eax, %edi
	adc %edx, %ebp	# sumamos LBB

	inc  %rsi
	cmp  %rsi,%rcx
	jne  bucle

	mov %edi, %eax
	mov %ebp, %edx

	ret
