.section .data
lista:		.int -1,-1, -4, -4, -7, -8
longlista:	.int   (.-lista)/4
resultado:	.quad  0
  formato2:	.string "media = %i = 0x%08x\n"
  formato3:	.string "resto = %i = 0x%08x\n"

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

	call trabajar	# subrutina de usuario
#	call imprim_C	# printf()  de libC
#	call acabar_L	# exit()   del kernel Linux
	call acabar_C	# exit()    de libC
	ret

trabajar:
	mov     $lista, %rdi
	mov  longlista, %esi
	call media		# == suma(&lista, longlista);

	mov $formato2, %rdi
	mov %eax, %esi
	mov %eax, %edx
	xor %eax, %eax
	call printf

	mov     $lista, %rdi
	mov  longlista, %esi
	call resto


	mov $formato3, %rdi
    mov %eax, %esi
    mov %eax, %edx
    xor %eax, %eax
    call printf

	ret
media:
	push %rsi
	call suma 
	idiv %esi
	pop %rsi
	ret

suma:
	#push     %rsi
	xor %eax, %eax
    xor %ecx, %ecx
    xor %edx, %edx
bucle:
	movslq (%rdi,%rcx,4), %r8  # almacenamos el valor en r8
	add   %r8, %rax	# sumamos el valor en rax
	inc   %rcx
	cmp   %rcx,%rsi
	jne    bucle
	mov   %rax, %rdx # copiamos en rdx
	shr   $32, %rdx  # hacemos un shift del bit mas significativo guardando el signo

	#pop   %rsi
	ret
resto:
	push %rsi
	call suma 
	idiv %esi
	mov %edx, %eax
	pop %rsi
	ret

#imprim_C:			# requiere libC
#	mov   $formato, %rdi
#	mov   resultado,%rsi
#	mov   resultado,%rdx
#	mov          $0,%eax	# varargin sin xmm
#	call  printf		# == printf(formato, res, res);
#	ret

#acabar_L:
#	mov        $60, %rax
#	mov  resultado, %edi
#	syscall			# == _exit(resultado)
#	ret

acabar_C:			# requiere libC
	mov  resultado, %rdi
	call _exit		# ==  exit(resultado)
	ret
