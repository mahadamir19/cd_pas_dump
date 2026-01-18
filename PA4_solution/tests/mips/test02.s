.data
a: .word 0
z: .word 0
t0: .word 0
c: .word 0
.text
.globl main
main:
    li $t0, 10
    sw $t0, a
    li $t0, 20
    sw $t0, z
    lw $t0, a
    lw $t1, z
    add $t2, $t0, $t1
    sw $t2, t0
    lw $t0, t0
    sw $t0, c
    lw $a0, c
    li $v0, 1
    syscall
    li $a0, 10
    li $v0, 11
    syscall
    li $v0, 0
    jr $ra
li $v0, 10
syscall
