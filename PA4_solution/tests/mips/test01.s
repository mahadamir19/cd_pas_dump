.data
t0: .word 0
x: .word 0
t1: .word 0
y: .word 0
t2: .word 0
.text
.globl main
main:
    li $t0, 30
    sw $t0, t0
    lw $t0, t0
    sw $t0, x
    li $t0, 50
    sw $t0, t1
    lw $t0, t1
    sw $t0, y
    lw $t0, x
    lw $t1, y
    add $t2, $t0, $t1
    sw $t2, t2
    lw $a0, t2
    li $v0, 1
    syscall
    li $a0, 10
    li $v0, 11
    syscall
li $v0, 10
syscall
