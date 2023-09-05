import bz2
import json
import os

from .embed_util import write_data


def _get_shiny(flag: int):
    if flag == 0:
        return "Shiny::Random"
    elif flag == 1:
        return "Shiny::Never"
    elif flag == 2:
        return "Shiny::Always"
    

def _get_stars(flags: list[str]):
    stars = [ "1" if int(x) != 0 else "0" for x in flags ]
    return f"std::array<bool, 5> {{ {', '.join(stars)} }}"


def embed_encounters8():
    arrays = []

    with open("EncounterTables/Gen8/encounters.json") as f:
        data = json.load(f)
        for type, encounters in data.items():
            string = f"constexpr std::array<StaticTemplate, {len(encounters)}> {type.upper()} = {{ "

            for i, encounter in enumerate(encounters):
                string += f"StaticTemplate({encounter['version']}, {encounter['specie']}, {encounter.get('form', 0)}, {encounter.get('shiny', 'Shiny::Random')}, {encounter.get('ability', 255)}, {encounter.get('gender', 255)}, {encounter.get('ivCount', 0)}, {encounter['level']})"
                
                if i != len(encounters) - 1:
                    string += ", "

            string += " };"
            arrays.append(string)

    with open("EncounterTables/Gen8/swsh/nests.json", "r") as f:
        data = json.load(f)
        tables = data["Tables"]

        name = os.path.basename(f.name).replace(".json", "")
        string = f"constexpr std::array<Den, {len(tables)}> {name.upper()} = {{ "

        tables.sort(key=lambda den: int(f"0x{den['TableID']}", 16))
        for i, table in enumerate(tables):
            string += f"Den(0x{table['TableID']}, "

            string += "std::array<Raid, 12> {"
            sword = table["SwordEntries"]
            raids = []
            for raid in sword:
                ability = raid["Ability"]
                altform = raid["AltForm"]
                iv_count = raid["FlawlessIVs"]
                gender = raid["Gender"]
                gigantamax = int(raid["IsGigantamax"])
                species = raid["Species"]
                stars = _get_stars(raid["Stars"])
                raids.append(f"Raid({species}, {altform}, Shiny::Random, {ability}, {gender}, {iv_count}, {gigantamax}, {stars})")
            string += f"{', '.join(raids)} }}, "

            string += "std::array<Raid, 12> {"
            shield = table["ShieldEntries"]
            raids = []
            for raid in shield:
                ability = raid["Ability"]
                altform = raid["AltForm"]
                iv_count = raid["FlawlessIVs"]
                gender = raid["Gender"]
                gigantamax = int(raid["IsGigantamax"])
                species = raid["Species"]
                stars = _get_stars(raid["Stars"])
                raids.append(f"Raid({species}, {altform}, Shiny::Random, {ability}, {gender}, {iv_count}, {gigantamax}, {stars})")
            string += f"{', '.join(raids)} }})"
            if i != len(tables) - 1:
                string += ", "
        string += "};"

        arrays.append(string)

    string = "constexpr std::array<DenEvent, 69> NESTS_EVENT = { "
    for i in range(1, 70):
        with open(f"EncounterTables/Gen8/swsh/event{i}.json", "r") as f:
            data = json.load(f)
            tables = data["Tables"]

            string += "DenEvent("

            string += "std::array<Raid, 30> {"
            sword = tables[0]["Entries"]
            raids = []
            for raid in sword:
                if all(int(x) == 0 for x in raid["Probabilities"]):
                    continue

                ability = raid["Ability"]
                altform = raid["AltForm"]
                iv_count = raid["FlawlessIVs"]
                gender = raid["Gender"]
                gigantamax = int(raid["IsGigantamax"])
                level = raid["Level"]
                shiny = _get_shiny(raid["ShinyFlag"])
                species = raid["Species"]
                stars = _get_stars(raid["Probabilities"])
                raids.append(f"Raid({species}, {altform}, {shiny}, {ability}, {gender}, {iv_count}, {gigantamax}, {stars}, {level})")
            string += f"{', '.join(raids)} }}, "

            string += "std::array<Raid, 30> {"
            shield = tables[1]["Entries"]
            raids = []
            for raid in shield:
                if all(int(x) == 0 for x in raid["Probabilities"]):
                    continue

                ability = raid["Ability"]
                altform = raid["AltForm"]
                iv_count = raid["FlawlessIVs"]
                gender = raid["Gender"]
                gigantamax = int(raid["IsGigantamax"])
                level = raid["Level"]
                shiny = _get_shiny(raid["ShinyFlag"])
                species = raid["Species"]
                stars = _get_stars(raid["Probabilities"])
                raids.append(f"Raid({species}, {altform}, {shiny}, {ability}, {gender}, {iv_count}, {gigantamax}, {stars}, {level})")
            string += f"{', '.join(raids)} }})"
        
        if i != 69:
            string += ", "
    string += "};"
    arrays.append(string)

    files = ("bd", "bd_underground", "sp", "sp_underground")
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

    write_data(arrays, "EncounterData8.hpp", ("Core/Gen8/Den.hpp", "Core/Parents/StaticTemplate.hpp", "array"))
