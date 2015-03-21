import math

lin = []
sin = []
sqrt = []

sinsc = []
sqrtsc = []

m = math.pi / 2
q = math.sqrt(2) / 2

for n in range(201):

    val = n - 100

    left = (100 - val) / 200

    right = (100 + val) / 200

    lin.append((left,right))

    sin.append((math.sin(left * m), math.sin(right * m)))

    sqrt.append((math.sqrt(left),math.sqrt(right)))

    sinsc.append((sin[n][0] * q, sin[n][1] * q))

    sqrtsc.append((sqrt[n][0] * q, sqrt[n][1] * q))


tables = [lin,sin,sqrt,sinsc,sqrtsc]

names = ["linear", "sine", "sqrt", "sine_scaled", "sqrt_scaled"]

for n,t in zip(names,tables):

    t = ["{} {}".format(i[0], i[1]) for i in t]

    s = "\n".join(t)

    with open(n+".table","wt") as f:

        f.write(s)
