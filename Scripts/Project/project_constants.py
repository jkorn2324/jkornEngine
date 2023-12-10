import os

# The constants for the path objects
root_path = './'
binaries_path = root_path + 'External/Binaries/'
engine_path = root_path + 'Engine/'


# Gets the path
def create_path(root_dir: str, sub_folder: str):
    return root_dir + os.sep + sub_folder