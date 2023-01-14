import bz2
import glob


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
                if "forms" in file:
                    entries = line.split(",")
                    line = f"{(int(entries[1]) << 11) | int(entries[0])},{entries[2]}"
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


def write_strings(maps):
    f = open("i18n.hpp", "w", encoding="utf-8")

    f.write("#include <Core/Global.hpp>\n\n")
    for map in maps:
        f.write(f"{map}\n\n")
