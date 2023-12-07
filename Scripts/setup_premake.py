import Utils.platform_utilities
import Utils.git_utilities
import os
import sys

# The expected windows premake files.
expected_windows_files = [
    'premake5.pdb',
    'premake5.exe',
    'luasocket.pdb',
    'luasocket.lib',
    'luasocket.exp',
    'luasocket.dll'
]


# The expected macos premake files.
expected_macos_files = [
    'premake5',
    'libluasocket.dylib'
]


# Creates the premake path for the root directory
def create_premake_path(premake_root_dir: str):
    return premake_root_dir + os.sep + 'premake'


# Creates the premake platform specific path.
def create_premake_platform_path(premake_root_dir: str):
    platform_name = Utils.platform_utilities.get_platform_name()
    if platform_name is None:
        return None
    return create_premake_path(premake_root_dir=premake_root_dir) + os.sep + platform_name


# Gets the executable path based on the root directory.
def get_premake_executable_path(premake_root_dir: str):
    platform_name = Utils.platform_utilities.get_platform_name()
    if platform_name is None:
        return None
    if platform_name == 'MacOS':
        return create_premake_platform_path(premake_root_dir=premake_root_dir) + os.sep + 'premake'
    elif platform_name == 'Windows':
        return create_premake_platform_path(premake_root_dir=premake_root_dir) + os.sep + 'premake5.exe'


# Determines whether or not the premake path + files exists.
def premake_exists(premake_dir: str):
    if not os.path.exists(premake_dir) or not os.path.isdir(premake_dir):
        return False
    
    platform_path = create_premake_platform_path(premake_dir)
    if not os.path.exists(platform_path) or not os.path.isdir(platform_path):
        return False

    system_name = Utils.platform_utilities.get_platform_name()
    if system_name == 'Windows':
        expected_files = expected_windows_files
    elif system_name == 'MacOS':
        expected_files = expected_macos_files
    else:
        expected_files = None

    if expected_files is None:
        return False
    
    # Iterates through each of the files to check if premake exists.
    for file in expected_files:
        path = premake_dir + os.sep + file
        if not os.path.exists(path=path):
            return False
    return True
    

# Installs premake to the path.
def install_premake(premake_path: str, premake_version: str, override: bool = False) -> bool:
    premake_dir = premake_path
    print("Installing Premake to Directory: " + premake_dir)
    if premake_exists(premake_dir) and not override:
        print("Premake didn't install as the premake file path already exists.")
        return False
    
    # Makes the premake directories.
    abs_premake_dir = create_premake_platform_path(premake_root_dir=premake_dir)
    os.makedirs(abs_premake_dir, exist_ok=True)

    system = Utils.platform_utilities.get_platform_name()
    if system == 'Windows':
        git_release_file = 'premake-' + premake_version + '-windows.zip'
    elif system == 'Linux':
        git_release_file = 'premake-' + premake_version + '-linux.tar.gz'
    elif system == 'MacOS':
        git_release_file = 'premake-' + premake_version + '-macosx.tar.gz'
    else:
        git_release_file = None

    if git_release_file is None:
        print("Failed to get the git release url file.")
        return False
    
    release_url = Utils.git_utilities.get_git_release_download_url(
        repo_owner='premake',
        repo_name='premake-core',
        version=premake_version,
        file_name=git_release_file
    )
    return Utils.git_utilities.dl_release_unzip(release_url=release_url,
            output_path=abs_premake_dir)


# The main function for premake.py
def main():
    cmd_args = sys.argv
    if len(cmd_args) != 2:
        return None
    premake_path = cmd_args[1]
    if install_premake(premake_path=premake_path):
        return 0
    return 1


if __name__ == "__main__":
    install_premake()