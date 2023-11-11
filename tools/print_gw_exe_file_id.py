import os
import sys
from scanner import FileScanner

def parse_file(path):
    print(f"Processing file '{path}'")
    scanner = FileScanner(path, '.text')

    pos = scanner.find(b'\x8B\xC8\x33\xDB\x39\x8D\xC0\xFD\xFF\xFF\x0F\x95\xC3', -5)
    function_rva = scanner.follow_call(pos)
    print(f'Function offset is: 0x{function_rva:X}')
    file_id, = scanner.read(function_rva + 1, '<I')
    print(f'File id: {file_id}')

def main(args):
    if len(args) == 0:
        print('Usage: program <path to game exe>')
        os.exit(1)

    parse_file(args[0])

if __name__ == '__main__':
    main(sys.argv[1:])
