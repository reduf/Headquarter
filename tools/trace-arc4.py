import collections
import signal

from hexdump import hexdump
from process import *

traces = collections.defaultdict(lambda: list())

@Hook.fastcall(LPVOID, LPVOID, DWORD, LPVOID, LPVOID)
def arc4_crypt_hook(state, _, size, input, output):
    global traces
    traces[state].append(proc.read(input, f'{size}s')[0])
    print(f'state: 0x{state:08x}')
    hexdump(proc.read(input, f'{size}s')[0])
    print('=====================')

stop = False
def signal_handler(sig, frame):
    global stop
    stop = True

if __name__ == '__main__':
    proc = GetProcesses('Gw.exe')[0]
    scanner = ProcessScanner(proc, 'Gw.exe')

    arc4_crypt_addr = scanner.find(b'\x0F\xB6\xF0\x8D\x49\x01\x8A\x1C\x32\x0F', -0x26)
    print(f'Found arc4_crypt @ 0x{arc4_crypt_addr:08X}')

    prev_handler = signal.signal(signal.SIGINT, signal_handler)
    with ProcessDebugger(proc) as dbg:
        dbg.add_hook(arc4_crypt_addr, arc4_crypt_hook)
        while not stop:
            dbg.poll(32)

    signal.signal(signal.SIGINT, prev_handler)
