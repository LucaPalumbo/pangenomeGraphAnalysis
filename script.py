import math

N = 101
y  = 65
z = N-1
w = 0
while z % 2 == 0:
    z /= 2
    w+=1

z = int(z)
print(f"{z=}")

P1 = math.gcd(N,y) == 1
print(f"P1: {P1}" )

P2 = pow(y,z,N) == 1 
print(f"P2: {P2} ")


P3 = False
print(f"{w=}")
for i in range(w): 
    a = pow(y, pow(2,i) * z , N)
    print(f"boh {a}")
    if a == N-1:
        P3 = True
print(f"P3: {P3}")
