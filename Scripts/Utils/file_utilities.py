import os
import sys
import time
from zipfile import ZipFile
from tarfile import TarFile

# Try to import requests
try:
    import requests
except:
    requests = None

# Downloads a file from an url and a path
# Returns True if we successfully downloaded a file, false otherwise
def download_file(url: str, file_path: str) -> bool:

    if requests is None:
        return False

    file_path = os.path.abspath(file_path)
    os.makedirs(os.path.dirname(file_path), exist_ok=True)

    headers = {
        'User-Agent': "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_4) AppleWebKit/537.36 (KHTML, like Gecko) "
                      "Chrome/83.0.4103.97 Safari/537.36"
    }
    print(f"Downloading File at url '{url}' to path: {file_path}")
    with open(file_path, 'wb') as file:
        response = requests.get(url, headers=headers, stream=True)
        total = response.headers.get('content-length')
        if total is None:
            file.write(response.content)
        else:
            downloaded_size = 0
            total = int(total)
            start_time = time.time()
            for data in response.iter_content(chunk_size=max(int(total / 1000), 1024 * 1024)):
                downloaded_size += len(data)
                file.write(data)

                try:
                    done = int(50 * downloaded_size / total) if downloaded_size < total else 50
                    percentage = (downloaded_size / total) * 100 if downloaded_size < total else 100
                except ZeroDivisionError:
                    done = 50
                    percentage = 100
                elapsed_time = time.time() - start_time
                try:
                    avg_kb_per_second = (downloaded_size / 1024) / elapsed_time
                except ZeroDivisionError:
                    avg_kb_per_second = 0.0

                avg_speed_string = '{:.2f} KB/s'.format(avg_kb_per_second)
                if avg_kb_per_second > 1024:
                    avg_mb_per_second = avg_kb_per_second / 1024
                    avg_speed_string = '{:.2f} MB/s'.format(avg_mb_per_second)

                sys.stdout.write(f"\r[{' ' * done}{'.' * (50 - done)}] {percentage:.2f}% ({avg_speed_string})")
                sys.stdout.flush()
    sys.stdout.write('\n')
    return True


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
    file_path = str(file_path)
    zip_file_path = os.path.abspath(file_path)
    zip_file_location = zip_file_path.replace('.zip', '')

    # Extract Zip file to folder path & then remove zip file
    z_obj = ZipFile.open(zip_file_path, mode='r')
    if z_obj is not None:
        z_obj.extractall(path=zip_file_location)
        z_obj.close()

    # Delete the zip file from the file path.
    if delete_zip:
        os.remove(zip_file_path)
    return True


# Unzips the tar.gz file at the file path
def unzip_tar_gz_file(file_path, delete_zip: bool = True):
    file_path = str(file_path)
    tar_file_path = os.path.abspath(file_path)
    tar_file_location = tar_file_path.replace('.tar.gz', '')

    # Extract the TarGz file to folder path & then remove the tar.gz file.
    t_obj = TarFile.open(tar_file_path, mode='r')
    if t_obj is not None:
        t_obj.extractall(path=tar_file_location)
        t_obj.close()

    # Delete the tar.gz at the location.
    if delete_zip:
        os.remove(tar_file_path)
    return True


# Unzips the file based on the file extension
def unzip_file(file_path, delete_zip: bool = True):
    file_ext = get_file_ext(file_path)
    print(f"Unzipping file at '{os.path.abspath(file_path)}'")
    if file_ext == 'zip':
        unzip_zip_file(file_path=file_path, delete_zip=delete_zip)
    elif file_ext == 'tar.gz':
        unzip_tar_gz_file(file_path=file_path, delete_zip=delete_zip)
