import os
full_list = []
dir_pwd = "E:\\projekt_gamma\\pomysły wstępne"

for root, subdirs, files in os.walk(dir_pwd):
    # print(root, " - ", subdirs, " - " , files)
    for file in files:
        if file.__contains__(".txt"):
            full_list.append(f"{root}\\{file}")
            # print(f"{root}\\{file}")

# print(len(full_list))


import random

import pyttsx3

choice = random.choice(full_list)
full = ""

with open(choice, 'r', encoding='utf8') as f:
    # print("--------", choice.split("\\")[-1], "--------")
    print(choice)
    title = choice.split("\\")[-1]
    full += f"""{title}.\n\n"""

    # for line in f:
    for line in enumerate(f): 
        line_s = (line[1])
        # print(line_s)
        if len(line_s) > 2:
            help = line_s.replace("-", " ").replace("\n", ".")
            full += f"""{help}\n"""




try:
    print(full)
    engine = pyttsx3.init()
    engine.setProperty('rate', 145)
    engine.setProperty('voice', "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens\TTS_MS_PL-PL_PAULINA_11.0")
    engine.say(full)
    engine.runAndWait()
finally:
    if engine is not None:
        engine.stop()