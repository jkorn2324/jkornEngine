import sys

# Gets the platform name
def get_platform_name():
    if sys.platform.startswith('win32'):
        return 'Windows'
    elif sys.platform.startswith('linux'):
        return 'Linux'
    elif sys.platform.startswith('darwin'):
        return 'MacOS'
    return None