from process import *

import argparse

region_type_to_str_table = {
    0:  'RegionType_AllianceBattle',
    1:  'RegionType_Arena',
    2:  'RegionType_ExplorableZone',
    3:  'RegionType_GuildBattleArea',
    4:  'RegionType_GuildHall',
    5:  'RegionType_MissionOutpost',
    6:  'RegionType_CooperativeMission',
    7:  'RegionType_CompetitiveMission',
    8:  'RegionType_EliteMission',
    9:  'RegionType_Challenge',
    10: 'RegionType_Outpost',
    11: 'RegionType_ZaishenBattle',
    12: 'RegionType_HeroesAscent',
    13: 'RegionType_City',
    14: 'RegionType_MissionArea',
    15: 'RegionType_HeroBattleOutpost',
    16: 'RegionType_HeroBattleArea',
    17: 'RegionType_EotnMission',
    18: 'RegionType_Dungeon',
    19: 'RegionType_Marketplace',
    20: 'RegionType_Unknown',
    21: 'RegionType_DevRegion',
}

def region_type_to_str(region_type):
    return region_type_to_str_table[region_type]

def main(args):
    if args.pid:
        proc = Process(args.pid)
    if args.proc:
        proc = Process.from_name(args.proc)
    scanner = ProcessScanner(proc)

    address = scanner.find(b'\x83\xc0\x0c\x41\x3d\x68\x01\x00\x00')
    translation_table_addr = proc.read(address + 0x19, 'I')[0]
    translation_table_size = proc.read(address + 5, 'I')[0] // 4
    values = proc.read(translation_table_addr, f'{translation_table_size}I')

    for i in range(0, translation_table_size, 3):
        map_type = values[i + 0]
        region_type = region_type_to_str(values[i + 2])
        print(f'{{.map_type = {map_type}, .region_type = {region_type}}},')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--pid", type=int, required=False,
        help="Process id of the target Guild Wars instance.")
    parser.add_argument("--proc", type=str, default='Gw.exe',
        help="Process name of the target Guild Wars instance.")
    args = parser.parse_args()

    main(args)
