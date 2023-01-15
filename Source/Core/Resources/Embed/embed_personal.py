import glob
import os
import struct

from .embed_util import write_data


def embed_personal():
    arrays = []
    read = struct.Struct("<H")
    for index in (3, 4, 5, 8):
        for file in glob.glob(f"Personal/Gen{index}/*.bin"):
            with open(file, "rb") as f:
                data = f.read()
                size = len(data)
                if index == 3:
                    offset = 0x1c
                elif index == 4:
                    offset = 0x2c
                elif index == 5:
                    if "bw" in file:
                        offset = 0x3c
                    else:
                        offset = 0x4c
                elif index == 8:
                    if "swsh" in file:
                        offset = 0xb0
                    else:
                        offset = 0x44

            name = os.path.basename(f.name).replace(".bin", "")
            string = f"constexpr std::array<PersonalInfo, {int(size/offset)}> {name} = {{ "

            for i in range(0, size, offset):
                hp = data[i]
                atk = data[i+0x1]
                defense = data[i+0x2]
                spe = data[i+0x3]
                spa = data[i+0x4]
                spd = data[i+0x5]
                type1 = data[i+0x6]
                type2 = data[i+0x7]

                if index == 3:
                    item1 = 0
                    item2 = 0
                    item3 = 0
                    gender = data[i+0x10]
                    ability1 = data[i+0x16]
                    ability2 = data[i+0x17]
                    abilityH = 0
                    if int(i/offset) in (386, 387, 388, 389):  # Deoxys
                        form_count = 4
                        form_stat_index = 387
                    else:
                        form_count = 1
                        form_stat_index = 0
                    hatch_species = 0
                    present = 1
                elif index == 4:
                    item1, = read.unpack(data[i+0xc:i+0xe])
                    item2, = read.unpack(data[i+0xe:i+0x10])
                    item3 = 0
                    gender = data[i+0x10]
                    ability1 = data[i+0x16]
                    ability2 = data[i+0x17] or ability1
                    abilityH = 0
                    if int(i/offset) in (386, 496, 497, 498):  # Deoxys
                        form_count = 4
                        form_stat_index = 496
                    elif int(i/offset) in (413, 499, 500):  # Wormadam
                        form_count = 3
                        form_stat_index = 499
                    elif int(i/offset) in (487, 501):  # Giratina
                        form_count = 2
                        form_stat_index = 501
                    elif int(i/offset) in (492, 502):  # Shaymin
                        form_count = 2
                        form_stat_index = 502
                    elif int(i/offset) in (479, 503, 504, 505, 506, 507):  # Rotom
                        form_count = 6
                        form_stat_index = 503
                    else:
                        form_count = 1
                        form_stat_index = 0
                    hatch_species = 0
                    present = 1
                elif index == 5:
                    item1, = read.unpack(data[i+0xc:i+0xe])
                    item2, = read.unpack(data[i+0xe:i+0x10])
                    item3, = read.unpack(data[i+0x10:i+0x12])
                    gender = data[i+0x12]
                    ability1 = data[i+0x18]
                    ability2 = data[i+0x19] or ability1
                    abilityH = data[i+0x1a] or ability1
                    form_count = data[i+0x20]
                    form_stat_index, = read.unpack(data[i+0x1c:i+0x1e])
                    hatch_species = 0
                    present = 1
                elif index == 8:
                    item1, = read.unpack(data[i+0xc:i+0xe])
                    item2, = read.unpack(data[i+0xe:i+0x10])
                    item3, = read.unpack(data[i+0x10:i+0x12])
                    gender = data[i+0x12]
                    ability1, = read.unpack(data[i+0x18:i+0x1a])
                    ability2, = read.unpack(data[i+0x1a:i+0x1c])
                    abilityH, = read.unpack(data[i+0x1c:i+0x1e])
                    form_count = data[i+0x20]
                    form_stat_index, = read.unpack(data[i+0x1e:i+0x20])
                    hatch_species, = read.unpack(data[i+0x3e:i+0x40])
                    present = (data[i+0x21] >> 6) & 1

                personal = f"PersonalInfo({hp}, {atk}, {defense}, {spa}, {spd}, {spe}, {type1}, {type2}, {item1}, {item2}, {item3}, {gender}, {ability1}, {ability2}, {abilityH}, {form_count}, {form_stat_index}, {hatch_species}, {present})"
                string += personal
                if i != size - offset:
                    string += ", "

            string += " };"
            arrays.append(string)

    write_data(arrays, "Personal.hpp", ("Core/Parents/PersonalInfo.hpp", "array"))
