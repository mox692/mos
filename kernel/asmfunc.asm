; asmfunc.asm
;
; System V AMD64 Calling Convention
; Registers: RDI, RSI, RDX, RCX, R8, R9

bits 64
section .text

global IoOut32  ; void IoOut32(uint16_t addr, uint32_t data);
IoOut32:
    mov dx, di    ; dx = addr
    mov eax, esi  ; eax = data
    out dx, eax
    ret

global IoIn32  ; uint32_t IoIn32(uint16_t addr);
IoIn32:
    mov dx, di    ; dx = addr
    in eax, dx
    ret

; MEMO: csレジスタ(16bitの値で、現在のcodeセグメントを指す)の内容を返す.
; ref: https://stackoverflow.com/questions/17777146/what-is-the-purpose-of-cs-and-ip-registers-in-intel-8086-assembly
global GetCS  ; uint16_t GetCS(void);
GetCS:
    xor eax, eax  ; also clears upper 32 bits of rax
    mov ax, cs
    ret

; MEMO: メインメモリ上のidtのアドレスと、idtの大きさを
;       10byteの領域に書き込む. (ref: p168)
global LoadIDT  ; void LoadIDT(uint16_t limit, uint64_t offset);
LoadIDT:
    push rbp
    mov rbp, rsp
    sub rsp, 10
    mov [rsp], di  ; limit(arg1)
    mov [rsp + 2], rsi  ; offset(arg2)
    lidt [rsp]
    mov rsp, rbp
    pop rbp
    ret

; MEMO: メインメモリ上のGDTのアドレスと、GDTの大きさを
;       lgdt命令でcpuに登録する. (ref: p192)
global LoadGDT  ; void LoadGDT(uint16_t limit, uint64_t offset);
LoadGDT:
    push rbp
    mov rbp, rsp
    sub rsp, 10
    mov [rsp], di  ; limit(2byteなのでdiを指定してることに注意!!!)
    mov [rsp + 2], rsi  ; offset
    lgdt [rsp]     ; load grobal discriptor table
    mov rsp, rbp
    pop rbp
    ret

; csを直接operandに取れないのでトリッキーな手法でcsレジスタを入れ替える
global SetCSSS  ; void SetCSSS(uint16_t cs, uint16_t si);
SetCSSS:
    push rbp
    mov rbp, rsp
    mov ss, si
    mov rax, .next
    push rdi    ; CS
    push rax    ; RIP
    o64 retf
.next:
    mov rsp, rbp
    pop rbp
    ret

; これらのregisterは64bit modeでは実際ほぼ使われないらしい.
global SetDSAll  ; void SetDSAll(uint16_t value);
SetDSAll:
    mov ds, di
    mov es, di
    mov fs, di
    mov gs, di
    ret

global SetCR3  ; void SetCR3(uint64_t value);
SetCR3:
    mov cr3, rdi
    ret

extern kernel_main_stack
extern KernelMainNewStack

global KernelMain
KernelMain:
    mov rsp, kernel_main_stack + 1024 * 1024
    call KernelMainNewStack
.fin:
    hlt
    jmp .fin
; #@@range_end(set_main_stack)
