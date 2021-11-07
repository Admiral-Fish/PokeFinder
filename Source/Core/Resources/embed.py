#!/usr/bin/env python3

import glob
import json
import os


def embed_encounters():
    arrays = []

    files = glob.glob("Encounters/**/*.bin", recursive=True)
    for file in files:
        with open(file, "rb") as f:
            data = f.read()

        name = os.path.basename(f.name).replace(".bin", "")

        string = f"constexpr std::array<u8, {len(data)}> {name} ="
        string += " { "

        for i in range(len(data)):
            string += str(data[i])
            if i != len(data) - 1:
                string += ", "

        string += " };"
        arrays.append(string)

    # Handle raids seperately
    with open("Encounters/Gen8/nests.json") as f:
        data = json.load(f)
        tables : list = data["Tables"]

        name = os.path.basename(f.name).replace(".json", "")

        string = f"constexpr std::array<Den, {len(tables)}> {name} ="
        string += " { "

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
                star_string = "std::array<bool, 5> {" + f"{int(stars[0])}, {int(stars[1])}, {int(stars[2])}, {int(stars[3])}, {int(stars[4])}" + "}"

                string += f"Raid({ability}, {altform}, {iv_count}, {gender}, {gigantamax}, {species}, personal8[{species}], {star_string})"
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
                star_string = "std::array<bool, 5> {" + f"{int(stars[0])}, {int(stars[1])}, {int(stars[2])}, {int(stars[3])}, {int(stars[4])}" + "}"

                string += f"Raid({ability}, {altform}, {iv_count}, {gender}, {gigantamax}, {species}, personal8[{species}], {star_string})"
                if j != len(sword) - 1:
                    string += ", "
            string += "})"
            if i != len(tables) - 1:
                string += ", "
        string += "};"

        arrays.append(string)

    write_data(arrays, "Encounters.hpp", ("Core/Gen8/Den.hpp", "Core/Util/Global.hpp", "Core/Resources/Personal.hpp", "array"))


def embed_personal():
    arrays = []

    for index in ("3", "4", "5", "8"):
        file = f"Personal/Gen{index}/personal{index}.bin"
        with open(file, "rb") as f:
            data = f.read()
            size = len(data)
            offset = 0x1c if index == "3" else 0x2c if index == "4" else 0x4c if index == "5" else 0xb0

        name = os.path.basename(f.name).replace(".bin", "")

        string = f"constexpr std::array<PersonalInfo, {int(size/offset)}> {name} ="
        string += " { "

        for i in range(0, size, offset):
            hp = data[i]
            atk = data[i+0x1]
            defense = data[i+0x2]
            spe = data[i+0x3]
            spa = data[i+0x4]
            spd = data[i+0x5]

            if index == "3":
                gender = data[i+0x10]
                ability1 = data[i+0x16]
                ability2 = data[i+0x17]
                abilityH = 0
                form_count = 1
                form_stat_index = 0
                present = 1
            elif index == "4":
                gender = data[i+0x10]
                ability1 = data[i+0x16]
                ability2 = data[i+0x17]
                abilityH = 0
                form_count = data[i+0x29]
                form_stat_index = (data[i+0x2b] << 8) | data[i+0x2a]
                present = 1
            elif index == "5":
                gender = data[i+0x12]
                ability1 = data[i+0x18]
                ability2 = data[i+0x19]
                abilityH = data[i+0x1a]
                form_count = data[i+0x20]
                form_stat_index = (data[i+0x1d] << 8) | data[i+0x1c]
                present = 1
            elif index == "8":
                gender = data[i+0x12]
                ability1 = (data[i+0x19] << 8) | data[i+0x18]
                ability2 = (data[i+0x1b] << 8) | data[i+0x1a]
                abilityH = (data[i+0x1d] << 8) | data[i+0x1c]
                form_count = data[i+0x20]
                form_stat_index = (data[i+0x1f] << 8) | data[i+0x1e]
                present = (data[i+0x21] >> 6) & 1

            personal = f"PersonalInfo({hp}, {atk}, {defense}, {spa}, {spd}, {spe}, {gender}, {ability1}, {ability2}, {abilityH}, {form_count}, {form_stat_index}, {present})"
            string += personal
            if i != size - offset:
                string += ", "

        string += " };"
        arrays.append(string)

    write_data(arrays, "Personal.hpp", ("Core/Parents/PersonalInfo.hpp", "array"))


def embed_strings(paths):
    arrays = []
    for path in paths:
        mapping = {}

        files = glob.glob(f"i18n/{path}/*.txt", recursive=True)
        for file in files:
            with open(file, "r", encoding="utf-8") as f:
                data = f.read().split("\n")

            name = os.path.basename(f.name).replace(".txt", "")

            string_data = []
            for line in data:
                line = line.replace('\r', '').replace('\n', '')

                for x in [int(y) for y in bytearray(line, encoding="utf-8")]:
                    string_data.append(x)
                string_data.append(0)

            string = f"constexpr std::array<u8, {len(string_data)}> {name} ="
            string += " { "

            for i in range(len(string_data)):
                string += str(string_data[i])
                if i != len(string_data) - 1:
                    string += ", "

            string += " };"
            mapping[name] = string

        arrays.append(mapping)

    write_strings(arrays)


def write_data(arrays, file, includes):
    f = open(file, "w")

    for include in includes:
        f.write(f"#include <{include}>\n")
    f.write("\n")

    for array in arrays:
        f.write(f"{array}\n\n")


def write_strings(maps):
    f = open("i18n.hpp", "w", encoding="utf-8")

    f.write("#include <Core/Util/Global.hpp>\n")
    f.write("#include <array>\n\n")

    for map in maps:
        for string in map.values():
            f.write(f"{string}\n\n")


def main():
    import pathlib
    os.chdir(pathlib.Path(__file__).parent.absolute())

    embed_encounters()
    embed_personal()
    embed_strings(["de", "en", "es", "fr", "it", "ja", "ko", "zh"])


if __name__ == "__main__":
    main()
