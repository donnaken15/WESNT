
format PE console 3.1

entry main

include 'win32a.inc'

 ; 1 KILOBYTE EXE
 
 ; also major post assembling
 ; header hacking

 section '' import code data readable writeable executable
  library msvcrt,'msvcrt.dll'
  import msvcrt,\
         printf,'printf'
  include 'api/kernel32.inc'

  align 10h
  main:

   invoke printf, str1
   pop ebp

   jmp main

   ret
  str1 db 'Test',$A,0

  ; todo: attempt to get 0x100 alignment working
  ; and make it work on w9x for whatever reason
  ;
  ; appease the demoscene
