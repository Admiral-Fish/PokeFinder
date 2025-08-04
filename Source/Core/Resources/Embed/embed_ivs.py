import os
import zstandard as zstd

from .embed_util import write_data


def embed_ivs():
    with open("IVs/ivs.bin", "rb") as f:
        data = f.read()

    data = zstd.compress(data, 22)

    name = os.path.basename(f.name).replace(".bin", "")
    string = f"constexpr std::array<u8, {int(len(data))}> {name.upper()} = {{ "

    for i in range(len(data)):
        string += str(data[i])
        if i != len(data) - 1:
            string += ", "

    string += " };"

    write_data([string], "IVs.hpp", ("Core/Global.hpp", "array"))
