import os
import pefile
import sys
import struct

# \x8B\xC8\x33\xDB\x39\x8D\xC0\xFD\xFF\xFF\x0F\x95\xC3

def parse_file(path):
    print(f"Processing file '{path}'")
    parsed = pefile.PE(path, fast_load=True)
    for section in parsed.sections:
        if section.Name == b'.text\x00\x00\x00':
            data = section.get_data()
            offset = data.find(b'\x8B\xC8\x33\xDB\x39\x8D\xC0\xFD\xFF\xFF\x0F\x95\xC3')
            if offset == -1:
                print('Failed to find the pattern in the .text section')
                os.exit(1)
            offset -= 5
            print(f'Found the pattern in the .text section, offset: 0x{offset:X}')
            call_param, = struct.unpack_from('<I', data, offset + 1)
            function_offset = call_param + offset + 5
            print(f'Function offset is: 0x{function_offset:X}')
            file_id, = struct.unpack_from('<I', data, function_offset + 1)
            print(f'File id: {file_id}')
            break
    else:
        print('Failed to find the .text section')

def main(args):
    if len(args) == 0:
        print('Usage: program <path to game exe>')
        os.exit(1)

    parse_file(args[0])

if __name__ == '__main__':
    main(sys.argv[1:])
