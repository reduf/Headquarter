from process import *

import sys
import time
import signal

def main(argv):
    # proc = Process.from_name('Gw.exe')
    proc = Process(3356)
    dbg  = ProcessDebugger(proc)
    scanner = ProcessScanner(proc)

    def signal_handler(signal, frame):
        dbg.detach()
    signal.signal(signal.SIGINT, signal_handler)

    @Hook.fastcall(LPVOID, DWORD, LPVOID)
    def OnSendPacket(ctx, size, packet):
        header = proc.read(packet, 'I')[0]
        print('Packet {%-3d, %d, 0x%X}' % (size, header, header))

    @Hook.fastcall(LPVOID)
    def OnRecvPacket(packet):
        header = proc.read(packet, 'I')[0]
        print(header)

    @Hook.fastcall(LPVOID, DWORD)
    def OnWriteChatLog(msg, channel):
        print(f'New message in channel {channel}')

    addr = scanner.find(b'\x55\x8B\xEC\x83\xEC\x2C\x53\x56\x57\x8B\xF9\x85')
    dbg.add_hook(addr, OnSendPacket)
    # dbg.add_hook(0x007DE540, OnWriteChatLog)

    """
    addr = scanner.find(b'\x50\x52\x8B\x55\x0C\xC7\x45\xF8', -0x23)
    addr = proc.read(addr)[0]      # 0xA2B294
    addr = proc.read(addr)[0]      # gs = *(GameServer **)0xA2B294, 0xa2b294
    addr = proc.read(addr + 8)[0]  # gs->consts
    gs_srv_codecs, _, gs_srv_codecs_count = proc.read(addr + 44, 'III')
    addr_gs_srv = range(gs_srv_codecs, gs_srv_codecs + (12 * gs_srv_codecs_count), 12) # GAME_SERVER
    for id, addr in enumerate(addr_gs_srv):
        fields_addr, count, handler = proc.read(addr, 'III')
        if not handler:
            continue
        if id in ignored_stoc:
            continue
        # dbg.add_hook(handler, OnRecvPacket)
    """

    print(f'Start debugging process {proc.name}, {proc.id}')
    return dbg.run(frequency=250)

if __name__ == '__main__':
    error = main(sys.argv[1:])
    sys.exit(error)
