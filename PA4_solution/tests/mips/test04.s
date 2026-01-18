.data
a: .word 0
z: .word 0
t0: .word 0
t1: .word 0
result: .word 0
.text
.globl main
multiply:
    lw $t0, ($sp)
    addiu $sp, $sp, 4
    sw $t0, z
    lw $t0, ($sp)
    addiu $sp, $sp, 4
    sw $t0, a
    lw $t0, a
    lw $t1, z
    mul $t2, $t0, $t1
    sw $t2, t0
    lw $v0, t0
    jr $ra
main:
    li $t0, 56
    subu $sp, $sp, 4
    sw $t0, ($sp)
    li $t0, 2
    subu $sp, $sp, 4
    sw $t0, ($sp)
    jal multiply
    sw $v0, t1
    lw $t0, t1
    sw $t0, result
    lw $a0, result
    li $v0, 1
    syscall
    li $a0, 10
    li $v0, 11
    syscall
li $v0, 10
syscall
