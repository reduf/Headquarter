import argparse
import utils
import sys

def reverse_endian(data):
    return data[::-1]

def print_bytes(bytes):
    str = '\\x' + '\\x'.join('%02X' % b for b in bytes)
    print(str)

def get_build_number(scanner):
    build_address = scanner.find(b'\xC2\x0C\x00\xCC\xCC\xB8', 6)
    build_number, = scanner.read(build_address, '<I')
    return build_number

def main(args):
    if args.pid:
        import process
        proc = process.Process(args.pid)
        scanner = process.ProcessScanner(proc)
    elif args.file:
        from scanner import FileScanner
        scanner = FileScanner(args.file)
    elif args.proc:
        import process
        proc = process.Process.from_name(args.proc)
        scanner = process.ProcessScanner(proc)

    addr = scanner.find(b'\x8B\x45\x08\xC7\x00\x88\x00\x00\x00\xB8', +0xA)
    keys, = scanner.read(addr)

    if args.file:
        # It would be nice to not do that explicitly, but overall, this is a reloc,
        # so when reading from a file, we need to remove the `ImageBase` to get an
        # RVA based on 0.
        keys -= scanner.parsed.OPTIONAL_HEADER.ImageBase

    pr, = scanner.read(keys + 4, '4s')
    pm, = scanner.read(keys + 8, '64s')
    pk, = scanner.read(keys + 72, '64s')

    pr = int.from_bytes(pr, byteorder='little')
    pm = int.from_bytes(pm, byteorder='little')
    pk = int.from_bytes(pk, byteorder='little')

    if args.output:
        output = args.output
    else:
        build = get_build_number(scanner)
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
    parser.add_argument("--file", type=str, required=False,
        help="Path to the file on disk.")
    parser.add_argument("--output", "-o", type=str, required=False,
        help="Path where to write the output keys.")
    parser.add_argument("--text", action='store_true', required=False,
        default=False, help="Will output the document as text")
    args = parser.parse_args()

    main(args)
