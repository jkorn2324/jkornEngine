# Sets up premake for the engine
import os
import Setup.setup_premake as setup_premake
from Utils import python_utilities    
import Project.project_constants as project_constants


# Executes the setup for the engine.
def setup():
    if not python_utilities.has_valid_python_version():
        # TODO: Install python.
        return True
    # Requests Installation
    python_utilities.install_package('requests')
    # Install Premake at the binaries path
    if not setup_premake.install_premake(premake_path=project_constants.binaries_path,
                                         premake_version='5.0.0-beta2', override=False):
        return False
    # TODO: If on windows, install directX11 at binaries path
    # TODO: Install VulkanSDK at Binaries Path
    return True
    

if __name__ == '__main__':
    setup()

