#!/usr/bin/env python3

import os
import pathlib

from Embed.embed_gen3 import embed_encounters3
from Embed.embed_gen4 import embed_encounters4
from Embed.embed_gen5 import embed_encounters5
from Embed.embed_gen8 import embed_encounters8
from Embed.embed_ivs import embed_ivs
from Embed.embed_personal import embed_personal
from Embed.embed_strings import embed_strings


def main():
    os.chdir(pathlib.Path(__file__).parent.absolute())

    embed_encounters3()
    embed_encounters4()
    embed_encounters5()
    embed_encounters8()
    embed_ivs()
    embed_personal()
    embed_strings(("de", "en", "es", "fr", "it", "ja", "ko", "zh"))


if __name__ == "__main__":
    main()
