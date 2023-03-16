data_seg segment
data db 65000 dup(0)
buffer db 256 dup(0)
conter dw 0
error db 'Input data error$'
data_seg ends
stack_seg segment
dw 4096 dup(0)
tos label word
stack_seg ends
prog_seg segment
main proc far
assume cs:prog_seg,ds:data_seg,ss:stack_seg
n_start:
mov ax,stack_seg
mov ss,ax
mov sp,offset tos
push ds
xor ax,ax
push ax
mov ax,data_seg
mov ds,ax
mov di,offset data
mov ax,10
mov [di+6],ax
mov ax,20
mov [di+8],ax
mov ax,30
mov [di+10],ax
jmp n_0
fun1:mov [di+6],di
jmp n_1
n_1:call FAR PTR n_2
mov ax,9
mov [di+10],ax
mov bx,[di+5]
mov ax,[bx+10]
mov bx,[di+4]
mov [bx+12],ax
mov bx,[di+6]
mov dx,[bx+8]
mov bx,[di+4]
xor ax,ax
cmp [bx+12],dx
jge n_3
mov ax,-1
n_3:nop
and ax,ax
jnz n_4
jmp n_5
n_4:nop
mov bx,[di+4]
mov ax,[bx+8]
mov bx,[di+4]
mov [bx+14],ax
n_5:nop
mov di,[di]
ret
n_2:mov ax,12
add ax,di
mov [di+2],ax
ret
n_0:call FAR PTR n_6
mov ax,90
mov [di+18],ax
mov bx,[di+4]
mov ax,[bx+18]
mov bx,[di+4]
mov [bx+12],ax
mov di,[di]
ret
n_6:mov ax,20
add ax,di
mov [di+2],ax
ret
prog_seg ends
main endp
end n_start
