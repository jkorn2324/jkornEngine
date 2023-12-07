# Sets up premake for the engine
import os
import setup_premake
import Utils.python_utilities

# The root path + engine path
root_path = './External/Binaries/'


# Executes the setup for the engine.
def setup():
    if not Utils.python_utilities.has_valid_python_version():
        # TODO: Install python.
        return True
    # Requests Installation
    Utils.python_utilities.install_package('requests')
    # Install Premake at the binaries path
    if not setup_premake.install_premake(premake_path=root_path,
                                         premake_version='5.0.0-beta2', override=False):
        return False
    # TODO: If on windows, install directX11 at binaries path
    # TODO: Install VulkanSDK at Binaries Path
    return True
    

if __name__ == '__main__':
    setup()

