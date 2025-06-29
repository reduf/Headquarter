#include <assert.h>
#include <memory.h>
#include <stdio.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))

typedef struct Table1Type
{
    unsigned int FirstEncoding;
    unsigned int LastIndex;
} Table1Type;

// Table1 is only used at table construction timne.
Table1Type Table1[28] = {
    {0xa0000000, 2},   // 3   0b10100000000000000000000000000000                                  (3)
    {0x60000000, 6},   // 4   0b01100000000000000000000000000000 10011111111111111111111111111111 (4)
    {0x40000000, 10},  // 4   0b01000000000000000000000000000000 01011111111111111111111111111111 (5)
    {0x20000000, 18},  // 8   0b00100000000000000000000000000000 00111111111111111111111111111111 (6)
    {0x12000000, 25},  // 7   0b00010010000000000000000000000000 00011111111111111111111111111111 (7)
    {0x0c000000, 31},  // 6   0b00001100000000000000000000000000 00010001111111111111111111111111 (8)
    {0x07000000, 41},  // 10  0b00000111000000000000000000000000 00001011111111111111111111111111 (9)
    {0x03000000, 57},  // 16  0b00000011000000000000000000000000 00000110111111111111111111111111 (10)
    {0x01600000, 70},  // 13  0b00000001011000000000000000000000 00000010111111111111111111111111 (11)
    {0x00f00000, 77},  // 7   0b00000000111100000000000000000000 00000001010111111111111111111111 (12)
    {0x00c00000, 83},  // 6   0b00000000110000000000000000000000 00000000111011111111111111111111 (13)
    {0x00b00000, 87},  // 4   0b00000000101100000000000000000000 00000000101111111111111111111111 (14)
    {0x00a00000, 95},  // 8   0b00000000101000000000000000000000 00000000101011111111111111111111 (15)
    {0x00000000, 255}, // 160 0b00000000000000000000000000000000 00000000100111111111111111111111 (16)
};

// This table encode all possibility of the tuple (number of code of specific length)
// The number of code is in the range [0, 7] and the code length is between [0, 31].
//
// It's unclear how this table is ordered, but my guess is:
// -  It takes less bytes to encode the lower index, due to how `Table1` is ordered.
// => That would mean that the first entry in `Table2` are the most common. i.e.,
//    (1 code on 8 bits, 1 code on 9 bits, 1 code on 10 bits)
//
// Overall, it allows the server to send an index in this array to specify the number
// of code of a given len. From that, we can infer the encoding of the encoding.
unsigned char Table2[256] = {
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
};

unsigned char Table3[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x14, 0x18, 0x1C,
    0x20, 0x28, 0x30, 0x38, 0x40, 0x50, 0x60, 0x70, 0x80, 0xA0, 0xC0, 0xE0, 0xFF, 0x00, 0x00, 0x00,
};

unsigned char s_extraBitsLength[29] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
    0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x00,
};

unsigned char s_extraBitsDistance[32] = {
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06,
    0x07, 0x07, 0x08, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B, 0x0C, 0x0C, 0x0D, 0x0D, 0x0E, 0x0E,
};

unsigned short BacktrackTable[46] = {
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
};

typedef struct HuffmanNode
{
    unsigned int len;
    unsigned int code;
} HuffmanNode;

typedef struct HuffmanHelper
{
    // All code encoded on the length specified in this structure start with
    // this specified encoding. So, you can find the offset from the first.
    unsigned int FirstEncoding;
    // This value is the index for the first code encoded.
    // The subsequent code in this group are simply `LastIndex - val`.
    unsigned int LastIndex;
    unsigned int EncodingLen;
} HuffmanHelper;

typedef struct HuffmanData
{
    HuffmanNode    HuffmanTable[0x100];
    HuffmanHelper  LargeEncodingTable[24]; // 32 - 8
    unsigned int  *CodesForLargeEncoding;
    unsigned int   NumberOfLargeEncoding;
} HuffmanData;

typedef struct DecompressState
{
    unsigned int BitsAvailable;
    unsigned int Buf1;
    unsigned int Buf2;

    unsigned int *CurInputData;
    unsigned int *InputDataEnd;
} DecompressState;

void ConsumeBits(DecompressState *State, unsigned int bitsCount)
{
    assert(bitsCount <= (sizeof(State->Buf1) * 8));
    State->Buf1 = (State->Buf2 >> (32 - bitsCount)) | (State->Buf1 << bitsCount);

    if (State->BitsAvailable < bitsCount)
    {
        if (State->CurInputData == State->InputDataEnd)
        {
            State->BitsAvailable = 0;
            State->Buf2 = 0;
        }
        else
        {
            State->Buf2 = State->CurInputData[0];
            ++State->CurInputData;

            unsigned int NewBitsAvailable = (State->BitsAvailable + 32) - bitsCount;

            State->Buf1 |= (State->Buf2 >> NewBitsAvailable);
            State->Buf2 = State->Buf2 << (bitsCount - State->BitsAvailable);
            State->BitsAvailable = NewBitsAvailable;
        }
    }
    else
    {
        State->BitsAvailable -= bitsCount;
        State->Buf2 = State->Buf2 << bitsCount;
    }
}

unsigned int GetBits(DecompressState *State, unsigned int bitsCount)
{
    assert(bitsCount <= (sizeof(State->Buf1) * 8));
    return State->Buf1 >> (32 - bitsCount);
}

unsigned int GetNextCode(DecompressState *State, HuffmanData *HData)
{
    // We always read the next 8 bits, but they may not all be used.
    // So, we don't consume right away.
    unsigned int nextBits = GetBits(State, 8);

    unsigned int encodingLen = HData->HuffmanTable[nextBits].len;
    unsigned int code = HData->HuffmanTable[nextBits].code;

    // If the `encodingLen` is 0xFFFFFFFF, the current encoding is more than 8 bits.
    // The code is the first 8 bits of it, but we gotta find the actual value.
    if (encodingLen == 0xFFFFFFFF) {
        //unsigned int nextWord = GetBits(State, 32);

        int idx;
        for (idx = 0; idx < ARRAY_SIZE(HData->LargeEncodingTable); ++idx) {
            if (HData->LargeEncodingTable[idx].FirstEncoding <= State->Buf1)
                break;
        }

        // It shouldn't be possible to not find any entry.
        assert(idx != ARRAY_SIZE(HData->LargeEncodingTable));

        // This is the actual encoding length.
        encodingLen = HData->LargeEncodingTable[idx].EncodingLen;

        // We generally have multiple code with an encoding of the same length, but all encoding
        // are a just incrementation and we know the first one for this length, i.e. `FirstEncoding`.
        // This give us the offset in the group.
        unsigned int groupIdx = (State->Buf1 - HData->LargeEncodingTable[idx].FirstEncoding) >> (32 - encodingLen);

        // Determine where the code for this encoding is saved.
        unsigned int largeEncIdx = HData->LargeEncodingTable[idx].LastIndex - groupIdx;

        assert(largeEncIdx < HData->NumberOfLargeEncoding);
        code = HData->CodesForLargeEncoding[largeEncIdx];
    }

    assert(encodingLen <= 32 && "Code encoded on more than 32 bits shouldn't be possible");

    if (encodingLen != 0)
    {
        ConsumeBits(State, encodingLen);
    }

    return code;
}

bool SetupNodesandTree(DecompressState* State, HuffmanData* HData)
{
    // Index of the table: The length of the code
    // Value of the table: Count the number of code encoded with the length being the idx.
    unsigned int CodeLenOccurrence[32];

    // Index of the table: The length of the code
    // Value of the table: The code (idx) of the next code encoded on the same length. 
    unsigned int SymbolsFollowTableRoot[32];

    unsigned int TotalSymbolCount = 0;

    HData->CodesForLargeEncoding = 0;
    HData->NumberOfLargeEncoding = 0;

    const unsigned int SymbolsCount = GetBits(State, 16);
    ConsumeBits(State, 16);

    // Index of the table: Value of a code.
    // Value of the table: Index (same as value) of the next code with same length.
    unsigned int *SymbolsFollowTable = malloc(SymbolsCount * sizeof(*SymbolsFollowTable));
    memset(SymbolsFollowTable, 0, SymbolsCount * 4);

    memset(CodeLenOccurrence, 0, sizeof(CodeLenOccurrence));
    memset(SymbolsFollowTableRoot, 0xff, sizeof(SymbolsFollowTableRoot));

    for (unsigned int CurSymbol = SymbolsCount - 1; CurSymbol < SymbolsCount;)
    {
        int idx;
        for (idx = 0; ARRAY_SIZE(Table1); ++idx) {
            if (Table1[idx].FirstEncoding <= State->Buf1)
                break;
        }

        // This assert can't trigger, because the value for the last entry of the table is 0.
        assert(idx != ARRAY_SIZE(Table1));

        // Not totally clear why the +3 here.
        unsigned int bitCount = idx + 3;
        unsigned int offset = (State->Buf1 - Table1[idx].FirstEncoding) >> (32 - bitCount);
        ConsumeBits(State, bitCount);

        int temp = Table2[Table1[idx].LastIndex - offset];
        int numberOfCode = temp >> 5; // This is the number of code of this length
        int codeLen = temp & 0x1f;    // This is the length of this code 


        assert(numberOfCode <= CurSymbol);

        if ((codeLen != 0) || SymbolsCount < 2)
        {
            CodeLenOccurrence[codeLen] += numberOfCode + 1;
            TotalSymbolCount += numberOfCode + 1;

            for (int i = 0; i < numberOfCode + 1; ++i) {
                SymbolsFollowTable[CurSymbol] = SymbolsFollowTableRoot[codeLen];
                SymbolsFollowTableRoot[codeLen] = CurSymbol;
                CurSymbol--;
            }
        }
        else
        {
            CurSymbol -= numberOfCode + 1;
        }
    }

    // processhuffmantree

    if (SymbolsCount != 0 && TotalSymbolCount == 0)
    {
        SymbolsFollowTable[SymbolsCount - 1] = SymbolsFollowTableRoot[0];
        SymbolsFollowTableRoot[0] = SymbolsCount - 1;
        CodeLenOccurrence[0] = 1;
        TotalSymbolCount = 1;
    }

    memset(HData->HuffmanTable, 0, sizeof(HData->HuffmanTable));

    unsigned int SymbolsInHuffmanTable = 0;

    // The idea for this loop is pretty simple.
    // We can find all code encoded in N bits using the table we built above.
    // So for all encoding length, we create a unique pattern `nextBitEncoding` of the appropriate length.
    // e.g.,
    // Length 1: there is 0 or 1
    // Length 2: there is 00, 01, 10, and 10.
    // etc...
    // We fill all the entry in the `HuffmanTable` that starts with the bit pattern. The idea is that
    // if we used the pattern of length 2 01, we want all index 01XXXXXX to decode to the same code
    // and to tell how many bits are used, in this case 2.
    // @Remark: We can't have the pattern 01 and 011 or 01 and 010 as it would create an ambiguous encoding
    //
    // This is why the "nextBitEncoding" can be re-used as long as we don't exhaust all possibilities.
    // For instance, we can encode on 1 bit using 1, but 0 needs to not be used.
    // Then, on 2 bits, we can encode 01, but not 00 (and we can't use 10 or 11, because 1 is already an encoding)
    unsigned int nextBitEncoding = 0;
    for (unsigned int encodingLen = 0; encodingLen < 9; ++encodingLen)
    {
        unsigned int currentSymbol = SymbolsFollowTableRoot[encodingLen];
        while (currentSymbol != 0xFFFFFFFF)
        {
            assert(currentSymbol < SymbolsCount); // Shouldn't be possible based on how we build it.

            // If we reach the last encoding, we can't loop anymore, otherwise we will
            // an ambiguous create encoding. It shouldn't ever be the case, otherwise we can't
            // decompress the document.
            if ((1 << encodingLen) <= nextBitEncoding)
            {
                //return here too
                free(SymbolsFollowTable);
                return true;
            }

            // We have an encoding, for instance 0b1101 (4 bits), so we want to mark all index
            // of the format 0b1101XXXX. The initial index is 0b11010000 and the last one is
            // 0b11011111, which mean we have 0b10000 iteration. (1 << 4)
            int indexStart = nextBitEncoding << (8 - encodingLen);
            int iterCount = (1 << (8 - encodingLen));

            assert(indexStart + iterCount <= 256);
            assert((indexStart & (iterCount - 1)) == 0);

            for (int i = indexStart; i < (indexStart + iterCount); ++i) {
                HData->HuffmanTable[i].len = encodingLen;
                HData->HuffmanTable[i].code = currentSymbol;
            }

            // Lookup the next code of same length.
            currentSymbol = SymbolsFollowTable[currentSymbol];

            ++SymbolsInHuffmanTable;
            --nextBitEncoding;
        }

        nextBitEncoding = (nextBitEncoding << 1) + 1;
    }


    assert(SymbolsInHuffmanTable <= TotalSymbolCount);

    if (SymbolsInHuffmanTable < TotalSymbolCount)
    {
        // We wrote in the Huffman table all code that could be encoded on 8 bits, but we might have some
        // that can't be encoded on 8 bits.

        unsigned int NumberOfRemainingCode = TotalSymbolCount - SymbolsInHuffmanTable;

        HData->NumberOfLargeEncoding = NumberOfRemainingCode;
        HData->CodesForLargeEncoding = malloc(NumberOfRemainingCode * sizeof(*HData->CodesForLargeEncoding));
        memset(HData->CodesForLargeEncoding, 0, NumberOfRemainingCode * 4);

        unsigned int BigEncodingCounter = 0;
        for (unsigned int encodingLen = 9; encodingLen < 32; ++encodingLen)
        {
            unsigned int code = SymbolsFollowTableRoot[encodingLen];
            while (code != 0xFFFFFFFF)
            {
                assert(code < SymbolsCount); // Shouldn't be possible based on how we build it.
                if (((1 << encodingLen)) <= nextBitEncoding)
                {
                    //return here too
                    free(SymbolsFollowTable);
                    return true;
                }

                // If the encoding for this code doesn't fit on 8 bits, we set the
                // the length to 0xFFFFFFFF which is used later to distinguish incomplete
                // encoding.
                unsigned int partialEncoding = (nextBitEncoding >> (encodingLen - 8));
                HData->HuffmanTable[partialEncoding].len = 0xFFFFFFFF;

                if (HData->NumberOfLargeEncoding <= BigEncodingCounter)
                {
                    free(SymbolsFollowTable);
                    return false;
                }

                HData->CodesForLargeEncoding[BigEncodingCounter] = code;
                BigEncodingCounter++;

                code = SymbolsFollowTable[code];
                nextBitEncoding--;
            }

            HuffmanHelper *CurHelperArray = &HData->LargeEncodingTable[encodingLen - 9];
            CurHelperArray->FirstEncoding = (nextBitEncoding + 1) << (32 - encodingLen);
            CurHelperArray->LastIndex = BigEncodingCounter - 1;
            CurHelperArray->EncodingLen = encodingLen;

            nextBitEncoding = (nextBitEncoding << 1) + 1;
        }
    }

    free(SymbolsFollowTable);
    return true;
}

unsigned char* DecompressFile(unsigned char *Input, int InputSize, int OutputSize)
{
    DecompressState State = {0};
    State.CurInputData = (unsigned int *)Input;
    State.InputDataEnd = (unsigned int *)(Input + InputSize);

    unsigned int code = 0;

    HuffmanData LiteralCodeTable;
    memset(&LiteralCodeTable, 0, sizeof(LiteralCodeTable));

    HuffmanData DistanceCodeTable;
    memset(&DistanceCodeTable, 0, sizeof(DistanceCodeTable));

    // this part came from cmpdecompress
    State.Buf1 = State.CurInputData[0];
    State.Buf2 = State.CurInputData[1];
    State.BitsAvailable = 32;
    State.CurInputData += 2;

    ConsumeBits(&State, 4); // Ignore the first 4 bits (is it always 0b1111 anyway?)

    //cmpdecompress part ends here

    unsigned char* Output = malloc(OutputSize);
    memset(Output, 0, OutputSize);
    unsigned char* ptrOutput = Output;
    unsigned char* ptrOutputEnd = Output + OutputSize;

    // This is used as some kind of offset when going through the "backtracking"
    int first4Bits = GetBits(&State, 4);
    ConsumeBits(&State, 4);

    while (ptrOutputEnd != ptrOutput)
    {
        if (!SetupNodesandTree(&State, &LiteralCodeTable))
        {
            if (LiteralCodeTable.CodesForLargeEncoding)
            {
                free(LiteralCodeTable.CodesForLargeEncoding);
            }
            free(Output);
            return 0;
        }

        if (!SetupNodesandTree(&State, &DistanceCodeTable))
        {
            if (DistanceCodeTable.CodesForLargeEncoding)
            {
                free(DistanceCodeTable.CodesForLargeEncoding);
            }
            free(Output);
            return 0;
        }


        // In the file, 0 = 1KB, 1 = 2KBs, ...
        unsigned int blockInKbs = GetBits(&State, 4) + 1;
        unsigned int blockSize = blockInKbs * 4096;
        ConsumeBits(&State, 4);

        for (unsigned int loopCount = 0; loopCount < blockSize; ++loopCount)
        {
            if (ptrOutput == ptrOutputEnd)
                break;

            code = GetNextCode(&State, &LiteralCodeTable);

            if (code < 0x100)
            {
                // If we decoded a byte, simply write it to the output.
                *ptrOutput++ = code;
            }
            else
            {
                // If the decoded code is not a byte, it means that we re-copy
                // using a back-pointer.
                assert((code - 0x100) < ARRAY_SIZE(s_extraBitsLength) && "The data is corrupt or the table weren't completely dump for the binary and we are relying on out of bond access");

                unsigned int len = s_extraBitsLength[code - 0x100];
                code = Table3[code - 0x100];

                if (len)
                {
                    code |= GetBits(&State, len);
                    ConsumeBits(&State, len);
                }

                unsigned int backtrackCount = first4Bits + code + 1;
                code = GetNextCode(&State, &DistanceCodeTable);

                len = s_extraBitsDistance[code];
                int backtrack = BacktrackTable[code];

                if (len)
                {
                    backtrack = backtrack | GetBits(&State, len);
                    ConsumeBits(&State, len);
                }

                assert((ptrdiff_t)backtrack < (ptrdiff_t)(ptrOutput - Output));
                assert((ptrdiff_t)backtrackCount <= (ptrdiff_t)(ptrOutputEnd - ptrOutput));

                unsigned char *src = ptrOutput - (backtrack + 1);
                unsigned char *dst = ptrOutput;

                for (unsigned int i = 0; i < backtrackCount; ++i) {
                    dst[i] = src[i];
                }

                ptrOutput += backtrackCount;
            }

            code = 0;
        }

        if (DistanceCodeTable.CodesForLargeEncoding)
        {
            free(DistanceCodeTable.CodesForLargeEncoding);
        }

        DistanceCodeTable.CodesForLargeEncoding = 0;
        DistanceCodeTable.NumberOfLargeEncoding = 0;

        if (LiteralCodeTable.CodesForLargeEncoding)
        {
            free(LiteralCodeTable.CodesForLargeEncoding);
        }

        LiteralCodeTable.CodesForLargeEncoding = 0;
        LiteralCodeTable.NumberOfLargeEncoding = 0;

    }

    return Output;
}
