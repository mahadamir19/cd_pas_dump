.data
.text
.globl main
main:
    li $a0, 9
    li $v0, 1
    syscall
    li $a0, 10
    li $v0, 11
    syscall
    li $v0, 0
    jr $ra
li $v0, 10
syscall
