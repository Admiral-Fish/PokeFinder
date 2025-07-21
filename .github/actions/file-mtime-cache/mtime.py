#!/usr/bin/env python3
import sys
import os
import json
import glob
import hashlib
from pathlib import Path

mode = sys.argv[1]
raw_input = sys.argv[2]
patterns = raw_input.splitlines()
cache_file = Path(".file-mtime-cache/mtimes.json")
cache_file.parent.mkdir(exist_ok=True)

if mode == "save":
    result = {}
    for pattern in patterns:
        for file in glob.glob(pattern, recursive=True):
            if os.path.isfile(file):
                with open(file, "rb") as f:
                    file_hash = hashlib.sha256(f.read()).hexdigest()
                    result[file] = (file_hash, os.path.getmtime(file))
    with cache_file.open("w+", encoding="utf-8") as f:
        json.dump(result, f)
    print(f"Saved {len(result)} mtimes.")

elif mode == "restore":
    if not cache_file.exists():
        print("No mtime cache to restore.")
        sys.exit(0)
    with cache_file.open(encoding="utf-8") as f:
        data = json.load(f)
    for file, (previous_hash, mtime) in data.items():
        if os.path.exists(file):
            with open(file, "rb") as f:
                file_hash = hashlib.sha256(f.read()).hexdigest()
            if file_hash != previous_hash:
                print(f"{file} has changed, skipping restoration.")
                continue
            os.utime(file, (mtime, mtime))
            print(f"Restored: {file}")
else:
    print("Usage: save or restore")
    sys.exit(1)
