import subprocess
import json
from enum import Enum
from Utils import file_utilities


class GitUrlType(Enum):
    url_type_git_api_latest_release = 0,
    url_type_git = 1


# Builds a url based on a repo_owner, repo_name, repo_version, & file_name
def get_git_release_download_url(repo_owner: str, repo_name: str, version: str, file_name: str) -> str:
    return "https://github.com/" + repo_owner + '/' + repo_name + '/releases/download/v' + version + '/' + file_name


# Gets the url from the repository owner + repository name.
def build_git_url(repo_owner, repo_name, api: GitUrlType = GitUrlType.url_type_git_api_latest_release):
    if api == GitUrlType.url_type_git_api_latest_release:
        return "https://api.github.com/repos/" + repo_owner + "/" + repo_name + "/releases/latest"
    if api == GitUrlType.url_type_git:
        return "https://github.com/" + repo_owner + "/" + repo_name + ".git"
    return ""


# Gets the browser download url from the asset json map.
def get_git_browser_dl_url(asset_json_map, asset_index):
    asset_index = int(asset_index)
    if asset_index >= len(asset_json_map) or asset_index < 0:
        return None
    obj = asset_json_map[asset_index]
    if not obj['browser_download_url']:
        return None
    return obj['browser_download_url']


# Gets the latest release download url.
def get_latest_git_release_browser_dl_url(repo_owner, repo_name, asset_index):
    asset_index = int(asset_index)
    url = build_git_url(repo_owner, repo_name, GitUrlType.url_type_git_api_latest_release)
    print(url)
    curl_output = subprocess.run(["curl", "-s", url], capture_output=True, text=True)
    json_load = json.loads(curl_output.stdout)
    # Determine if the assets exist.
    if 'assets' not in json_load:
        return [None, json_load['message']]

    assets = json_load['assets']
    dl_url = get_git_browser_dl_url(assets, asset_index=asset_index)
    return [dl_url, None]


# Downloads the git release and unzips the file
def dl_release_unzip(release_url: str, output_path: str) -> int:
    folder_path = str(output_path)
    # Get the file path + extension
    file_ext = file_utilities.get_file_ext(release_url)
    file_path = folder_path + '.' + file_ext

    if not file_utilities.download_file(release_url, file_path=file_path):
        print("Failed to download the file at url.")
        return 1
    # Extract Zip file to folder path & then remove zip file
    file_utilities.unzip_file(file_path=file_path, delete_zip=True)
    return 0
