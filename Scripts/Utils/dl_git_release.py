# Usage: {python} repo_test.py {repo_owner} {repo_name} {download_index} {output_folder_path}

import sys
import Utils.file_utilities
import Utils.git_utilities

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


# Downloads the latest release from a repository & unzips the repository
def dl_latest_release_unzip(repo_owner, repo_name, asset_index, output_path) -> bool:
    output = Utils.git_utilities.get_latest_git_release_browser_dl_url(
        repo_owner=repo_owner,repo_name=repo_name, asset_index=asset_index)
    dirUrl = output[0]
    if dirUrl is None:
        print("Failed to get the browser download url: " + output[1])
        return 1
    return Utils.git_utilities.dl_release_unzip(dirUrl, output_path=output_path)


def main():
    parsed_cmd_args = parse_command_args()
    if parsed_cmd_args == None:
        print("Usage: dl_latest_git_release.py {repository_owner} {repository_name} {download_index}")
        return 1
    result = dl_latest_release_unzip(parsed_cmd_args[repository_owner], parsed_cmd_args[repository_name], 
                            parsed_cmd_args[asset_index], parsed_cmd_args[output_path])
    if result:
        return 0
    return False


# Invokes the main functionality.
if __name__ == "__main__":
    main()