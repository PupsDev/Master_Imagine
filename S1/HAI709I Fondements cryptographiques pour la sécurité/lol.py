import math




# 9517766
for i in range(5000):
	for j in range(5000):
		if math.ceil(i*j) > 9517000 and math.ceil(i*j) < 9518000 :
			clair = open('image.ecb.ppm', 'rb').read()
			entete = 'P6\n'+str(i)+' '+str(j)+'\n255\n'
			entete2 = bytes(entete,'ascii')
			clair = entete2 + clair
			open('/tmp/test'+str(i)+'_'+str(j)+'.ppm','wb').write(clair)
