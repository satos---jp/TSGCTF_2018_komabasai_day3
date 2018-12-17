with open('flag') as fp:
	flag = fp.read()

import re
assert re.match('^TSG{[0-9A-Za-z_]*}$',flag)

import random
random.seed(flag)

import base64
for _ in range(10):
	flag = base64.b64encode(flag)
	
corrupt = ""
for c in flag:
	if random.random() > 0.5:
		c = chr(ord(c)^1)
	corrupt += c

print corrupt 
