import bz2
import glob
import json
import os

from .embed_util import write_data


def embed_encounters8():
    arrays = []

    with open("EncounterTables/Gen8/encounters.json") as f:
        data = json.load(f)
        for type, encounters in data.items():
            string = f"constexpr std::array<StaticTemplate, {len(encounters)}> {type} = {{ "

            for i, encounter in enumerate(encounters):
                string += f"StaticTemplate({encounter['version']}, {encounter['specie']}, {encounter.get('form', 0)}, {encounter.get('shiny', 'Shiny::Random')}, {encounter.get('ability', 255)}, {encounter.get('gender', 255)}, {encounter.get('ivCount', 0)}, {encounter['level']})"
                
                if i != len(encounters) - 1:
                    string += ", "

            string += " };"
            arrays.append(string)

    with open("EncounterTables/Gen8/nests.json", "r") as f:
        data = json.load(f)
        tables = data["Tables"]

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
                string += f"Raid({species}, {altform}, Shiny::Random, {ability}, {gender}, {iv_count}, {gigantamax}, {star_string})"
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
                string += f"Raid({species}, {altform}, Shiny::Random, {ability}, {gender}, {iv_count}, {gigantamax}, {star_string})"
                if j != len(shield) - 1:
                    string += ", "
            string += "})"
            if i != len(tables) - 1:
                string += ", "
        string += "};"

        arrays.append(string)

    files = ("bd", "bd_underground", "sp", "sp_underground")
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

    write_data(arrays, "EncounterData8.hpp", ("Core/Gen8/Den.hpp", "Core/Parents/StaticTemplate.hpp", "Core/Global.hpp", "array"))
