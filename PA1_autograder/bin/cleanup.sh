#!/bin/bash

WORKSPACE_DIR="../workspace"
RESULTS_DIR="../results"

echo "WARNING: This will delete:"
echo "  1. All content in $WORKSPACE_DIR"
echo "  2. All content in $RESULTS_DIR (Grades & Feedback)"
echo ""
read -p "Are you sure you want to proceed? (y/n): " confirm

if [[ "$confirm" != "y" ]]; then
    echo "Aborted."
    exit 1
fi

echo "Cleaning Workspace..."
rm -rf "$WORKSPACE_DIR/current_student"
rm -rf "$WORKSPACE_DIR/logs"

echo "Cleaning Results..."
rm -f "$RESULTS_DIR/grades.csv"
rm -rf "$RESULTS_DIR/feedback"

# Recreate empty directories
mkdir -p "$WORKSPACE_DIR/current_student"
mkdir -p "$WORKSPACE_DIR/logs"
mkdir -p "$RESULTS_DIR/feedback"

echo "Cleanup done. Ready for new grading run."