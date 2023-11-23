#!/usr/bin/env python3

import re
import os


def enum_map(parent, files):
    re1 = re.compile(r"enum class (\w+) : u\d+")
    re2 = re.compile(r"(\w+)(?:,| =|$)")

    strings = []
    for file in files:
        with open(f"{parent}/{file}.hpp", "r") as f:
            data = [x for x in f.read().split("\n") if x != ""]

        for start, string in enumerate(data):
            if string.startswith("enum class"):
                enum_name = re1.search(string).group(1)
                start += 2
                break

        for end in range(start + 1, len(data), 1):
            if data[end] == "};":
                break

        data = data[start:end]

        string = f"NLOHMANN_JSON_SERIALIZE_ENUM( {enum_name}, {{\n"
        for i, line in enumerate(data):
            enum_value = re2.search(line).group(1)

            string += f"\t{{ {enum_name}::{enum_value}, \"{enum_value}\" }}"
            if i != len(data) - 1:
                string += ","
            string += "\n"
        string += "})"

        strings.append(string)

    with open("Enum.hpp", "w+") as f:
        f.write("#ifndef ENUM_HPP\n")
        f.write("#define ENUM_HPP\n\n")

        for file in files:
            f.write(f"#include <Core/Enum/{file}.hpp>\n")
        f.write("#include <nlohmann/json.hpp>\n\n")

        for string in strings:
            f.write(f"{string}\n\n")
        f.write("\n")

        f.write("#endif")


def main():
    import pathlib
    os.chdir(pathlib.Path(__file__).parent.absolute())

    parent = pathlib.Path(__file__).parent.parent.absolute() / "Core" / "Enum"
    enum_map(parent, ("Buttons", "DSType", "Encounter", "Game", "Language", "Lead", "Method"))


if __name__ == "__main__":
    main()
