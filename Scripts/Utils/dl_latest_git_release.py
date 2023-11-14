# Usage: {python} repo_test.py {repo_owner} {repo_name} {download_index} {output_folder_path}

import sys
import os
import utilities
import subprocess
from zipfile import ZipFile
from tarfile import TarFile

repository_owner = 0
repository_name = 1
asset_index = 2
output_path = 3

num_command_line_params = 4

# Parses the command args.
def parse_command_args():
    cmd_args = sys.argv
    if len(cmd_args) != num_command_line_params + 1:
        return None
    return [cmd_args[1], cmd_args[2], cmd_args[3], cmd_args[4]]


def main():
    parsed_cmd_args = parse_command_args()
    if parsed_cmd_args == None:
        print("Usage: repo_test.py {repository_owner} {repository_name} {download_index}")
        return 1
    output = utilities.get_latest_release_browser_dl_url(parsed_cmd_args[repository_owner], parsed_cmd_args[repository_name], parsed_cmd_args[asset_index])
    dirUrl = output[0]
    if dirUrl is None:
        print("Failed to get the browser download url: " + output[1])
        return 1
    folder_path = str(parsed_cmd_args[output_path])

    # Get the file path + extension
    file_ext_list = str(dirUrl).split('.')
    last_index = len(file_ext_list) - 1
    file_ext = file_ext_list[last_index]
    if file_ext == "gz":
        file_ext = file_ext_list[last_index - 1] + '.' + file_ext
    file_path = folder_path + '.' + file_ext
        
    # Execute zip download download
    success = subprocess.run(['curl', '-L', dirUrl, '-o', file_path])
    if success == 0:
        print("Failed to download the file at url.")
        return 1
    
    ## if file extension is a zip.
    if file_ext == 'zip':
        # Extract Zip file to folder path & then remove zip file
        with ZipFile(file_path, 'r') as z_obj:
            z_obj.extractall(path=folder_path)
        z_obj.close()
    # if file extension is a tar.gz zip file
    elif file_ext == 'tar.gz':
        t_obj = TarFile.open(file_path, 'r')
        t_obj.extractall(path=folder_path)
        t_obj.close()

    os.remove(file_path)
    return 0

main()