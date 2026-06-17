#!/usr/bin/env python3

import argparse
import os

from Embed.embed_gen3 import embed_encounters3
from Embed.embed_gen4 import embed_encounters4
from Embed.embed_gen5 import embed_encounters5
from Embed.embed_gen8 import embed_encounters8
from Embed.embed_personal import embed_personal
from Embed.embed_strings import embed_strings

SCRIPT_FOLDER = os.path.dirname(os.path.abspath(__file__))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input_dir", type=str)
    parser.add_argument("output_dir", type=str)
    args = parser.parse_args()

    os.makedirs(args.output_dir, exist_ok=True)

    embed_encounters3(SCRIPT_FOLDER, args.input_dir, args.output_dir)
    embed_encounters4(SCRIPT_FOLDER, args.input_dir, args.output_dir)
    embed_encounters5(SCRIPT_FOLDER, args.input_dir, args.output_dir)
    embed_encounters8(SCRIPT_FOLDER, args.input_dir, args.output_dir)
    embed_personal(SCRIPT_FOLDER, args.output_dir)
    embed_strings(SCRIPT_FOLDER, args.output_dir)


if __name__ == "__main__":
    main()
