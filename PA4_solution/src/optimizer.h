#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "ir.h"

/**
 * Runs all optimization passes repeatedly until no more changes occur.
 * (This is often called finding the "fixed point").
 */
void optimize_ir(IRInst *head);

/* --- Individual Passes (You implement these in optimizer.c) --- */

/**
 * Scans the IR for constant expressions (e.g., ADD 3 4 t0) 
 * and replaces them with direct assignments (ASSIGN 7 t0).
 * Returns 1 if a change was made, 0 otherwise.
 */
int constant_folding(IRInst *head);

/**
 * Scans the IR for unused variables or unreachable code 
 * and removes those instructions.
 * Returns 1 if a change was made, 0 otherwise.
 */
int dead_code_elimination(IRInst *head);

#endif