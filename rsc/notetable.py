def convert(note):
    exp = (note - 69) / 12.0
    return 2**exp * 440

out = ""

for note in range(128):
    out += str(convert(note)) + "\n"

with open("notes.table", "wt") as f:
	f.write(out)