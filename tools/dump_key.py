from process import *
import sys

def reverse_endian(data):
    new_bytes = []
    for byte in data[::-1]:
        new_bytes.append(byte)
    return bytes(new_bytes)

def print_bytes(bytes):
    str = '\\x' + '\\x'.join('%02X' % b for b in bytes)
    print(str)

def main(argv):
    if (len(argv) == 0):
        print('Set the file output path in parameters')
        sys.exit(1)

    proc = Process.from_name('Gw.exe')
    scanner = ProcessScanner(proc)

    addr = scanner.find(b'\x8B\x45\x08\xC7\x00\x88\x00\x00\x00\xB8', +0xA)
    keys = proc.read(addr)[0]

    root   = proc.read(keys + 4, '4s')[0]
    prime  = proc.read(keys + 8, '64s')[0]
    public = proc.read(keys + 72, '64s')[0]

    root   = reverse_endian(root)
    prime  = reverse_endian(prime)
    public = reverse_endian(public)

    file_name = argv[0]
    with open(file_name, 'wb') as file:
        file.write(root)
        file.write(prime)
        file.write(public)

if __name__ == '__main__':
    main(sys.argv[1::])
