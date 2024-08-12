#!/bin/bash

# Main script
main() {
    # Check if at least three arguments are provided
    if [ "$#" -lt 3 ]; then
        echo "Usage: $0 output_zip exclude1 exclude2 folder1 [folder2 ... folderN]"
        exit 1
    fi

    # Get the output zip file name
    local output_zip=$1

    # Get the files to exclude
    local exclude1=$2
    local exclude2=$3

    # Shift the arguments to get the list of folders
    shift 3

    if [ -f "$output_zip" ]; then
        rm "$output_zip"
        echo "Deleted existing zip file: $output_zip"
    fi

    # Find all files in the specified folders, excluding the specified files
    files_to_zip=()
    for folder in "$@"; do
        if [ -d "$folder" ]; then
            while IFS= read -r -d $'\0' file; do
                files_to_zip+=("$file")
            done < <(find "$folder" -type f ! -name "$exclude1" ! -name "$exclude2" -print0)
        else
            echo "Warning: '$folder' is not a directory, skipping."
        fi
    done

    files_to_zip+=("submissions.txt")
    files_to_zip+=("Dry.pdf")

    # Create the zip file with all found files, without directory structure
    zip -j "$output_zip" "${files_to_zip[@]}"
}

# Execute the main script
main "$@"
