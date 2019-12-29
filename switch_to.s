.global switch_to
.section .text
switch_to:
  movl 12(%esp), %eax
  cmpl $0x0, %eax
  je switch_stack
  movl %ebp, (%eax)
  movl 16(%esp), %eax
  movl %esp, (%eax)
switch_stack:
  movl 4(%esp), %ebp 
  movl 8(%esp), %esp
  ret
