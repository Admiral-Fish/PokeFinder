#!/usr/bin/env python3

import glob
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

    return arrays


def embed_personal():
    arrays = []

    for index in ("3", "4", "5"):
        file = f"Personal/Gen{index}/personal{index}.bin"
        with open(file, "rb") as f:
            data = f.read()
            size = len(data)
            offset = 0x1c if index == "3" else 0x2c if index == "4" else 0x4c

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
                form_count = 1
                abilityH = 0
                form_stat_index = 0
            elif index == "4":
                gender = data[i+0x10]
                ability1 = data[i+0x16]
                ability2 = data[i+0x17]
                form_count = data[i+0x29]
                abilityH = 0
                form_stat_index = (data[i+0x2b] << 8) | data[i+0x2a]
            elif index == "5":
                gender = data[i+0x12]
                ability1 = data[i+0x18]
                ability2 = data[i+0x19]
                form_count = data[i+0x20]
                abilityH = data[i+0x1a]
                form_stat_index = (data[i+0x1d] << 8) | data[i+0x1c]

            personal = f"PersonalInfo({hp}, {atk}, {defense}, {spa}, {spd}, {spe}, {gender}, {ability1}, {ability2}, {abilityH}, {form_count}, {form_stat_index})"
            string += personal
            if i != size - offset:
                string += ", "

        string += " };"
        arrays.append(string)

    return arrays


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


def write_data(arrays):
    f = open("Resources.hpp", "w")

    f.write("#include <Core/Parents/PersonalInfo.hpp>\n")
    f.write("#include <Core/Util/Global.hpp>\n")
    f.write("#include <array>\n\n")

    for array in arrays:
        f.write(f"{array}\n\n")


def write_strings(maps):
    f = open("I18n.hpp", "w", encoding="utf-8")

    f.write("#include <Core/Util/Global.hpp>\n")
    f.write("#include <array>\n\n")

    for map in maps:
        for string in map.values():
            f.write(f"{string}\n\n")


def main():
    import pathlib
    os.chdir(pathlib.Path(__file__).parent.absolute())

    encounters = embed_encounters()
    personal = embed_personal()
    write_data(encounters + personal)

    embed_strings(["de", "en", "es", "fr", "it", "ja", "ko", "zh"])


if __name__ == "__main__":
    main()
