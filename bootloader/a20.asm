bits 16

%include "const.inc"

enable_a20:
  call check_a20
  jne a20_on

  call a20_try_int15
  call check_a20
  jne a20_on

  call a20_try_kbd
  call check_a20
  jne a20_on

  jmp _die
a20_on:
  ret

check_a20:
  mov edi, (0x100000 + scratch)
  mov esi, scratch
  mov [esi], esi
  mov [edi], edi
  mov eax, [esi]
  mov ebx, [edi]
  cmp eax, ebx
  ret

a20_try_int15:
  mov ax, 0x2401
  int 0x15
  ret

a20_try_kbd:
  cli

  call a20wait
  mov al, 0xad
  out 0x64, al

  call a20wait
  mov al, 0xd0
  out 0x64, al

  call a20wait2
  in al,0x60
  push eax

  call a20wait
  mov al, 0xd1
  out 0x64, al

  call a20wait
  pop eax
  or al, 2
  out 0x60, al

  call a20wait
  mov al, 0xae
  out 0x64, al

  call a20wait
  sti
  ret

a20wait:
  in al, 0x64
  test al, 2
  jnz a20wait
  ret

a20wait2:
  in al, 0x64
  test al, 1
  jz a20wait2
  ret