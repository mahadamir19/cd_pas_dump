#!/bin/bash

SUBMISSIONS_DIR="../submissions"
DEST_DIR="../unzipped_submissions"

mkdir -p "$DEST_DIR"

echo "Unzipping all submissions..."

for zipfile in "$SUBMISSIONS_DIR"/*.zip; do
    [ -e "$zipfile" ] || continue
    
    filename=$(basename "$zipfile")
    # Extract Roll Number assuming <rollnumber>_PA4.zip
    rollnumber="${filename%_PA4.zip}"
    
    target_folder="$DEST_DIR/$rollnumber"
    
    echo "Extracting $rollnumber..."
    mkdir -p "$target_folder"
    unzip -q -o "$zipfile" -d "$target_folder"
done

echo "All submissions extracted to $DEST_DIR"