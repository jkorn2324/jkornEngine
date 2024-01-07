import os
import subprocess
import Utils.platform_utilities as platform_utilities
import Utils.file_utilities as file_utilities
import Project.project_constants as project_constants

from enum import Enum


# Install VulkanSDK
# MacOS: https://sdk.lunarg.com/sdk/download/1.3.268.0/mac/vulkansdk-macos-1.3.268.0.dmg
# Windows: https://sdk.lunarg.com/sdk/download/1.3.268.0/windows/VulkanSDK-1.3.268.0-Installer.exe

class VulkanSDKInstallStatus(Enum):
    # Vulkan is not installed @ path & vulkan installation exe is not downloaded either
    NotInstalled = 0,
    # Vulkan is not installed @ path & vulkan installation exe is downloaded
    Downloaded = 1,
    # Vulkan is not installed @ path & vulkan installation exe is set up
    Setup = 2,
    # Vulkan is installed @ path
    FullInstalled = 3


# Gets the vulkan operating system install path
# On Mac it is in the ~/VulkanSDK/{version}
# On Windows it is where the environment variable is
def get_vulkan_os_install_path(major_version: int, minor_version: int, patch_version: int,
                               revision_version: int) -> str or None:
    vulkan_version = get_vulkan_version(major_version=major_version, minor_version=minor_version,
                                        patch_version=patch_version, revision=revision_version)
    platform_type = platform_utilities.get_platform_type()
    if platform_type == platform_utilities.PlatformType.MacOS:
        return os.path.expanduser(f"~/VulkanSDK/{vulkan_version}")
    if platform_type == platform_utilities.PlatformType.Windows:
        return os.path.abspath(f"C:\\VulkanSDK\\{vulkan_version}")
    # TODO: the vulkan os install path for linux
    return None


# Determines whether or not the user has the vulkan sdk installed
# 0 = Doesn't have executable, doesn't have sdk installed at path
# 1 = Has executable, doesn't have sdk installed at path
# 2 = Has executable, has sdk installed
def get_vulkan_sdk_status(vulkan_root_path: str, major_version: int, minor_version: int, patch_version: int,
                          revision: int) -> VulkanSDKInstallStatus:
    platform_type = platform_utilities.get_platform_type()
    vulkan_path = get_vulkan_path(vulkan_root_path)

    vulkan_installation_path = os.path.abspath(get_vulkan_install_path(vulkan_root_path))
    status_installed = os.path.exists(vulkan_installation_path)
    if status_installed:
        return VulkanSDKInstallStatus.FullInstalled

    vulkan_install_path = get_vulkan_os_install_path(major_version=major_version, minor_version=minor_version,
                                                     patch_version=patch_version, revision_version=revision)
    status_setup = vulkan_install_path is not None and os.path.exists(vulkan_install_path)
    if status_setup:
        return VulkanSDKInstallStatus.Setup

    if platform_type == platform_utilities.PlatformType.MacOS:
        executable_path = os.path.abspath(vulkan_path + os.sep + 'vulkan-sdk-installer.dmg')
        status_downloaded = os.path.exists(executable_path)
        if status_downloaded:
            return VulkanSDKInstallStatus.Downloaded
        # The not installed.
        return VulkanSDKInstallStatus.NotInstalled

    # TODO: Determine whether or not the vulkansdk is installed on windows
    return VulkanSDKInstallStatus.NotInstalled


# Gets the vulkan install path
def get_vulkan_install_path(root_path: str) -> str:
    return get_vulkan_path(root_path) + os.sep + 'install'


# Gets the vulkan path.
def get_vulkan_path(root_path: str) -> str:
    return project_constants.create_path(root_path, 'vulkansdk')


# Gets the vulkan version.
def get_vulkan_version(major_version: int, minor_version: int, patch_version: int, revision: int):
    return f"{major_version}.{minor_version}.{patch_version}.{revision}"


# Gets the vulkan sdk url.
def get_vulkan_sdk_url(platform_type: platform_utilities.PlatformType, major_version: int, minor_version: int,
                       patch_version: int, revision: int) -> str or None:
    version = get_vulkan_version(major_version, minor_version, patch_version, revision)
    if platform_type == platform_utilities.PlatformType.MacOS:
        return f"https://sdk.lunarg.com/sdk/download/{version}/mac/vulkansdk-macos-{version}.dmg"
    elif platform_type == platform_utilities.PlatformType.Windows:
        return f"https://sdk.lunarg.com/sdk/download/{version}/windows/VulkanSDK-{version}-Installer.exe"
    return None


# Downloads teh vulkan setup executable file.
def download_vulkan_setup_executable(vulkan_root_path: str, major_version: int, minor_version, patch_version: int,
                                     revision_number: int) -> bool:
    print("Downloading VulkanSDK Setup Executable!")
    vulkan_path = get_vulkan_path(vulkan_root_path)
    platform_type = platform_utilities.get_platform_type()
    sdk_url = get_vulkan_sdk_url(platform_type, major_version, minor_version, patch_version, revision_number)
    # Determines whether or not the sdk url is none
    if sdk_url is None:
        print("Failed to Download VulkanSDK Setup Executable: Invalid URL!")
        return False

    file_ext = file_utilities.get_file_ext(sdk_url)
    executable_path = vulkan_path + os.sep + 'vulkan-sdk-installer.' + file_ext
    # Download the sdk to the executable path
    if not file_utilities.download_file(sdk_url, executable_path):
        print("Failed to Download VulkanSDK Setup Executable: Invalid SDK!")
        return False
    if platform_type == platform_utilities.PlatformType.Windows:
        os.startfile(os.path.abspath(executable_path))
    elif platform_type == platform_utilities.PlatformType.MacOS or platform_type == platform_utilities.PlatformType.Linux:
        subprocess.run(['open', executable_path], capture_output=True)

    # TODO: Download VulkanSDK executable
    print("Successfully Downloaded VulkanSDK Setup Executable.")
    return True


# Installs vulkan at the root path
def install_vulkan(vulkan_root_path: str, major_version: int = 1, minor_version=3, patch_version=268,
                   revision_number: int = 0, override: bool = False) -> bool:
    # TODO: Check if the vulkan sdk exists
    vulkan_sdk_status = get_vulkan_sdk_status(vulkan_root_path=vulkan_root_path, major_version=major_version,
                                              minor_version=minor_version, patch_version=patch_version,
                                              revision=revision_number)
    install_path = get_vulkan_install_path(vulkan_root_path)
    abs_install_path = os.path.abspath(install_path)

    platform = platform_utilities.get_platform_type()
    print("Installing VulkanSDK to directory: " + abs_install_path)

    if vulkan_sdk_status == VulkanSDKInstallStatus.FullInstalled:
        print(f"VulkanSDK already is installed at the path!")
        return True
    elif vulkan_sdk_status == VulkanSDKInstallStatus.Setup:
        print(f"VulkanSDK has been detected!")
        print(
            f"Copying VulkanSDK from directory '{get_vulkan_os_install_path(major_version, minor_version, patch_version, revision_number)}' to Install Path")

        os_path = str(get_vulkan_os_install_path(major_version=major_version, minor_version=minor_version,
                                                 patch_version=patch_version, revision_version=revision_number))
        if platform == platform_utilities.PlatformType.MacOS:
            # Create the directory at the path.
            if not os.path.exists(abs_install_path):
                os.makedirs(abs_install_path, exist_ok=False)
            # Copies the files over to the install directory
            subprocess.run(['cp', '-R', os_path + os.sep + 'macOS' + os.sep, abs_install_path], capture_output=True)
        elif platform == platform_utilities.PlatformType.Windows:
            # Copies the Bin, Bin32, Include, Lib, Lib32, Share, & Source files to install path
            subprocess.run(['robocopy', '/e', '/mt', os_path + os.sep + 'Include',
                            abs_install_path + os.sep + 'Include'], capture_output=True)
            subprocess.run(['robocopy', '/e', '/mt', os_path + os.sep + 'Lib',
                            abs_install_path + os.sep + 'Lib'], capture_output=True)
            subprocess.run(['robocopy', '/e', '/mt', os_path + os.sep + 'Lib32',
                            abs_install_path + os.sep + 'Lib32'], capture_output=True)

        # TODO: Install VulkanSDK at Path.
        print(f"Finished Copying VulkanSDK to '{abs_install_path}'")
        return True
    elif vulkan_sdk_status == VulkanSDKInstallStatus.Downloaded:
        # TODO: Make sure that the user sets up vulkan
        print(f"Failed to Install VulkanSDK at path: the VulkanSDK Installer needs to be executed.")
        return False

    # download the vulkan setup executable and open
    download_vulkan_setup_executable(vulkan_root_path=vulkan_root_path, major_version=major_version,
                                     minor_version=minor_version, patch_version=patch_version,
                                     revision_number=revision_number)
    return True
