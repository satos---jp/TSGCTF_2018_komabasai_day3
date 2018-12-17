import base64
import sys

conv_num = 10
def conv(s):
	global conv_num
	for _ in range(conv_num):
		s = base64.b64encode(s)
	return s

def deconv(s):
	global conv_num
	try:
		for _ in range(conv_num):
			s = base64.b64decode(s)
		return s
	except TypeError:
		return None


def binl(i,l):
	return bin(i)[2:].zfill(l)

b64_table = []
for b in range(1 << 6):
	ns = base64.b64encode(chr((b << 2)))
	b64_table.append(ns[0])

b64_inv = [None for _ in range(256)]
for i,v in enumerate(b64_table):
	b64_inv[ord(v)] = i

class Invalid(Exception):
	pass

def safe_rev(bs):
	res = ""
	while len(bs)>0:
		nb = bs[:8]
		bs = bs[8:]
		if len(nb)==8:
			if chr(int(nb,2)) == '=' and len(bs)<=8*2:
				break
			c = b64_inv[int(nb,2)]
			if c is None:
				raise Invalid
			else:
				res += binl(c,6)
		else:
			pass
	return res
		
def valid(s):
	global conv_num
	try:
		s = ''.join(map(lambda c: binl(ord(c),8),s))
		#print s
		for _ in range(conv_num):
			s = safe_rev(s)
		return True
	except Invalid:
		return False
		
#bsa = "TSG{Ba5e64_1s_r0bus7}"
#tsa = conv(bsa)


tsa = "Vl0vd2QyTXlVWFxYYUGwT1[r[GOkSm[1TVZOV10VbEOYa2L1VjKJRFVGWm[NajDxWjBaR3LySjWTbGinTWhCUVZucDd[V1KIVlusalKt`F8UW2N3[TZaeD0UTlSNazE1Wke1c3FGSoNkSEJXTUdRelmpRluXR0KITmxvW01DSUBVa2LxWkKGW0OuTlZhdmyYWWynU00xVoNXbXSXUjGKRWVteFOhVmqxV1ivW0KrbGhW`jZhZDZObmqF`GhmbXhZV1ZkLFQxUnNWcmKsTkB`cWlraENSbFqY[TZOaG[scHmWLWJHWkFaRlduWlFSRYBHVWqGU3SVVoNUcWisYlinVF[tMTCWMT05WWtj`lJuUlmZbF[hVjZ`eFRHZGyVbGw1VmWWU0XvLXKW`k5aTU[wWG[qRlGVLk5IWF0FT1JVbEWWbGP1TUG`Vk1VWk4RSDE5"

nvs = [""]
for c in tsa:
	tvs = [s + c for s in nvs] + [s + chr(ord(c)^1) for s in nvs]
	tvs = filter(valid,tvs)
	print len(tvs),
	sys.stdout.flush()
	
	if len(tvs)==0:
		print nvs
		break
	nvs = tvs

nvs = [s + tsa[len(s):] for s in nvs]

for d in nvs:
	td = deconv(d)
	print td
	if td is not None:
		print conv(td)
		print (tsa == conv(td))

