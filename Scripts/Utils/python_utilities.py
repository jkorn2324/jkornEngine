import importlib
import importlib.util
import Utils.platform_utilities
import subprocess
import sys


def has_valid_python_version(min_version_major: int = 3, min_version_minor: int = 1) -> bool:
    """
    Args:
        min_version_major: The minimum major version of python.
        min_version_minor: The minimum minor version of python.
    Returns:
        True if the user has a minimum viable version of python.
    """
    if sys.version is None:
        return False
    version_info = sys.version_info
    print(f"Detected python version: v{version_info.major}.{version_info.minor}.{version_info.micro}")
    if version_info.major < min_version_major and version_info.minor < min_version_minor:
        print(
            f"Invalid python version, the version must be greater than at least v{min_version_major}.{min_version_minor}")
        return False
    return True


def try_import_package(package: str) -> bool:
    try:
        __import__(package)
        return True
    except ImportError:
        print("Failed to import package.")
        return False


def is_package_installed(package_name: str) -> bool:
    """
    Args:
        package_name: The package name we are checking is installed.
    Returns:
        True if package is installed, false otherwise
    """
    return importlib.util.find_spec(package_name) is not None


def install_package(package_name: str) -> bool:
    """
    Args:
        package_name: The python package name that we want to install
    Returns:
        True if package successfully installed, false otherwise
    """
    print(f"Installing python dependency package '{package_name}.")
    if importlib.util.find_spec(package_name) is None:
        permission_granted = Utils.platform_utilities.get_user_permission(
            f"Would you like to install the python package '{package_name}'")
        if not permission_granted:
            return False

        platform_name = Utils.platform_utilities.get_platform_type()
        if platform_name == Utils.platform_utilities.PlatformType.Windows:
            completed = subprocess.run(['py', '-m', 'pip', 'install', '--user', package_name], capture_output=True)
        elif platform_name == Utils.platform_utilities.PlatformType.Linux \
                or platform_name == Utils.platform_utilities.PlatformType.MacOS:
            completed = subprocess.run(['python3', '-m', 'pip', 'install', '--user', package_name], capture_output=True)
        else:
            completed = None

        if completed is None:
            return False
        return completed.returncode == 0
    print(f"Python Package '{package_name}' is already installed.\n")
    return True
