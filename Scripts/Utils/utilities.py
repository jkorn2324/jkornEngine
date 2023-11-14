import subprocess
import json


url_type_git_api_latest_rel = 0
url_type_git = 1


# Gets the url from the repository owner + repository name.
def get_url(repo_owner, repo_name, api: int = url_type_git_api_latest_rel):
    if api == url_type_git_api_latest_rel:
        return "https://api.github.com/repos/" + repo_owner + "/" + repo_name + "/releases/latest"
    if api == url_type_git:
        return "https://github.com/" + repo_owner + "/" + repo_name + ".git"
    return ""

# Gets the browser download url from the asset json map.
def get_browser_dl_url(asset_json_map, asset_index):
    asset_index = int(asset_index)
    if asset_index >= len(asset_json_map) or asset_index < 0:
        return None
    obj = asset_json_map[asset_index]
    if not obj['browser_download_url']:
        return None
    return obj['browser_download_url']


# Gets the latest release download url.
def get_latest_release_browser_dl_url(repo_owner, repo_name, asset_index):
    asset_index = int(asset_index)
    url = get_url(repo_owner, repo_name, url_type_git_api_latest_rel)
    curlOutput = subprocess.run(["curl", "-s", url], capture_output=True, text=True)
    jsonLoad = json.loads(curlOutput.stdout)
    # Determine if the assets exist.
    if not 'assets' in jsonLoad:
        return [None, jsonLoad['message']]
    
    assets = jsonLoad['assets']
    dlUrl = get_browser_dl_url(assets, asset_index=asset_index)
    return [dlUrl, None]
