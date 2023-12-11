import Setup.setup_premake as setup_premake
import Project.project_constants as project_constants
import subprocess

# Import platform utilities
from Utils import platform_utilities


# Generates project files.
def generate_project_files() -> bool:
    print("Generating project files...")
    if not setup_premake.premake_exists(project_constants.binaries_path):
        print("Premake doesn't exist yet, make sure that you run setup.py")
        return False
    premake_executable = setup_premake.get_premake_executable_path(project_constants.binaries_path)
    platform = platform_utilities.get_platform_type()
    if platform == platform_utilities.PlatformType.MacOS:
        subprocess.run([premake_executable, 'xcode4'])
    elif platform == platform_utilities.PlatformType.Windows:
        subprocess.run([premake_executable, 'vs2022'])
    else:
        return False
    print("Finished generating project files!")
    return True


if __name__ == '__main__':
    generate_project_files()
