import bz2
import json
import os

from .embed_util import write_data


def embed_encounters3():
    arrays = []

    with open("EncounterTables/Gen3/encounters.json") as f:
        data = json.load(f)
        for type, encounters in data.items():
            if "Shadow" in type:
                string = f"constexpr std::array<ShadowTemplate, {len(encounters)}> {type} = {{ "
            else:
                string = f"constexpr std::array<StaticTemplate, {len(encounters)}> {type} = {{ "

            for i, encounter in enumerate(encounters):
                if "Shadow" in type:
                    locks = "std::array<LockInfo, 5> { "
                    for j in range(int(encounter["count"])):
                        lock = encounter["locks"][j]
                        locks += f"LockInfo({lock['nature']}, {lock['gender']}, {lock['genderRatio']})"
                        if j != int(encounter["count"]) - 1:
                            locks += ", "
                    locks += " }"

                    string += f"ShadowTemplate({encounter['version']}, {encounter['specie']}, {encounter.get('shiny', 'Shiny::Random')}, {encounter['level']}, {locks}, {encounter['count']}, {encounter['type']})"
                else:
                    string += f"StaticTemplate({encounter['version']}, {encounter['specie']}, {encounter.get('form', 0)}, {encounter.get('shiny', 'Shiny::Random')}, 255, 255, 0, {encounter['level']})"
                
                if i != len(encounters) - 1:
                    string += ", "

            string += " };"
            arrays.append(string)

    files = ("emerald", "firered", "leafgreen", "ruby", "sapphire", "xd")
    for file in files:
        with open(f"EncounterTables/{file}.bin", "rb") as f:
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

    write_data(arrays, "EncounterData3.hpp", ("Core/Gen3/ShadowTemplate.hpp", "Core/Global.hpp", "array"))
