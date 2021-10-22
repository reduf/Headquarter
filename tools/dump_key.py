from process import *

import argparse
import utils
import sys

def reverse_endian(data):
    return data[::-1]

def print_bytes(bytes):
    str = '\\x' + '\\x'.join('%02X' % b for b in bytes)
    print(str)

def get_build_number(proc, scanner):
    build_address = scanner.find(b'\xC2\x0C\x00\xCC\xCC\xB8', 6)
    build_number, = proc.read(build_address, 'I')
    return build_number

def main(args):
    if args.pid:
        proc = Process(args.pid)
    if args.proc:
        proc = Process.from_name(args.proc)
    scanner = ProcessScanner(proc)

    addr = scanner.find(b'\x8B\x45\x08\xC7\x00\x88\x00\x00\x00\xB8', +0xA)
    keys = proc.read(addr)[0]

    pr = proc.read(keys + 4, '4s')[0]
    pm = proc.read(keys + 8, '64s')[0]
    pk = proc.read(keys + 72, '64s')[0]

    pr = int.from_bytes(pr, byteorder='little')
    pm = int.from_bytes(pm, byteorder='little')
    pk = int.from_bytes(pk, byteorder='little')

    if args.output:
        output = args.output
    else:
        build = get_build_number(proc, scanner)
        output = utils.get_path(f'data\\gw_{build}.pub')

    if args.text:
        content = '\n'.join([f'pr: {pr}', f'prime: {pm}', f'pkey: {pk}'])
    else:
        content = pr.to_bytes(4, byteorder='big')
        content += pm.to_bytes(64, byteorder='big')
        content += pk.to_bytes(64, byteorder='big')

    if output == '-':
        print(content)
    else:
        print(f'Writting to "{output}"')
        if args.text:
            fd = open(output, 'w')
        else:
            fd = open(output, 'wb')
        fd.write(content)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--pid", type=int, required=False,
        help="Process id of the target Guild Wars instance.")
    parser.add_argument("--proc", type=str, default='Gw.exe',
        help="Process name of the target Guild Wars instance.")
    parser.add_argument("--output", "-o", type=str, required=False,
        help="Path where to write the output keys.")
    parser.add_argument("--text", action='store_true', required=False,
        default=False, help="Will output the document as text")
    args = parser.parse_args()

    main(args)
