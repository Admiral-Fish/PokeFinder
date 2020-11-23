import os
import glob

def embed(paths):    
    arrays = []
    for path in paths:
        files = glob.glob(path + "/**/*.bin", recursive=True)
        for file in files:
            with open(file, "rb") as f:
                data = f.read()

            name = os.path.basename(f.name).replace(".bin", "")
            
            string = "constexpr u8 " + name + "[" + str(len(data)) + "] = {"
            for i in range(len(data)):
                string += str(data[i])
                if i != len(data) - 1:
                    string += ", "

            string += " };"
            arrays.append(string)
    return arrays


def write(arrays):
    f = open("Core\Resources\Resources.hpp", "w")
    f.write("#include <Core/Util/Global.hpp>\n\n")

    for array in arrays:
        f.write(array + "\n\n")

def main():
    arrays = embed(["Core\Resources\Encounters", "Core\Resources\Personal"])
    write(arrays)


if __name__ == "__main__":
    main()
