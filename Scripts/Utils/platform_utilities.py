import sys
from enum import Enum


# The platform type
class PlatformType(Enum):
    Windows = 0
    Linux = 1
    MacOS = 2


# Gets the platform type
def get_platform_type() -> PlatformType or None:
    if sys.platform.startswith('win32'):
        return PlatformType.Windows
    elif sys.platform.startswith('linux'):
        return PlatformType.Linux
    elif sys.platform.startswith('darwin'):
        return PlatformType.MacOS
    return None


# Gets the platform name
def get_platform_name():
    current_type = get_platform_type()
    if current_type is not None:
        return str(current_type)
    return None


# Gets the user permission.
def get_user_permission(prompt: str) -> bool:
    permission_granted = False
    while not permission_granted:
        reply = str(input(f"{prompt} '{0:s}'? [Y/N]")).lower().strip()[:1]
        if reply == 'n':
            return False
        permission_granted = (reply == 'y')
    return permission_granted
