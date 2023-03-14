def write_data(arrays, file, includes):
    f = open(file, "w")

    for include in includes:
        f.write(f"#include <{include}>\n")
    f.write("\n")

    for array in arrays:
        f.write(f"{array}\n\n")
