default rel
section .text
global dot_asm

dot_asm:                        ; rcx=n, rdx=A, r8=B, r9=&sdot
    xor      rax, rax           ; i = 0
    cvtsi2sd xmm0, rax          ; sum = 0.0
    test     rcx, rcx
    jz       .done
.loop:
    movsd    xmm1, [rdx + rax*8]    ; A[i]
    mulsd    xmm1, [r8 + rax*8]     ; * B[i]
    addsd    xmm0, xmm1             ; sum += ...
    inc      rax
    cmp      rax, rcx
    jl       .loop
.done:
    movsd    [r9], xmm0
    ret