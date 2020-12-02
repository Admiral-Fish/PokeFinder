import glob
import os


def embed_data(paths):
    arrays = []

    for path in paths:
        files = glob.glob(f"{path}/**/*.bin", recursive=True)
        for file in files:
            with open(file, "rb") as f:
                data = f.read()

            name = os.path.basename(f.name).replace(".bin", "")

            string = f"constexpr std::array<u8, {len(data)}> {name} = "
            string += " { "

            for i in range(len(data)):
                string += str(data[i])
                if i != len(data) - 1:
                    string += ", "

            string += " };"
            arrays.append(string)

    return arrays


def write_data(arrays):
    f = open("Core\Resources\Resources.hpp", "w")

    f.write("#include <Core/Util/Global.hpp>\n")
    f.write("#include <array>\n\n")

    for array in arrays:
        f.write(f"{array}\n\n")


def embed_strings(paths):
    arrays = []
    for path in paths:
        mapping = {}

        files = glob.glob(f"{path}/**/*.txt", recursive=True)
        for file in files:
            with open(file, "r", encoding="utf-8") as f:
                data = f.read().split("\n")

            name = os.path.basename(f.name).replace(".txt", "")

            string_data = []
            for line in data:
                line = line.replace('\r', '').replace('\n', '')

                for x in [int(y) for y in bytearray(line, encoding="utf-8")]:
                    string_data.append(x)
                string_data.append(0)

            string = f"constexpr std::array<u8, {len(string_data)}> {name} = "
            string += " { "

            for i in range(len(string_data)):
                string += str(string_data[i])
                if i != len(string_data) - 1:
                    string += ", "

            string += " };"
            mapping[name] = string

        arrays.append(mapping)

    return arrays


def write_strings(maps):
    f = open("Core\Resources\I18n.hpp", "w", encoding="utf-8")

    f.write("#include <Core/Util/Global.hpp>\n")
    f.write("#include <array>\n\n")

    for map in maps:
        for string in map.values():
            f.write(f"{string}\n\n")


def main():
    arrays = embed_data(
        ["Core\Resources\Encounters", "Core\Resources\Personal"])
    write_data(arrays)

    maps = embed_strings(["Core\Resources\i18n\de", "Core\Resources\i18n\en", "Core\Resources\i18n\es", "Core\Resources\i18n\\fr",
                          "Core\Resources\i18n\it", "Core\Resources\i18n\ja", "Core\Resources\i18n\ko", "Core\Resources\i18n\zh", ])
    write_strings(maps)


if __name__ == "__main__":
    main()
