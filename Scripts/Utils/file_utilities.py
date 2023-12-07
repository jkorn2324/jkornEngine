import os
import subprocess
from zipfile import ZipFile
from tarfile import TarFile

# Downloads a file from a url and a path
# Returns True if we successfully downloaded a file, false otherwise
def download_file(url: str, file_path: str) -> bool:
    file_path = os.path.abspath(file_path)
    os.makedirs(os.path.dirname(file_path), exist_ok=True)
    headers = {'User-Agent': "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36"}
    curl_cmd = ['curl']

    # Adds curl headers
    for key in headers:
        value = headers[key]
        curl_cmd.append('-H')
        curl_cmd.append( '\"' + key + ": " + value + '\"')

    curl_cmd.append('-L')
    curl_cmd.append(url)
    curl_cmd.append('-o')
    curl_cmd.append(file_path)
    
    success = subprocess.run(curl_cmd)
    print(success)

    return success != 0


# Gets the file extension from a string path
def get_file_ext(path: str):
    file_ext_list = str(path).split('.')
    last_index = len(file_ext_list) - 1
    file_ext = file_ext_list[last_index]
    if file_ext == "gz":
        file_ext = file_ext_list[last_index - 1] + '.' + file_ext
    return file_ext


# Unzips the zip file at the file path
def unzip_zip_file(file_path, delete_zip: bool = True):
    zip_file_path = os.path.abspath(file_path)
    zip_file_location = os.path.dirname(zip_file_path)

    # Extract Zip file to folder path & then remove zip file
    with ZipFile(zip_file_path, 'r') as z_obj:
        z_obj.extractall(path=zip_file_location)
    z_obj.close()
    
    # Delete the zip file from the file path.
    if delete_zip:
        os.remove(zip_file_path)
    return True


# Unzips the tar.gz file at the file path
def unzip_tar_gz_file(file_path, delete_zip: bool=True):
    tar_file_path = os.path.abspath(file_path)
    tar_location = os.path.dirname(tar_file_path)

    with TarFile(tar_file_path, 'r') as t_obj:
        print(t_obj)
        t_obj.extractall(path=tar_location)
    t_obj.close()

    # Delete the tar.gz at the location.
    if delete_zip:
        os.remove(tar_file_path)
    return True

# Unzips the file based on the file extension
def unzip_file(file_path, delete_zip: bool=True):
    file_ext = get_file_ext(file_path)
    if file_ext == 'zip':
        unzip_zip_file(file_path=file_path, delete_zip=delete_zip)
    elif file_ext == 'tar.gz':
        unzip_tar_gz_file(file_path=file_path, delete_zip=delete_zip)
