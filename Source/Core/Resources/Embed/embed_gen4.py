import bz2
import json
import os

from .embed_util import write_data


def embed_encounters4():
    arrays = []

    with open("EncounterTables/Gen4/encounters.json") as f:
        data = json.load(f)
        for type, encounters in data.items():
            string = f"constexpr std::array<StaticTemplate4, {len(encounters)}> {type.upper()} = {{ "

            for i, encounter in enumerate(encounters):
                string += f"StaticTemplate4({encounter['version']}, {encounter['specie']}, {encounter.get('form', 0)}, {encounter.get('shiny', 'Shiny::Random')}, {encounter['level']}, {encounter['method']})"

                if i != len(encounters) - 1:
                    string += ", "

            string += " };"
            arrays.append(string)

    files = ("d_honey", "diamond", "heartgold", "hg_headbutt", "hgss_bug", "hgss_safari", "p_honey", "pearl", "platinum", "pt_honey", "soulsilver", "ss_headbutt")
    for file in files:
        with open(f"EncounterTables/{file}.bin", "rb") as f:
            data = f.read()

        size = len(data)
        data = bz2.compress(data, 9)
        data = size.to_bytes(4, "little") + data

        name = os.path.basename(f.name).replace(".bin", "")
        string = f"constexpr std::array<u8, {len(data)}> {name.upper()} = {{ "

        for i in range(len(data)):
            string += str(data[i])
            if i != len(data) - 1:
                string += ", "

        string += " };"
        arrays.append(string)

    write_data(arrays, "EncounterData4.hpp", ("Core/Gen4/StaticTemplate4.hpp", "array"))
