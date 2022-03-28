
format PE console 3.1

entry main
       ; v--------------v why NppExec
include 'c:\fasm\include\win32a.inc'

section '' import code data readable writeable executable
	library msvcrt,'msvcrt.dll'
	import msvcrt,\
		printf,'printf'

	mcand dd 250
	multi dd 5

	align 10h
	main:
		mov eax, [mcand]
		mov edx, [multi]
		call fastmult

		invoke printf, str1, [mcand], [multi], eax
		;pop ebp (*4)
		add esp, (4*4)
								; can i make a macro where this is done after
								; without writing it every time
								; ive still heard how sometimes stack
								; moving is backwards in some cases
								; so i dont know if i should stick with add
		ret

	; uint __attribute__(( regparm(2) )) fastmult(uint a, uint d)
	fastmult:
								; args in EAX,EDX
								; EAX = multiplicand, EDX = multiplier,
								;  CL = bit index,	  EBX = value

		cmp  eax, edx
								; make the lower number the multiplier
								; makes this easier to write
		ja   fm_hilo_ord		; if eax > edx

	;	fm_lohi_ord:
		xchg eax, edx			; edx <-> eax

		fm_hilo_ord:
		test edx, edx			; if multi == 0
		jnz  fm_nonzero

								; this func actually doesn't need
								; this zero check but at least
								; the main part wouldn't
								; execute for nothing

	;	fm_gotzero:
		xor  eax, eax			; ret 0
		ret

		fm_nonzero:
			bsr	 ecx, edx		; set counter based on MSB
			xor	 ebx, ebx		; ebx = 0

			push ebp			; need another reg...

			fm_main1:
				bt	edx, ecx	; do i need to use the whole reg?
								; does this computer support 4294967295-bit integers??
								; (trollface)
				jnc	fm_main2	; if d & (1<<c)

				mov	ebp, eax
				shl	ebp,  cl	; ebp * (1<<cl)
								; OH NOW I NEED A BYTE
				add	ebx, ebp	; ebx += ebp
				fm_main2:
				dec	 cl			; cl--
				cmp	 cl, $FF	; if (cl > -1)
				jne	fm_main1

			pop	 ebp

		mov  eax, ebx			; return value
		ret

	str1 db '"Fast" multiply: %u*%u=%u',10,0

