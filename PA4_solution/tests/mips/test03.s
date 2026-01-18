.data
i: .word 0
t0: .word 0
t1: .word 0
.text
.globl main
main:
    li $t0, 0
    sw $t0, i
L0:
    lw $t0, i
    li $t1, 5
    slt $t2, $t0, $t1
    sw $t2, t0
    lw $t0, t0
    beqz $t0, L1
    lw $a0, i
    li $v0, 1
    syscall
    li $a0, 10
    li $v0, 11
    syscall
    lw $t0, i
    li $t1, 1
    add $t2, $t0, $t1
    sw $t2, t1
    lw $t0, t1
    sw $t0, i
    j L0
L1:
    li $a0, 1111
    li $v0, 1
    syscall
    li $a0, 10
    li $v0, 11
    syscall
li $v0, 10
syscall
