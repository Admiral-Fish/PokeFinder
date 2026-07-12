import compression.zstd as zstd
import json
from pathlib import Path

from .embed_util import write_data

LANGUAGES = ("de", "en", "es", "fr", "it", "ja", "ko", "zh")


def embed_strings(parent_dir: str, output_dir: str):
    strings = bytes()
    indexes = []
    index = 0
    for path in LANGUAGES:
        dir = Path(parent_dir) / "i18n" / path
        files = [str(file) for file in dir.rglob("*") if file.suffix.lower() in (".txt", ".json")]
        for file in sorted(files):
            with open(file, "r", encoding="utf-8") as f:
                data = f.read()

            string_data = bytes()
            if ".json" in file:
                j = json.loads(data)
                string_data = bytes(json.dumps(j, separators=(',', ':')), encoding="utf-8")
            else:
                for line in data.split("\n"):
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

    write_data((string, index_string), f"{output_dir}/i18n.hpp", ("Core/Global.hpp", "array"), "utf-8")
