import sys
import struct

try:
    import pefile
except:
    print("Install pefile 'pip install pefile'")
    sys.exit(1)

class FileScanner:
    def __init__(self, path, section_name = '.text'):
        self.sections = []
        section_name = section_name.encode('ascii')
        self.parsed = pefile.PE(path, fast_load=True)
        for section in self.parsed.sections:
            self.sections.append((section, section.get_data()))
            stripped_name = section.Name.strip(b'\0')
            if stripped_name == section_name:
                self.text_idx = len(self.sections) - 1

    def find(self, pattern, offset = 0):
        section, buffer = self.sections[self.text_idx]
        pos = buffer.find(pattern)
        if pos == -1:
            raise RuntimeError("Couldn't find the pattern")
        return section.VirtualAddress + pos + offset

    def get_section_by_rva(self, rva):
        for section, buffer in self.sections:
            if section.contains_rva(rva):
                return section, buffer
        raise RuntimeError("Couldn't find the section with given RVA")

    def follow_call(self, call_rva):
        section, buffer = self.get_section_by_rva(call_rva)
        pos_in_section = call_rva - section.VirtualAddress
        op, call_param = struct.unpack_from('<BI', buffer, pos_in_section)
        if op != 0xE8 and op != 0xE9:
            raise RuntimeError(f"Unsupported opcode '0x{op:02X} ({op})'")
        return call_rva + call_param + 5

    def read(self, rva, format = '<I'):
        section, buffer = self.get_section_by_rva(rva)
        pos_in_section = rva - section.VirtualAddress
        return struct.unpack_from(format, buffer, pos_in_section)

