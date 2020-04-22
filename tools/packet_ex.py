from process import *
import sys

TYPE_MSG_HEADER     = 1
TYPE_AGENT_ID       = 2
TYPE_FLOAT          = 3
TYPE_VECT2          = 4
TYPE_VECT3          = 5
TYPE_BYTE           = 6
TYPE_WORD           = 7
TYPE_DWORD          = 8
TYPE_BLOB           = 9
TYPE_STRING_16      = 10
TYPE_ARRAY_8        = 11
TYPE_ARRAY_16       = 12
TYPE_ARRAY_32       = 13
TYPE_NESTED_STRUCT  = 14

TYPE_MAX = 14

SIZE_T_SIZE = 4

def die(*args, **kw):
    print('[ERROR]', *args, **kw)
    sys.exit(1)

def gw_type_to_my_type(value):
    type =  (value >> 0) & 15
    size =  (value >> 4) & 15
    count = (value >> 8) & 15

    if (type < 0) or (12 < type):
        die("Type '%d' out of range." % type)

    if type == 0: return TYPE_AGENT_ID
    if type == 1: return TYPE_FLOAT
    if type == 2: return TYPE_VECT2
    if type == 3: return TYPE_VECT3
    if type == 4 or type == 8:
        if count == 1: return TYPE_BYTE
        if count == 2: return TYPE_WORD
        if count == 4: return TYPE_DWORD
    if type == 5 or type == 9:
        return TYPE_BLOB
    if type == 6 or type == 10:
        return None
    if type == 7: return TYPE_STRING_16
    if type == 11:
        if size == 0: return TYPE_ARRAY_8
        if size == 1: return TYPE_ARRAY_16
        if size == 2: return TYPE_ARRAY_32
    if type == 12: return TYPE_NESTED_STRUCT

def size_of_my_type(type, param):
    if type == TYPE_MSG_HEADER:     return 2
    if type == TYPE_AGENT_ID:       return 4
    if type == TYPE_FLOAT:          return 4
    if type == TYPE_VECT2:          return 8
    if type == TYPE_VECT3:          return 12
    if type == TYPE_BYTE:           return 1
    if type == TYPE_WORD:           return 2
    if type == TYPE_DWORD:          return 4
    if type == TYPE_BLOB:           return param
    if type == TYPE_STRING_16:      return param * 2
    if type == TYPE_ARRAY_8:        return param * 1
    if type == TYPE_ARRAY_16:       return param * 2
    if type == TYPE_ARRAY_32:       return param * 4
    if type == TYPE_NESTED_STRUCT:  return 0
    raise RuntimeError("This type '%d' has no size." % type)

def name_of_my_type(type):
    if type == TYPE_MSG_HEADER:     return 'TYPE_MSG_HEADER'
    if type == TYPE_AGENT_ID:       return 'TYPE_AGENT_ID'
    if type == TYPE_FLOAT:          return 'TYPE_FLOAT'
    if type == TYPE_VECT2:          return 'TYPE_VECT2'
    if type == TYPE_VECT3:          return 'TYPE_VECT3'
    if type == TYPE_BYTE:           return 'TYPE_BYTE'
    if type == TYPE_WORD:           return 'TYPE_WORD'
    if type == TYPE_DWORD:          return 'TYPE_DWORD'
    if type == TYPE_BLOB:           return 'TYPE_BLOB'
    if type == TYPE_STRING_16:      return 'TYPE_STRING_16'
    if type == TYPE_ARRAY_8:        return 'TYPE_ARRAY_8'
    if type == TYPE_ARRAY_16:       return 'TYPE_ARRAY_16'
    if type == TYPE_ARRAY_32:       return 'TYPE_ARRAY_32'
    if type == TYPE_NESTED_STRUCT:  return 'TYPE_NESTED_STRUCT'
    raise RuntimeError("The type '%d' has no name." % type)

def my_type_has_prefix(type):
    if type == TYPE_MSG_HEADER:     return False
    if type == TYPE_AGENT_ID:       return False
    if type == TYPE_FLOAT:          return False
    if type == TYPE_VECT2:          return False
    if type == TYPE_VECT3:          return False
    if type == TYPE_BYTE:           return False
    if type == TYPE_WORD:           return False
    if type == TYPE_DWORD:          return False
    if type == TYPE_BLOB:           return False

    if type == TYPE_STRING_16:      return True
    if type == TYPE_ARRAY_8:        return True
    if type == TYPE_ARRAY_16:       return True
    if type == TYPE_ARRAY_32:       return True
    if type == TYPE_NESTED_STRUCT:  return True
    raise RuntimeError("The type '%d' is unknow." % type)

class Field(object):

    def __init__(self, type, param = 0):
        self.type = type
        self.param = param

        self.size = 0
        if my_type_has_prefix(type):
            self.size += SIZE_T_SIZE
        self.size += size_of_my_type(type, param)

    def __str__(self):
        return '{%s, %d}' % (name_of_my_type(self.type), self.param)

    def from_int(value):
        type =  (value >> 0) & 0xF
        size =  (value >> 4) & 0xF
        count = (value >> 8) & 0xFFFF

        if type == 0: return Field(TYPE_AGENT_ID)
        if type == 1: return Field(TYPE_FLOAT)
        if type == 2: return Field(TYPE_VECT2)
        if type == 3: return Field(TYPE_VECT3)
        if type == 4 or type == 8:
            if count == 1: return Field(TYPE_BYTE)
            if count == 2: return Field(TYPE_WORD)
            if count == 4: return Field(TYPE_DWORD)
        if type == 5 or type == 9:
            return Field(TYPE_BLOB, count)
        if type == 6 or type == 10:
            # ARRAY_END
            return None
        if type == 7: return Field(TYPE_STRING_16, count)
        if type == 11:
            if size == 0: return Field(TYPE_ARRAY_8, count)
            if size == 1: return Field(TYPE_ARRAY_16, count)
            if size == 2: return Field(TYPE_ARRAY_32, count)
        if type == 12: return Field(TYPE_NESTED_STRUCT, count)

        die("Unknow type '%d' {size: %d, count: %d}" % (type, size, count))

class Descriptor(object):

    def __init__(self, header, handler = None):
        self.header = header
        self.handler = handler

        self.fields = [Field(TYPE_MSG_HEADER, header)]
        self.size = 2

        self.has_nested_struct = False

    def push_field(self, field):
        self.fields.append(field)
        self.size += field.size

    def print(self, prefix):
        str_fields = []
        for f in self.fields:
            str_fields.append(str(f))
        
        field_count = self.count_field()
        seperator = ',\n\t'

        if not self.handler is None:
            print('// Handler: %08X' % self.handler)
        print('MsgField %s_%04d[%d] = {' % (prefix, self.header, field_count))

        s = '\t' + seperator.join(str_fields) + ','
        print(s)

        str_fields = []
        if self.has_nested_struct:
            for f in self.nested_struct_fields:
                str_fields.append(str(f))
            s = '\t' + seperator.join(str_fields) + ','
            print(s)

        print('};\n')

    def count_field(self):
        count = 0
        for field in self.fields:
            count += 1
        if self.has_nested_struct:
            for field in self.nested_struct_fields:
                count += 1
        return count

    def compute_size(self):
        self.size = 0
        for field in self.fields:
            self.size += field.size
        return self.size

def decode_msg_description(msg_id, gw_fields, handler = None):

    def decode_nested_struct(desc, struct_fields):
        """`struct_fields` must be an array of int32"""
        struct_size = 0 # size of struct 1-align

        fields = []
        for value in struct_fields[1:]:
            f = Field.from_int(value)
            struct_size += f.size
            fields.append(f)
        
        desc.nested_struct_size = struct_size
        desc.nested_struct_fields = fields


    assert len(gw_fields) > 0
    desc = Descriptor(msg_id, handler)

    for i, value in enumerate(gw_fields[1:]):
        type = gw_type_to_my_type(value)
        field = Field.from_int(value)
        if field != None:
            desc.push_field(field)

        if type == TYPE_NESTED_STRUCT:
            desc.has_nested_struct = True
            decode_nested_struct(desc, gw_fields[i+1:])
            desc.nested_struct_count = field.param
            field.size += desc.nested_struct_size * desc.nested_struct_count
            desc.size  += desc.nested_struct_size * desc.nested_struct_count
            break

    return desc

def main(argv):
    proc = Process.from_name('Gw.exe')
    scanner = ProcessScanner(proc)
    
    addr = scanner.find(b'\x75\x04\x33\xC0\x5D\xC3\x8B\x41\x08\xA8\x01\x75', -0x6)
    addr = proc.read(addr)[0]      # 0xA2B294
    addr = proc.read(addr)[0]      # gs = *(GameServer **)0xA2B294, 0xa2b294
    addr = proc.read(addr + 8)[0]  # gs->consts

    gs_codecs = addr
    ls_codecs = proc.read(addr + 12)[0] # gs->consts->ls

    gs_clt_codecs, _, gs_clt_codecs_count = proc.read(gs_codecs + 28, 'III')
    gs_srv_codecs, _, gs_srv_codecs_count = proc.read(gs_codecs + 44, 'III')

    ls_clt_codecs, _, ls_clt_codecs_count = proc.read(ls_codecs + 28, 'III')
    ls_srv_codecs, _, ls_srv_codecs_count = proc.read(ls_codecs + 44, 'III')

    # addr_ls_clt = range(ls_clt_codecs, ls_clt_codecs + (8  * ls_clt_codecs_count), 8)  # AUTH_CLIENT
    # addr_ls_srv = range(ls_srv_codecs, ls_srv_codecs + (12 * ls_srv_codecs_count), 12) # AUTH_SERVER
    addr_gs_clt = range(gs_clt_codecs, gs_clt_codecs + (8  * gs_clt_codecs_count), 8)  # GAME_CLIENT
    addr_gs_srv = range(gs_srv_codecs, gs_srv_codecs + (12 * gs_srv_codecs_count), 12) # GAME_SERVER

    desc_ls_clt = []
    desc_ls_srv = []
    desc_gs_clt = []
    desc_gs_srv = []

    """
    for id, addr in enumerate(addr_ls_clt):
        fields_addr, count, handler = proc.read(addr, 'III')

        if count > 1:
            fields = proc.read(fields_addr, 'I' * count)
            desc = decode_msg_description(id, fields)
        else:
            assert count >= 0
            desc = Descriptor(id)
            desc.compute_size()

        desc_ls_clt.append(desc)

    for id, addr in enumerate(addr_ls_srv):
        fields_addr, count, handler = proc.read(addr, 'III')

        if count > 1:
            fields = proc.read(fields_addr, 'I' * count)
            desc = decode_msg_description(id, fields, handler)
        else:
            assert count >= 0
            desc = Descriptor(id, handler)
            desc.compute_size()

        desc_ls_srv.append(desc)

    """
    for id, addr in enumerate(addr_gs_clt):
        fields_addr, count, handler = proc.read(addr, 'III')

        if count > 1:
            fields = proc.read(fields_addr, 'I' * count)
            desc = decode_msg_description(id, fields)
        else:
            assert count >= 0
            desc = Descriptor(id)
            desc.compute_size()

        desc_gs_clt.append(desc)

    for id, addr in enumerate(addr_gs_srv):
        fields_addr, count, handler = proc.read(addr, 'III')

        if count > 1:
            fields = proc.read(fields_addr, 'I' * count)
            desc = decode_msg_description(id, fields, handler)
        else:
            assert count >= 0
            desc = Descriptor(id, handler)
            desc.compute_size()

        desc_gs_srv.append(desc)
    # """

    def create_desc_array(descriptions, prefix):
        print('MsgFormat %s_FORMATS[%d] = {' % (prefix, len(descriptions)))
        print('// header | field_count | fields | max_size | name')
        for desc in descriptions:
            header = desc.header
            d = header // 10
            count = desc.count_field()
            print('\t{%-3d, %-3d, %s_%04d, %-3d, 0},' % (header, count, prefix, header, desc.size))
        print('};\n')
    
    # for desc in desc_ls_clt:
    #     desc.print('AUTH_CLIENT')

    # for desc in desc_ls_srv:
    #     desc.print('AUTH_SERVER')

    for desc in desc_gs_clt:
        desc.print('GAME_CLIENT')

    for desc in desc_gs_srv:
        desc.print('GAME_SERVER')

    # create_desc_array(desc_ls_clt, 'AUTH_CLIENT')
    # create_desc_array(desc_ls_srv, 'AUTH_SERVER')
    create_desc_array(desc_gs_clt, 'GAME_CLIENT')
    create_desc_array(desc_gs_srv, 'GAME_SERVER')

if __name__ == '__main__':
    main(sys.argv[1::])
