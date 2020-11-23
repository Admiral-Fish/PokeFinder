import os
import glob
import bz2

def embed(paths):    
    arrays = []
    for path in paths:
        files = glob.glob(f"{path}/**/*.bin", recursive=True)
        for file in files:
            with open(file, "rb") as f:
                data = f.read()

            name = os.path.basename(f.name).replace(".bin", "")
            compressed_data = bz2.compress(data, 9)
            
            string = f"constexpr u8 {name}[{len(compressed_data)}] = "
            string += " { "

            for i in range(len(compressed_data)):
                string += str(compressed_data[i])
                if i != len(compressed_data) - 1:
                    string += ", "

            string += " };"
            arrays.append(string)
    return arrays


def write(arrays):
    f = open("Core\Resources\Resources.hpp", "w")

    f.write("#include <Core/Util/Global.hpp>\n\n")

    for array in arrays:
        f.write(f"{array}\n\n")

def main():
    arrays = embed(["Core\Resources\Encounters", "Core\Resources\Personal"])
    write(arrays)


if __name__ == "__main__":
    main()
