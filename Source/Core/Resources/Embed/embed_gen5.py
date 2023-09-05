import bz2
import json
import os

from .embed_util import write_data


def embed_encounters5():
    arrays = []

    with open("EncounterTables/Gen5/encounters.json") as f:
        data = json.load(f)
        for type, encounters in data.items():
            if "dreamRadar" in type:
                string = f"constexpr std::array<DreamRadarTemplate, {len(encounters)}> {type.upper()} = {{ "
            else:
                string = f"constexpr std::array<StaticTemplate5, {len(encounters)}> {type.upper()} = {{ "

            for i, encounter in enumerate(encounters):
                if "dreamRadar" in type:
                    string += f"DreamRadarTemplate({encounter['specie']}, {encounter.get('form', 0)}, {encounter['ability']})"
                else:
                    string += f"StaticTemplate5({encounter['version']}, {encounter['specie']}, {encounter.get('form', 0)}, {encounter.get('shiny', 'Shiny::Random')}, {encounter.get('ability', 255)}, {encounter.get('gender', 255)}, {encounter['level']})"

                if i != len(encounters) - 1:
                    string += ", "

            string += " };"
            arrays.append(string)

    files = ("black", "black2", "bw2_grotto", "white", "white2")
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

    write_data(arrays, "EncounterData5.hpp", ("Core/Gen5/DreamRadarTemplate.hpp", "Core/Gen5/StaticTemplate5.hpp", "array"))
