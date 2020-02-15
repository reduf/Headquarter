import sys

def fatal(*args, **kw):
    print(*args, **kw)
    sys.exit(1)

def nextword(str):
    str = str.lstrip()
    pos = str.find(' ')
    if pos == -1:
        pos = len(str)
    return str[:pos]

def main(args):

    if len(args) < 1:
        fatal("Specify file to process")
    
    file_path = args[0]
    with open(file_path, "r") as f:
        data = f.read()

    offset = 0
    lines = data.splitlines()

    for line in lines:
        # We conserve empty lines
        if len(line) == 0:
            print('')
            continue

        if line.startswith('//'):
            str = nextword(line[2:])
            if len(str) >= 2 and (str[0] == '+' or str[0] == '-'):
                sign = str[0]
                val  = str[1:]
                offset = int(val) if sign == '+' else -int(val)
                continue

        if not line.startswith('#define'):
            print(line)
            continue

        # format is: "#define name (hex) // dec"
        words = line.split()
        if len(words) != 5:
            print(line)
            continue

        # if the hex_val is not the same as the dec_val we notice it, but base our result on the hex
        hex_val = int(words[2][3:-1], 16)
        dec_val = int(words[4], 10)
        if (hex_val & 0x7FFF) != (dec_val & 0x7FFF):
            print('// @@@@@@@ Error: hex val != dec val')

        val = hex_val + offset
        print('#define', '%-43s' % words[1], '(0x%04X) // %d' % (val, val & 0x7FFF))

if __name__ == '__main__':
    main(sys.argv[1:])
