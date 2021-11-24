import png
import json
import math

datau={}
datav={}

with open('uwnd23.json') as json_file:
    datau = json.load(json_file)
with open('vwnd23.json') as json_file:
    datav = json.load(json_file)
maxu = max(datau['uwnd'])
minu = min(datau['uwnd'])

maxv = max(datav['vwnd'])
minv = min(datav['vwnd'])

height = len(datav['lat'])
width = len(datav['lon'])


s = []
for y in range(0,height):
	liste = []
	for x in range(0,width):
		p = y * width + (x + width / 2) % width;
		liste.append(math.floor(255 * (datau['uwnd'][int(p)] - minu) / (maxu - minu)))
		liste.append(math.floor(255 * (datav['vwnd'][int(p)] - minv) / (maxv - minv)))
		liste.append(0)
	s.append(tuple(liste))
#print(s)	
w = png.Writer(width,height , greyscale=False)
f = open('png.png', 'wb')
w.write(f, s)
f.close()

print(maxu)
print(minu)
print(maxv)
print(minv)


 

