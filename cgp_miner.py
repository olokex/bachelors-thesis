#!/usr/bin/env python3

import json, os, sys
import subprocess
import time
import json
import shutil

settings = []
proclist = []
next_setting = 0

def load_settings():
    global settings
    with open('cgp_run_settings.json', 'r') as outfile:
        settings = json.load(outfile)
    for s in settings:
        if not 'runs_done' in s:
            s['runs_done'] = 0
        s['run'] = s['runs_done']

def save_settings():
    shutil.copy('cgp_run_settings.json', 'cgp_run_settings.json.bak')
    with open('cgp_run_settings.json', 'w') as outfile:
        json.dump(settings, outfile)

def get_next_setting():
    global next_setting
    n = 0
    while settings[next_setting]['run'] == settings[next_setting]['runs']:
        next_setting = (next_setting + 1) % len(settings)
        n += 1
        if n > len(settings):
            return None
    settings[next_setting]['run'] += 1
    next_setting = (next_setting + 1) % len(settings)
    return settings[next_setting]

def spawn_process(d):
    print(f"running {d['circuit_name']}")
    template = f"./build/cgp path ./data/{d['input_file']} generations {d['gen']} second-criterion true column {d['column']} lambda {d['lambda']} mutate {d['mutation']} functions {d['functions']}"
    out = f"./outs_cgp/{d['output_file']}"
    outfile = open(out, 'a')
    p = subprocess.Popen(template, shell=True, stdout=outfile, stderr=outfile)
    proclist.append((p, d))

load_settings()

if not os.path.exists("outs_cgp"):
    os.mkdir("outs_cgp")

while True:
    no_settings = False
    while len(proclist) < 5:
        s = get_next_setting()
        if not s:
            no_settings = True
            break
        spawn_process(s)
    if no_settings and not proclist:
        break
    time.sleep(5)
    ended = []
    for p in proclist:
        if p[0].poll() != None:
            print("process ended")
            ended.append(p)
    for p in ended:
        p[1]['runs_done'] += 1
        proclist.remove(p)
    save_settings()
    
print('all finished')