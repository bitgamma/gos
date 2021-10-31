bits 32

section .text

global memset

memset:
  cld
  mov ecx, [esp + 12]
  mov eax, [esp + 8]
  mov edx, edi
  mov edi, [esp + 4]
  rep stosb
  mov edi, edx
  ret