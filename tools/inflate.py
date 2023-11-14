import argparse
import struct
import sys

TABLE1 = [
    (0xa0000000, 2),
    (0x60000000, 6),
    (0x40000000, 10),
    (0x20000000, 18),
    (0x12000000, 25),
    (0x0c000000, 31),
    (0x07000000, 41),
    (0x03000000, 57),
    (0x01600000, 70),
    (0x00f00000, 77),
    (0x00c00000, 83),
    (0x00b00000, 87),
    (0x00a00000, 95),
    (0x00000000, 255),
]

TABLE2 = [
  0x08, 0x09, 0x0A, 0x00, 0x07, 0x0B, 0x0C, 0x06, 0x29, 0x2A, 0xE0, 0x04, 0x05, 0x20, 0x28, 0x2B, 0x2C, 0x40,
  0x4A, 0x03, 0x0D, 0x25, 0x26, 0x27, 0x48, 0x49, 0x24, 0x47, 0x4B, 0x4C, 0x69, 0x6A, 0x23, 0x46, 0x60, 0x63,
  0x67, 0x68, 0x88, 0x89, 0xA0, 0xE8, 0x01, 0x02, 0x2D, 0x43, 0x44, 0x45, 0x65, 0x66, 0x80, 0x87, 0x8A, 0xA8,
  0xA9, 0xC0, 0xC9, 0xE9, 0x0E, 0x4D, 0x64, 0x6B, 0x6C, 0x84, 0x85, 0x8B, 0xA4, 0xA5, 0xAA, 0xC8, 0xE5, 0x83,
  0x86, 0xA6, 0xA7, 0xC7, 0xCA, 0xE7, 0x22, 0x2E, 0x8C, 0xC4, 0xE4, 0xE6, 0x4E, 0x6D, 0xC6, 0xEC, 0x0F, 0x10,
  0x11, 0x8D, 0xAB, 0xAC, 0xCC, 0xEA, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
  0x1E, 0x1F, 0x21, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,
  0x3E, 0x3F, 0x41, 0x42, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C,
  0x5D, 0x5E, 0x5F, 0x61, 0x62, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A,
  0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x81, 0x82, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
  0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA1, 0xA2, 0xA3, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
  0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC1, 0xC2, 0xC3, 0xC5, 0xCB, 0xCD, 0xCE,
  0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE1,
  0xE2, 0xE3, 0xEB, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB,
  0xFC, 0xFD, 0xFE, 0xFF,
]

TABLE3 = [
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x14, 0x18, 0x1C,
    0x20, 0x28, 0x30, 0x38, 0x40, 0x50, 0x60, 0x70, 0x80, 0xA0, 0xC0, 0xE0, 0xFF, 0x00, 0x00, 0x00,
]

EXTRA_BITS_LENGTH = [
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
    0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x00,
]

EXTRA_BITS_DISTANCE = [
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06,
    0x07, 0x07, 0x08, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B, 0x0C, 0x0C, 0x0D, 0x0D, 0x0E, 0x0E,
]

BACKTRACK_TABLE = [
    0x0000, 0x0001, 0x0002, 0x0003,
    0x0004, 0x0006, 0x0008, 0x000C,
    0x0010, 0x0018, 0x0020, 0x0030,
    0x0040, 0x0060, 0x0080, 0x00C0,
    0x0100, 0x0180, 0x0200, 0x0300,
    0x0400, 0x0600, 0x0800, 0x0C00,
    0x1000, 0x1800, 0x2000, 0x3000,
    0x4000, 0x6000, 0x0100, 0x0302,
    0x0504, 0x0706, 0x0A08, 0x0E0C,
    0x1410, 0x1C18, 0x2820, 0x3830,
    0x5040, 0x7060, 0xA080, 0xE0C0,
    0x00FF, 0x0000,
]

def u32(v):
    return v & 0xFFFFFFFF

class BitStream:
    def __init__(self, input):
        assert(2 <= len(input))
        self.input = input
        self.buf1, = struct.unpack_from('<I', input, 0)
        self.buf2, = struct.unpack_from('<I', input, 4)
        self.idx = 8
        self.avail = 32
        self.is_empty = False

    def peek(self, count):
        assert(count <= 32)
        return self.buf1 >> (32 - count)

    def read(self, count):
        res = self.peek(count)
        self.consume(count)
        return res

    def consume(self, count):
        self.buf1 = (self.buf2 >> (32 - count)) | u32(self.buf1 << count)
        if self.avail < count:
            if self.idx == len(self.input):
                self.is_empty = True
                self.avail = 0
                self.buf2 = 0
            else:
                self.buf2, = struct.unpack_from('<I', self.input, self.idx)
                self.idx += 4
                new_avail = (self.avail + 32) - count
                self.buf1 |= self.buf2 >> new_avail
                self.buf2 = u32(self.buf2 << (count - self.avail))
                self.avail = new_avail
        else:
            self.avail -= count
            self.buf2 = u32(self.buf2 << count)


class HuffmanTable:
    def __init__(self):
        self.nodes = [(0, 0)] * 256

    def get_next_code(self, stream):
        bits = stream.peek(8)
        enc_len, enc_val = self.nodes[bits]
        if enc_len == 0xFFFFFFFF:
            buf1 = stream.peek(32)
            for (first_encoding, last_index, enc_len) in self.large_symbol_translation:
                if first_encoding <= buf1:
                    break
            else:
                raise RuntimeError("Isn't possible")
            group_idx = (buf1 - first_encoding) >> (32 - enc_len)
            large_enc_idx = last_index - group_idx
            assert(large_enc_idx < len(self.large_symbol_values))
            enc_val = self.large_symbol_values[large_enc_idx]

        stream.consume(enc_len)
        return enc_val


def build_huffman_table(stream):
    symbol_follow_table_root = [0xFFFFFFFF] * 32

    symbol_count = stream.read(16)
    symbol_follow_table = [0] * symbol_count
    total_symbol_count = 0

    symbol_idx = symbol_count - 1
    while symbol_idx != -1:
        buf1 = stream.peek(32)
        for (idx, (first_enc, last_index)) in enumerate(TABLE1):
            if first_enc <= buf1:
                break
        else:
            raise RuntimeError("Shouldn't be possible")

        bit_count = idx + 3
        offset = (buf1 - first_enc) >> (32 - bit_count)
        stream.consume(bit_count)

        temp = TABLE2[last_index - offset]
        number_of_symbol = temp >> 5
        symbol_len = temp & 0x1F


        if symbol_len != 0 or symbol_count < 2:
            number_of_symbol += 1
            total_symbol_count += number_of_symbol
            for _ in range(number_of_symbol):
                symbol_follow_table[symbol_idx] = symbol_follow_table_root[symbol_len]
                symbol_follow_table_root[symbol_len] = symbol_idx
                symbol_idx -= 1
        else:
            symbol_idx -= number_of_symbol + 1

    if number_of_symbol != 0 and total_symbol_count == 0:
        symbol_follow_table[number_of_symbol - 1] = symbol_follow_table_root[0]
        symbol_follow_table_root[0] = number_of_symbol - 1
        total_symbol_count = 1

    huffman = HuffmanTable()

    # Build Huffman table for codelen 1-8
    next_bits_encoding = 1
    symbol_in_huffman_table = 0
    for enc_len in range(1, 9):
        current_symbol = symbol_follow_table_root[enc_len]
        while current_symbol != 0xFFFFFFFF:
            assert(current_symbol < symbol_count)
            assert(next_bits_encoding < (1 << enc_len))

            first_symbol = next_bits_encoding << (8 - enc_len)
            iter_count = 1 << (8 - enc_len)

            for idx in range(first_symbol, first_symbol + iter_count):
                huffman.nodes[idx] = (enc_len, current_symbol)

            current_symbol = symbol_follow_table[current_symbol]
            symbol_in_huffman_table += 1
            next_bits_encoding -= 1

        next_bits_encoding = (next_bits_encoding << 1) + 1

    assert(symbol_in_huffman_table <= total_symbol_count)
    if (symbol_in_huffman_table == total_symbol_count):
        return huffman

    large_symbol_count = total_symbol_count - symbol_in_huffman_table
    huffman.large_symbol_translation = [(0, 0, 0)] * 24
    huffman.large_symbol_values = []
    for enc_len in range(9, 32):
        current_symbol = symbol_follow_table_root[enc_len]
        while current_symbol != 0xFFFFFFFF:
            assert(current_symbol < symbol_count)
            assert(next_bits_encoding < (1 << enc_len))

            partial_encoding = next_bits_encoding >> (enc_len - 8)
            huffman.nodes[partial_encoding] = (0xFFFFFFFF, 0)
            huffman.large_symbol_values.append(current_symbol)
            current_symbol = symbol_follow_table[current_symbol]
            next_bits_encoding -= 1

        first_encoding = (next_bits_encoding + 1) << (32 - enc_len)
        last_index = len(huffman.large_symbol_values) - 1
        huffman.large_symbol_translation[enc_len - 9] = (first_encoding, last_index, enc_len)

        next_bits_encoding = (next_bits_encoding << 1) + 1

    return huffman


def inflate(input):
    stream = BitStream(input)
    stream.consume(4)
    first_4_bits = stream.read(4)

    output = []
    while not stream.is_empty:
        lit_huffman = build_huffman_table(stream)
        dist_huffman = build_huffman_table(stream)

        block_size = (stream.read(4) + 1) * 4096
        for _ in range(block_size):
            # It's perfectly possible that the steam is over before the block size.
            if stream.is_empty:
                break

            code = lit_huffman.get_next_code(stream)
            if code < 0x100:
                output.append(code)
            else:
                blen = EXTRA_BITS_LENGTH[code - 256]
                code = TABLE3[code - 256]

                if blen:
                    code = code | stream.read(blen)

                backtrack_count = first_4_bits + code + 1
                code = dist_huffman.get_next_code(stream)
                blen = EXTRA_BITS_DISTANCE[code]
                backtrack = BACKTRACK_TABLE[code]

                if blen:
                    backtrack = backtrack | stream.read(blen)

                assert(backtrack < len(output))
                src = len(output) - (backtrack + 1)
                for i in range(src, src + backtrack_count):
                    output.append(output[i])

    return bytes(output)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', '-o', type=str, required=False,
        help="Path of the output file")
    parser.add_argument('--input', '-i', type=str, required=False,
        help="Path of the input file")
    args = parser.parse_args()

    if args.input:
        input = open(args.input, 'rb').read()
    else:
        input = sys.stdin.read()

    result = inflate(input)

    if args.output:
        open(args.output, 'wb').write(result)
    else:
        sys.stdout.buffer.write(result)
