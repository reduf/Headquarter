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
    raise NotImplementedError("What is the pattern to get the build number?")

def main(args):
    if args.pid:
        proc = Process(args.pid)
    if args.proc:
        proc = Process.from_name(args.proc)
    scanner = ProcessScanner(proc)

    addr = scanner.find(b'\x8B\x45\x08\xC7\x00\x88\x00\x00\x00\xB8', +0xA)
    keys = proc.read(addr)[0]

    root = proc.read(keys + 4, '4s')[0]
    prime = proc.read(keys + 8, '64s')[0]
    public = proc.read(keys + 72, '64s')[0]

    root = reverse_endian(root)
    prime = reverse_endian(prime)
    public = reverse_endian(public)

    if args.output:
        output = args.output
    else:
        build = get_build_number(proc, scanner)
        output = utils.get_path(f'data/gw_{build}.pub')

    if output == '-':
        fd = sys.stderr.buffer
    else:
        fd = open(output, 'wb')

    fd.write(root)
    fd.write(prime)
    fd.write(public)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--pid", type=int, required=False,
        help="Process id of the target Guild Wars instance.")
    parser.add_argument("--proc", type=str, default='Gw.exe',
        help="Process name of the target Guild Wars instance.")
    parser.add_argument("--output", "-o", type=str, required=False,
        help="Path where to write the output keys.")
    args = parser.parse_args()

    main(args)
