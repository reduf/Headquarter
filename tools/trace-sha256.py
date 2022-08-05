import signal
import collections
from hexdump import hexdump
from process import *

traces = collections.defaultdict(lambda: list())

@Hook.fastcall(LPVOID, DWORD, LPVOID, DWORD)
def on_sha256_update(this, _, data, size):
    # print(f'Sha256::Update 0x{this:08X}, Size = {size} (0x{size:X})')
    global traces
    traces[this].append(proc.read(data, f'{size}s')[0])

@Hook.fastcall(LPVOID)
def on_sha256_finish(this):
    # print(f'Sha256::Finish 0x{this:08X}')
    pass

@Hook.fastcall(LPVOID, DWORD, LPVOID, DWORD, LPVOID)
def on_cbc_encrypt(this, _, output, size, input):
    print('CbcCrypt::Encrypt')
    hexdump(proc.read(input, f'{size}s')[0])

@Hook.fastcall(LPVOID, DWORD, DWORD, LPVOID, DWORD)
def on_some_random_func(this, _edx, _, client_finished, size):
    if size != 12:
        return
    client_finished_bytes = proc.read(client_finished, f'{size}s')[0]
    print('### client finished')
    hexdump(client_finished_bytes)

def read_LPCSTR(proc, addr, maxlen):
    codepoints = proc.read(addr, '%ds' % maxlen)[0]
    try:
        length = codepoints.index(0)
    except ValueError:
        length = maxlen
    codepoints = codepoints[:length]
    return ''.join(chr(c) for c in codepoints)

@Hook.fastcall(LPVOID, DWORD, LPVOID)
def on_prf_process_label(this, _, label):
    label = read_LPCSTR(proc, label, 64)
    print(label)

# @Hook.fastcall(LPVOID, DWORD, LPVOID, DWORD)
# def on_prf_finish(this, _, buffer, size):
#     pass

@Hook.fastcall(LPVOID, DWORD, LPVOID, DWORD)
def on_prf_init(this, _, data, size):
    print('PrfInit')
    hexdump(proc.read(data, f'{size}s')[0])

@Hook.fastcall(LPVOID, DWORD, LPVOID, DWORD)
def on_prf_update(this, _, data, size):
    print('PrfUpdate')
    hexdump(proc.read(data, f'{size}s')[0])

stop = False
def signal_handler(sig, frame):
    global stop
    stop = True

if __name__ == '__main__':
    proc = GetProcesses('Gw.exe')[0]
    scanner = ProcessScanner(proc, 'GwLoginClient.dll')
    sha256_update_addr = scanner.find(b'\x8B\xF1\x80\xBE\x98\x00\x00\x00\x00\x74\x17', -4)
    sha256_finish_addr = scanner.find(b'\x8B\xF1\x80\xBE\x98\x00\x00\x00\x00\x8D', -1)
    cbc_encrypt_addr = scanner.find(b'\x8B\xD9\x56\x83\x7B\x10\x00\x75\x17', -7)
    some_random_func_addr = scanner.find(b'\x56\x8B\x75\x0C\x8B\xD9\x57\x8B\x7D\x08\x8D\x43\x20', -20)
    prf_init_addr = scanner.find(b'\x8B\xF1\x57\x8B\x4E\x30\xC7\x46\x34\x00\x00', -8)
    prf_process_label_addr = scanner.find(b'\x8B\x56\x40\x03\xCF\x83\xC4\x04\x3B\xCA\x76', -52)
    prf_update_addr = scanner.find(b'\x83\xC4\x0C\x8B\x4E\x44\x8B\x7D\x0C\x03\xCF', -36)
    prf_finish_addr = scanner.find(b"\x8B\x5E\x44\x8B\x46\x3C\x8B\x4E\x20\x89\x45\xFC", -41)

    print(f'Found Sha256::Update @ 0x{sha256_update_addr:08X}')
    print(f'Found Sha256::Finish @ 0x{sha256_finish_addr:08X}')
    print(f'Found CptCbc::Encrypt @ 0x{cbc_encrypt_addr:08X}')
    print(f'Found PrfInit @ 0x{prf_init_addr:08X}')
    print(f'Found PrfProcessLabel @ 0x{prf_process_label_addr:08X}')
    print(f'Found PrfFinish @ 0x{prf_finish_addr:08X}')

    prev_handler = signal.signal(signal.SIGINT, signal_handler)

    dbg = ProcessDebugger(proc)
    # dbg.add_hook(sha256_update_addr, on_sha256_update)
    # dbg.add_hook(sha256_finish_addr, on_sha256_finish)
    # dbg.add_hook(cbc_encrypt_addr,   on_cbc_encrypt)
    # dbg.add_hook(some_random_func_addr, on_some_random_func)
    dbg.add_hook(prf_init_addr, on_prf_init)
    dbg.add_hook(prf_process_label_addr, on_prf_process_label)
    dbg.add_hook(prf_update_addr, on_prf_update)
    # dbg.add_hook(prf_finish_addr, on_prf_finish)

    while not stop:
        dbg.poll(32)

    dbg.detach()
    signal.signal(signal.SIGINT, prev_handler)

    print('<============ traces ============>')
    for this, trace in traces.items():
        for data in trace:
            size = len(data)
            print(f'Sha256::Update 0x{this:08X}, Size = {size} (0x{size:X})')
            hexdump(data)
