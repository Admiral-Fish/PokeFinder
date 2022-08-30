#!/usr/bin/env python3

import bz2
import glob
import json
import os
import re


def embed_encounters():
    arrays = []
    files = glob.glob("Encounters/**/*.bin", recursive=True)
    for file in files:
        with open(file, "rb") as f:
            data = f.read()

        size = len(data)
        data = bz2.compress(data, 9)
        data = size.to_bytes(2, byteorder="little") + data

        name = os.path.basename(f.name).replace(".bin", "")
        string = f"constexpr std::array<u8, {len(data)}> {name} = {{ "

        for i in range(len(data)):
            string += str(data[i])
            if i != len(data) - 1:
                string += ", "

        string += " };"
        arrays.append(string)

    # Handle raids seperately
    with open("Encounters/Gen8/nests.json") as f:
        data = json.load(f)
        tables = data["Tables"]

        # Since some raids have altform of non 0
        # We need to dynamically adjust the index we lookup in the personal table
        with open("Personal.hpp", "r") as f_personal:
            data = f_personal.readlines()
            for line in data:
                if "personal_swsh" in line:
                    personal = re.findall(r"PersonalInfo\(.+?\)", line)
                    for i in range(len(personal)):
                        personal[i] = re.search(r"\((.+?)\)", personal[i]).group(1).split(", ")

        name = os.path.basename(f.name).replace(".json", "")
        string = f"constexpr std::array<Den, {len(tables)}> {name} = {{ "

        tables.sort(key=lambda den: int(f"0x{den['TableID']}", 16))
        for i, table in enumerate(tables):
            string += f"Den(0x{table['TableID']}, "

            string += "std::array<Raid, 12> {"
            sword = table["SwordEntries"]
            for j, raid in enumerate(sword):
                ability = raid["Ability"]
                altform = raid["AltForm"]
                iv_count = raid["FlawlessIVs"]
                gender = raid["Gender"]
                gigantamax = int(raid["IsGigantamax"])
                species = raid["Species"]
                stars = raid["Stars"]
                star_string = f"std::array<bool, 5> {{{int(stars[0])}, {int(stars[1])}, {int(stars[2])}, {int(stars[3])}, {int(stars[4])}}}"

                personal_index = species
                if altform != 0:
                    altform_index = int(personal[species][11])
                    personal_index = altform_index + altform - 1

                string += f"Raid({ability}, {altform}, {iv_count}, {gender}, {gigantamax}, {species}, personal_swsh[{personal_index}], {star_string})"
                if j != len(sword) - 1:
                    string += ", "
            string += "}, "

            string += "std::array<Raid, 12> {"
            shield = table["ShieldEntries"]
            for j, raid in enumerate(shield):
                ability = raid["Ability"]
                altform = raid["AltForm"]
                iv_count = raid["FlawlessIVs"]
                gender = raid["Gender"]
                gigantamax = int(raid["IsGigantamax"])
                species = raid["Species"]
                stars = raid["Stars"]
                star_string = f"std::array<bool, 5> {{{int(stars[0])}, {int(stars[1])}, {int(stars[2])}, {int(stars[3])}, {int(stars[4])}}}"

                personal_index = species
                if altform != 0:
                    altform_index = int(personal[species][11])
                    personal_index = altform_index + altform - 1

                string += f"Raid({ability}, {altform}, {iv_count}, {gender}, {gigantamax}, {species}, personal_swsh[{personal_index}], {star_string})"
                if j != len(sword) - 1:
                    string += ", "
            string += "})"
            if i != len(tables) - 1:
                string += ", "
        string += "};"

        arrays.append(string)

    write_data(arrays, "Encounters.hpp", ("Core/Gen8/Den.hpp", "Core/Global.hpp", "Core/Resources/Personal.hpp", "array"))


def embed_personal():
    arrays = []
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
                    gender = data[i+0x10]
                    ability1 = data[i+0x16]
                    ability2 = data[i+0x17]
                    abilityH = 0
                    form_count = 1
                    form_stat_index = 0
                    present = 1
                elif index == 4:
                    gender = data[i+0x10]
                    ability1 = data[i+0x16]
                    ability2 = data[i+0x17]
                    abilityH = 0
                    form_count = data[i+0x29]
                    form_stat_index = (data[i+0x2b] << 8) | data[i+0x2a]
                    present = 1
                elif index == 5:
                    gender = data[i+0x12]
                    ability1 = data[i+0x18]
                    ability2 = data[i+0x19]
                    abilityH = data[i+0x1a]
                    form_count = data[i+0x20]
                    form_stat_index = (data[i+0x1d] << 8) | data[i+0x1c]
                    present = 1
                elif index == 8:
                    gender = data[i+0x12]
                    ability1 = (data[i+0x19] << 8) | data[i+0x18]
                    ability2 = (data[i+0x1b] << 8) | data[i+0x1a]
                    abilityH = (data[i+0x1d] << 8) | data[i+0x1c]
                    form_count = data[i+0x20]
                    form_stat_index = (data[i+0x1f] << 8) | data[i+0x1e]
                    present = (data[i+0x21] >> 6) & 1

                personal = f"PersonalInfo({hp}, {atk}, {defense}, {spa}, {spd}, {spe}, {type1}, {type2}, {gender}, {ability1}, {ability2}, {abilityH}, {form_count}, {form_stat_index}, {present})"
                string += personal
                if i != size - offset:
                    string += ", "

            string += " };"
            arrays.append(string)

    write_data(arrays, "Personal.hpp", ("Core/Parents/PersonalInfo.hpp", "array"))


def embed_strings(paths):
    strings = bytes()
    indexes = []
    index = 0
    for path in paths:
        files = glob.glob(f"i18n/{path}/*.txt", recursive=True)
        for file in sorted(files):
            with open(file, "r", encoding="utf-8") as f:
                data = f.read().split("\n")

            string_data = bytes()
            for line in data:
                string_data += bytes(line, encoding="utf-8")
                string_data += b"\x00"

            size = len(string_data).to_bytes(2, byteorder="little")
            string_data = bz2.compress(string_data, 9)
            string_data = size + string_data

            indexes.append(index)
            index += len(string_data)

            strings += string_data
    indexes.append(index)

    string = f"constexpr u8 i18n[{len(strings)}] = {{ "
    for i, char in enumerate(strings):
        string += str(char)
        if i != len(strings) - 1:
            string += ", "
    string += " };"

    index_string = f"constexpr u32 indexes[{len(indexes)}] = {{ "
    for i, index in enumerate(indexes):
        index_string += str(index)
        if i != len(indexes) - 1:
            index_string += ", "
    index_string += " };"

    write_strings((string, index_string))


def write_data(arrays, file, includes):
    f = open(file, "w")

    for include in includes:
        f.write(f"#include <{include}>\n")
    f.write("\n")

    for array in arrays:
        f.write(f"{array}\n\n")


def write_strings(maps):
    f = open("i18n.hpp", "w", encoding="utf-8")

    f.write("#include <Core/Global.hpp>\n")
    f.write("#include <array>\n\n")

    for map in maps:
        if type(map) is str:
            f.write(f"{map}\n\n")
        else:
            for string in map.values():
                f.write(f"{string}\n\n")


def main():
    import pathlib
    os.chdir(pathlib.Path(__file__).parent.absolute())

    embed_personal()
    embed_encounters()
    embed_strings(("de", "en", "es", "fr", "it", "ja", "ko", "zh"))


if __name__ == "__main__":
    main()
