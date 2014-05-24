import re

def stars(s):
        
	l = s.split("\n")
	l = [i.strip() for i in l]
	
	m = len(max(l)) + 10

	l[0] = l[0].ljust(m,'*')

	l[-1] = "*".ljust(m,'*') + "/"

	for n,line in enumerate(l[1:-1]):
		l[n + 1] = "* " + line.strip()

	return "\n".join(l)

def findCmnt(s):

        pattern = re.compile(r'/*!')

        for m in pattern.finditer(s):
                match = s[m.start() - 2 : s.find("*/",m.end()) + 3]
                
                s = s.replace(match,stars(match))

        return "\n".join([i.strip() for i in s.split("\n")])


s = input("Input: ")

print(stars(s))
