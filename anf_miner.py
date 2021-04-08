#!/usr/bin/env python3

import json, os, sys

if not os.path.exists("outs_anf"):
    os.mkdir("outs_anf")

with open('anf_run_settings.json', 'r') as outfile:
    jd = json.load(outfile)

def call(cname, file, out, arity, terms, gen, lambd, mut, runs):
    print(f"running {cname}")
    template = f"./build/anf path ./data/{file} generations {gen} second-criterion true arity {arity} terms {terms} lambda {lambd} mutate {mut}"
    out = f"./outs_anf/{out}"
    for i in range(runs):
        os.system(f"{template} >> {out}")

for each in jd:
    call(*each.values())