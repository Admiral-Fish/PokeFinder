#!/usr/bin/env python3

import re
import os


def enum_map(parent, files):
    re1 = re.compile(r"enum class (\w+) .+ {(.+)};")
    re2 = re.compile(r"([a-zA-Z]\w+)[, ]")

    strings = []
    for file in files:
        with open(f"{parent}/{file}.hpp", "r") as f:
            data = f.read().replace("\n", " ")

        enum_data = re1.search(data)

        enum_name = enum_data.group(1)
        enum_class = enum_data.group(2)
        enum_values = list(set(re2.findall(enum_class)))

        string = f"static std::map<std::string, {enum_name}> {enum_name.lower()} = {{ "
        for i, enum_value in enumerate(enum_values):
            string += f"{{ \"{enum_value}\", {enum_name}::{enum_value} }}"
            if i != len(enum_values) - 1:
                string += ","
        string += "};"

        strings.append(string)

    with open("Enum.hpp", "w+") as f:
        f.write("#ifndef ENUM_HPP\n")
        f.write("#define ENUM_HPP\n\n")

        for file in files:
            f.write(f"#include <Core/Enum/{file}.hpp>\n")
        f.write("#include <string>\n\n")

        for file in files:
            f.write(f"{file} get{file}(const std::string &s);\n")
        f.write("\n")

        f.write("#endif")

    with open("Enum.cpp", "w+") as f:
        f.write("#include \"Enum.hpp\"\n")
        f.write("#include <map>\n\n")

        for string in strings:
            f.write(f"{string}\n\n")

        for file in files:
            f.write(f"{file} get{file}(const std::string &s) {{ return {file.lower()}[s]; }};\n")


def main():
    import pathlib
    os.chdir(pathlib.Path(__file__).parent.absolute())

    parent = pathlib.Path(__file__).parent.parent.absolute() / "Core" / "Enum"
    enum_map(parent, ("DSType", "Encounter", "Game", "Language", "Lead", "Method"))


if __name__ == "__main__":
    main()
