def convert(note):
    exp = (note - 48) / 12
    return 2**exp * 440

f = open("notes.table","wt")

out = ""

for note in range(128):
    out += str(convert(note)) + "\n"

f.write(out)

f.close()
