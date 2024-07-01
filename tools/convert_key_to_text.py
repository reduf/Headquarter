import os
import utils

def main():
    data_dir = utils.get_path('data')
    files = os.listdir(data_dir)
    files = list(filter(lambda file: file.endswith('.pub'), files))
    
    for file in files:
        path = os.path.join(data_dir, file)
        bytes = open(path, 'rb').read()
        root = int.from_bytes(bytes[0:4], byteorder='big')
        prime = int.from_bytes(bytes[4:68], byteorder='big')
        public_key = int.from_bytes(bytes[68:], byteorder='big')
        output = f'root = {root}\nserver_public = {public_key}\nprime = {prime}'
        open(f'{path}.txt', 'w').write(output)
        print(f"Converted '{path}'")

if __name__ == '__main__':
    main()