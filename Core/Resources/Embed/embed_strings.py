import glob
import zstandard as zstd

from .embed_util import write_data


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

            indexes.append(index)
            index += len(string_data)

            strings += string_data
    indexes.append(index)

    strings = zstd.compress(strings, 22)

    string = f"constexpr std::array<u8, {len(strings)}> I18N = {{ "
    for i, char in enumerate(strings):
        string += str(char)
        if i != len(strings) - 1:
            string += ", "
    string += " };"

    index_string = f"constexpr u32 INDICES[{len(indexes)}] = {{ "
    for i, index in enumerate(indexes):
        index_string += str(index)
        if i != len(indexes) - 1:
            index_string += ", "
    index_string += " };"

    write_data((string, index_string), "i18n.hpp", ("Core/Global.hpp", "array"), "utf-8")
