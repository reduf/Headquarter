import socket
import struct
import time
import inflate

class FileNotFoundError(RuntimeError):

    def __init__(self, fileid):
        super().__init__()


class FileRequest(object):

    def __init__(self, cli, fileid, sz_cmp, sz_dcmp, crc):
        self.cli = cli
        self.fileid = fileid
        self.size_compressed = sz_cmp
        self.size_decompressed = sz_dcmp
        self.crc = crc
        self.buffer = b''
        self.last_chunk = 0
        self.size_complete = 0
        self.first = True

        self.file_id_manifest = 0
        self.file_id_backup_exe = 0
        self.file_id_latest_exe = 0

    def __repr__(self):
        return f'FileRequest: id={self.fileid} downloaded=({self.size_complete} of {self.size_compressed}) uncompressed {self.size_decompressed}'

    @property
    def percent(self):
        return self.size_complete / self.size_compressed

    @property
    def complete(self):
        return self.size_compressed <= self.size_complete

    def download_chunk(self):
        head, size = self.cli.recv('<HH')
        if head != 0x6F2 and head != 0x6F3:
            raise RuntimeError('header {head:X} is not 0x6F2 or 0x6F3?')

        size -= 4
        self.last_chunk = size

        while size > 0:
            data = self.cli.s.recv(size)
            size -= len(data)
            self.buffer += data
            self.size_complete += len(data)

    def request_more(self):
        self.cli.send('<HHI', 0x7F3, 0x8, 0x4000)

    def dump(self, fd):
        fd.write(self.buffer)

    def decompressed(self):
        assert self.complete
        result = inflate.inflate(self.buffer)
        assert(len(result) == self.size_decompressed)
        return result

    def finalize_download(self):
        pass

    def download(self):
        while True:
            self.download_chunk()
            if self.complete:
                self.finalize_download()
                break
            yield self.percent
            self.request_more()


class FileClient(object):
    def __init__(self):
        self.s = None

    def connect(self):
        for i in range(1, 13):
            try:
                self.s = socket.create_connection((f'file{i}.arenanetworks.com', 6112))
                self.s.settimeout(0)
                self.send_handshake()

                results = self.recv_wait('<HHIIIIIII')
                self.file_id_manifest = results[3]
                self.file_id_backup_exe = results[4]
                self.file_id_latest_exe = results[8]

                self.s.settimeout(None)
                return True
            except ConnectionResetError:
                continue
        return False

    def send(self, format, *data):
        payload = struct.pack(format, *data)
        self.s.send(payload)
    
    def recv(self, format):
        sz = struct.calcsize(format)
        buffer = self.s.recv(sz)
        data = struct.unpack(format, buffer)
        return data if len(data) > 1 else data[0]

    def recv_wait(self, format):
        sz = struct.calcsize(format)
        buffer = None
        while True:
            try:
                buffer = self.s.recv(sz)
                break
            except BlockingIOError: time.sleep(.1)
        data = struct.unpack(format, buffer)
        return data if len(data) > 1 else data[0]

    def recv_print(self):
        buf = self.s.recv_wait(0x3000)
        self.buffer += buf

    def send_handshake(self, game = 1):
        return self.send('<BIHHIII', 1, 0, 0xF1, 0x10, game, 0, 0)

    def request_file(self, fileid, version = 0):
        '''
        Request file with specified file hash. 
        Version will request a delta update from specified build to the current version. 
        version=0 will send entire file.
        '''
        self.send('<HHII', 0x3F2, 0xC, fileid, version)

        head, size = self.recv('<HH')
        if head == 0x4F2:
            raise RuntimeError(f'FileServer could not find file ({fileid}) 0x{fileid:X}')
        elif head != 0x5F2:
            raise RuntimeError(f'Unknown header response {head:X}')
            
        recv_fileid, szdcmp, szcmp, crc = self.recv('<IIII')
        return FileRequest(self, recv_fileid, szcmp, szdcmp, crc)

"""

https://www.elitepvpers.com/forum/gw-exploits-hacks-bots-tools-macros/218544-info-client-server-pakete.html

FS_COMPLETE

======Introduction======
This packet is sent upon successful completion of the requested file.

======Packet Format======

^ Name        ^ Size        ^ Value (Hex - Little Endian) ^ Description ^
| Op Code     | [[short]]   | F2 07                       | Op Code |
| Length      | [[short]]   | 08 00 00 00                 | Length of packet |
| File Length | [[long]]    | Varies                      | The length of the data received for the requested file |

======Packet Example======

  F2 07 08 00 00 00 FF FF FF FF
Code:
FS_CONNECT

======Introduction======
This packet initialises the connection to the file server. It is prefixed with 5 bytes.




======Packet Format======

^ Name    ^ Size        ^ Value (Hex - Little Endian) ^ Description ^
| Prefix  | [[char]][5] | 01 00 00 00 00              | Unknown - Has been known to also start with 00 |
| Op Code | [[short]]   | F1 00                       | Op Code |
| Length  | [[short]]   | 10 00 00 00                 | Length of packet |
| Unknown | [[long]]    | 01 00 00 00                 | Unknown |
| Unknown | [[long]]    | 00 00 00 00                 | Unknown |
| Unknown | [[long]]    | 00 00 00 00                 | Unknown |


======Packet Example======

  01 00 00 00 00 F1 00 10 00 01 00 00 00 00 00 00
  00 00 00 00 00
Code:
FS_DETAILS

======Introduction======
This packet is sent in response to a file request from the client. It contains information about the file requested, and will be followed by the files data (See [[FS_RESPONSE]]).

======Packet Format======

^ Name        ^ Size        ^ Value (Hex - Little Endian) ^ Description ^
| Op Code     | [[short]]   | F2 05                       | Op Code |
| Length      | [[short]]   | 14 00                       | Length of packet |
| File Hash   | [[long]]    | Varies                      | Requested File Hash |
| File Length | [[long]]    | Varies                      | Uncompressed Length of file |
| File Length | [[long]]    | Varies                      | Compressed Length of file |
| Unnknown    | [[long]]    | Varies                      | Unknown (CRC?) |

======Packet Example======

  F2 05 14 00 02 10 00 00 01 00 00 00 02 00 00 00
  FF FF FF FF
Code:
FS_NOTFOUND

======Introduction======
This packet is sent in response to a file request which doesn't exist on the server.

======Packet Format======

^ Name      ^ Size        ^ Value (Hex - Little Endian) ^ Description ^
| Op Code   | [[short]]   | F2 04                       | Op Code |
| Length    | [[short]]   | 08 00                       | Length of packet |
| File Hash | [[long]]    | Varies                      | Requested File Hash |

======Packet Example======

  F2 04 08 00 02 10 00 00

Code:
FS_REQUEST

======Introduction======
This packet is sent to request a file from the file server.

======Packet Format======

^ Name      ^ Size        ^ Value (Hex - Little Endian) ^ Description ^
| Op Code   | [[short]]   | F2 03                       | Op Code |
| Length    | [[short]]   | Varies                      | Length of packet |
| File Hash | [[long]]    | Varies                      | Requested File Hash |
| File Hash | [[long]]    | Varies                      | (Optional - Use a value of 0 if requesting only one file) |

Note: Multiple file hashs can be appended to this packet, but a minimum of two is always required.


======Packet Example======

  F2 03 0C 00 02 10 00 00 00 00 00 00
Code:
FS_REQUEST_MORE

======Introduction======
This packet is sent from the client, when the file data received (either from [[FS_RESPONSE]] or [[FS_RESPONSE_MORE]]) doesn't equal the length of the file requested. Hence the name, "request more".

======Packet Format======

^ Name        ^ Size                 ^ Value (Hex - Little Endian) ^ Description ^
| Op Code     | [[short]]            | F3 07                       | Op Code |
| Length      | [[short]]            | 08 00                       | Length of packet |
| Data Length | [[long]]             | Varies                      | The length of the data last received from either [[FS_RESPONSE]] or [[FS_RESPONSE_MORE]] |


======Packet Example======

  F3 07 08 00 FF FF FF FF
Code:
FS_RESPONSE

======Introduction======
This packet is sent in response to a file request. It contains the requested files data (sometimes, only part of it).

If the requested file is larger than 0xFFFB (The length of this packet minus its header) than a [[FS_REQUEST_MORE]] packet must be sent from the client to the file server.

======Packet Format======

^ Name      ^ Size                 ^ Value (Hex - Little Endian) ^ Description ^
| Op Code   | [[short]]            | F2 06                       | Op Code |
| Length    | [[short]]            | Varies                      | Length of packet |
| File Data | [[char]][Length - 4] | Varies                      | Requested File Data |

Note: What is sent should be a power of 2
Code:
FS_RESPONSE_MORE

======Introduction======
This packet is sent in response to a [[FS_REQUEST_MORE]] packet. It contains more of the requested files data.

If the requested file is larger than 0xFFFB (The length of this packet minus its header) than a [[FS_REQUEST_MORE]] packet must be sent from the client to the file server.

======Packet Format======

^ Name      ^ Size                 ^ Value (Hex - Little Endian) ^ Description ^
| Op Code   | [[short]]            | F3 06                       | Op Code |
| Length    | [[short]]            | Varies                      | Length of packet |
| File Data | [[char]][Length - 4] | Varies                      | Requested File Data |


Code:
FS_UPDATE

======Introduction======
This is the first packet sent from the file server to the client. It somehow tells guild wars what files it should be downloading.

One theory is that a hash increments for each build. If the client doesn't have that hash file in it's local DAT file, it will request it.

======Packet Format======

^ Name      ^ Size        ^ Value (Hex - Little Endian) ^ Description ^
| Op Code   | [[short]]   | F2 01                       | Op Code |
| Length    | [[short]]   | Varies                      | Length of packet |
| File Hash | [[long]]    | Varies                      | File Hash(s)... |

Note: Multiple file hashs can be (and are) appended to this packet.


======Packet Example======

  F1 02 18 00 DC 20 05 00 4F 21 05 00 59 21 05 00 5A 21 05 00 5B 21 05 00

"""

if __name__ == '__main__':
    import sys
    client = FileClient()
    client.connect()

    if 2 <= len(sys.argv):
        file_id = int(sys.argv[1])
    else:
        file_id = client.file_id_latest_exe

    fr = client.request_file(file_id)
    for percent in fr.download():
        print("Complete percent: %{:3>f}".format(percent * 100))
    with open(f'{fr.fileid}.download', 'wb') as fd:
        fd.write(fr.decompressed())
