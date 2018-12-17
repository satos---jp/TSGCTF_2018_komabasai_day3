from Crypto.Util import number 
from itertools import permutations

with open('flag') as fp:
	flag = fp.read()

M = int(flag.encode('hex'),16)
p = number.getPrime(1024)
q = number.getPrime(1024)
N = p*q
e = 65537

assert M < N
print "N = %d" % N
print "e = %d" % e

for a in 'MNe':
	for b in 'MNe':
		for c in 'MNe':
			print "pow(%s,%s,%s) = %d" % (a,b,c,pow(eval(a),eval(b),eval(c)))


