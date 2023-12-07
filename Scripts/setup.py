# Sets up premake for the engine
import os
import setup_premake

# The root path + engine path
root_path = os.path.abspath(os.path.curdir) + os.sep
engine_path = root_path + 'Engine'
# Engine binaries path
binaries_path = root_path + 'Binaries'

# Executes the setup for the engine.
def setup():
    # Install Premake at the binaries path
    if not setup_premake.install_premake(premake_path=binaries_path, premake_version='5.0.0-beta2', override=False):
        return False
    return True
    

if __name__ == '__main__':
    setup()