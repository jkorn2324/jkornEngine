#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ] || [ -z "$4" ]; then
    echo "Usage: $0 <repo_owner> $1 <repo_name> $2 <download_index> $3 <output_folder_path>"
    exit 1
fi

repo_owner=$1
repo_name=$2
index=$3
output_folder_path=$4

# Check if Python is installed
if command -v python3 &> /dev/null; then
    python3 dl_latest_git_release.py $repo_owner $repo_name $index $output_folder_path
elif command -v python &> /dev/null; then
    python dl_latest_git_release.py $repo_owner $repo_name $index $output_folder_path
else
    echo "Error: Python is not installed."
    exit 1
fi
