# Sets up premake for the engine
import os
import Setup.setup_premake as setup_premake
import Setup.setup_vulkan as setup_vulkan
from Utils import python_utilities    
import Project.project_constants as project_constants


# Executes the setup for the engine.
def setup():
    # TODO: Setup + install python
    if not python_utilities.has_valid_python_version():
        # TODO: Install python.
        return True
    # Requests Installation
    python_utilities.install_package('requests')
    # Install Premake at the binaries path
    setup_premake.install_premake(premake_path=project_constants.binaries_path,
                                         premake_version='5.0.0-beta2', override=False)
    
    setup_vulkan.install_vulkan(vulkan_root_path=project_constants.binaries_path)
    # TODO: If on windows, install directX11 at binaries path
    # TODO: Install fbxsdk
    return True
    

if __name__ == '__main__':
    setup()

