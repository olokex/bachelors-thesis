#!/usr/bin/env python3

import json, os, sys


if not os.path.exists("outs_cgp"):
    os.mkdir("outs_cgp")

with open('cgp_run_settings.json', 'r') as outfile:
    jd = json.load(outfile)

def call(cname, file, out, gen, lambd, col, mut, fun, runs):
    print(f"running {cname}")
    template = f"./build/cgp path ./data/{file} generations {gen} second-criterion true column {col} lambda {lambd} mutate {mut} functions {fun}"
    out = f"./outs_cgp/{out}"
    for i in range(runs):
        os.system(f"{template} >> {out}")

for each in jd:
    call(*each.values())