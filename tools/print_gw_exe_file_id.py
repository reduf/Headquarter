import argparse
import os
import sys
from scanner import FileScanner

def follow_call(scanner, call_rva):
    op, call_param = scanner.read(call_rva, '<BI')
    if op != 0xE8 and op != 0xE9:
        raise RuntimeError(f"Unsupported opcode '0x{op:02X} ({op})'")
    return call_rva + call_param + 5

def get_file_id(scanner):
    function_call_rva = scanner.find(b'\x8B\xC8\x33\xDB\x39\x8D\xC0\xFD\xFF\xFF\x0F\x95\xC3', -5)
    function_rva = follow_call(scanner, function_call_rva)
    file_id, = scanner.read(function_rva + 1, '<I')
    return file_id

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

    file_id = get_file_id(scanner)
    print(f'File id: {file_id}')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--pid", type=int, required=False,
        help="Process id of the target Guild Wars instance.")
    parser.add_argument("--proc", type=str, default='Gw.exe',
        help="Process name of the target Guild Wars instance.")
    parser.add_argument("--file", type=str, required=False,
        help="Path to the file on disk.")
    args = parser.parse_args()

    main(args)
