#!/bin/bash

# Define paths relative to the bin directory
SUBMISSIONS_DIR="../submissions"
DEST_DIR="../unzipped_submissions"

mkdir -p "$DEST_DIR"

echo "Unzipping all submissions..."

for zipfile in "$SUBMISSIONS_DIR"/*.zip; do
    [ -e "$zipfile" ] || continue
    
    filename=$(basename "$zipfile")
    # Extract ID assuming <rollnumber>_PA1.zip format
    rollnumber="${filename%_PA1.zip}"
    
    target_folder="$DEST_DIR/$rollnumber"
    
    echo "Extracting $rollnumber..."
    mkdir -p "$target_folder"
    unzip -q -o "$zipfile" -d "$target_folder"
done

echo "All submissions unzipped to $DEST_DIR"