from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from Crypto.Random.random import randrange
from Crypto.Util.strxor import strxor as xor
from Crypto.Util.number import bytes_to_long, long_to_bytes
from hashlib import md5, sha3_256, sha1

def Enc(bloc, clef):
    if not isinstance(bloc, bytes) or not isinstance(clef, bytes):
        raise TypeError("le bloc et la clef doivent être de type bytes")
    if len(bloc) != 16 or len(clef) != 16:
        raise ValueError("le bloc et la clef doivent faire 16 octets")
    return AES.new(clef,AES.MODE_ECB).encrypt(bloc)

def Dec(bloc, clef):
    if not isinstance(bloc, bytes) or not isinstance(clef, bytes):
        raise TypeError("le bloc et la clef doivent être de type bytes")
    if len(bloc) != 16 or len(clef) != 16:
        raise ValueError("le bloc et la clef doivent faire 16 octets")
    return AES.new(clef,AES.MODE_ECB).decrypt(bloc)


def pad(s):
    bit1 = 0b1000000
    bit0 = 0b0000000
    s+=bit1.to_bytes(1, byteorder='little')
    while(len(s)%16!=0):
        s+=bit0.to_bytes(1, byteorder='little')

    return s

def unpad(s):
    bit1 = 0b1000000
    bit0 = 0b0000000

    while(s[-1]!= bit1):
        if s[-1] != bit0 or len(s)<1:
            print("message non correctement paddé")
        s = s[0:-2]
    return s[0:-1]

def Enc_ECB(clair,clef):
    lenClair = len(clair)
    nbBlock = lenClair/16
    print("nombre de blocs " + str(nbBlock))
    i = 0
    cryptedS = b''
    if nbBlock>0:
        while(i<nbBlock-1):
            print("iteration " + str(i) + "/"+str(nbBlock))
            clairBloc = clair[i*16:i*16+16]
            i+=1
            cryptedS+=Enc(clairBloc,clef)
    clairBloc = clair[i*16:]
    cryptedS+=Enc(pad(clairBloc),clef)
    return cryptedS

def Dec_ECB(chiffre,clef):
    lenChiffre = len(chiffre)
    nbBlock = lenChiffre/16
    clairS = b''
    for i in range(int(nbBlock)):
        chiffreBloc = chiffre[i*16:i*16+16]
        clairS += Dec(chiffreBloc, clef)
    return unpad(clairS)

def Enc_CBC(clair,clef):

    cryptedS = b''
    salt = get_random_bytes(16)

    vec = salt
    lenClair = len(clair)
    nbBlock = lenClair/16
    i = 0
    cryptedS = b''
    if nbBlock>0:
        while(i<nbBlock-1):
            clairBloc = clair[i*16:i*16+16]
            saltedClair = xor(clairBloc,salt)
            i+=1
            salt = Enc(saltedClair,clef)
            cryptedS+=salt
            
    clairBloc = clair[i*16:]
    saltedClair = xor(pad(clairBloc),salt)
    cryptedS+=Enc(saltedClair,clef)

    return (cryptedS,vec)

def Dec_CBC(chiffre,clef):
    vec = chiffre[1]
    chiffre=chiffre[0]

    lenChiffre = len(chiffre)
    nbBlock = lenChiffre/16
    clairS = b''

    for i in range(int(nbBlock)):
        chiffreBloc = chiffre[i*16:i*16+16]
        decrypt = Dec(chiffreBloc, clef)
        clair = xor(decrypt,vec)
        vec = chiffreBloc
        clairS += clair

    return unpad(clairS)

def Enc_OFB(clair,clef):

    cryptedS = b''
    salt = get_random_bytes(16)

    vec = salt
    lenClair = len(clair)
    nbBlock = lenClair/16
    i = 0
    cryptedS = b''
    if nbBlock>0:
        while(i<nbBlock-1):
            clairBloc = clair[i*16:i*16+16]
            salt = Enc(salt,clef)
            saltedClair = xor(clairBloc,salt)
            cryptedS+=saltedClair
            i+=1
            
    clairBloc = clair[i*16:]
    salt = Enc(salt,clef)
    cryptedS+= xor(pad(clairBloc),salt)

    return (cryptedS,vec)

def Dec_OFB(chiffre,clef):
    vec = chiffre[1]    
    chiffre=chiffre[0]

    lenChiffre = len(chiffre)
    nbBlock = lenChiffre/16
    clairS = b''

    for i in range(int(nbBlock)):
        chiffreBloc = chiffre[i*16:i*16+16]
        vec = Enc(vec,clef)
        clair = xor(chiffreBloc,vec)
        clairS += clair

    return unpad(clairS)

def printBinary(s):
    print("Binary:")
    for l in s :
        print(bin(l))

# s = b'Cryptographie'
# print(s[0])
# print(s[4])
# print(s[-2])

# print(s[7:10])
# print( bin(s[0]) == bin( (b'x')[0]))

# paddedS =pad(s)
# print(Enc(paddedS, paddedS))
# unpaddedS =unpad(paddedS)
# print(unpaddedS)

# clair = b'C etait sur en fait'
# clef =pad(b'Sardoche')

# print("Message clair")
# print(clair)
# chiffre = Enc_ECB(clair , clef)
# print("Message chiffre")
# print(chiffre)
# clair2 = Dec_ECB(chiffre,clef)
# print("Message clair")
# print(clair2)


# chiffre = Enc_CBC(clair,clef)
# print("Message chiffre CBC")
# print(chiffre)

# clair2 = Dec_CBC(chiffre,clef)
# print("Message clair CBC")
# print(clair2)


# chiffre = Enc_OFB(clair,clef)
# print("Message chiffre OFB")
# print(chiffre)

# clair2 = Dec_OFB(chiffre,clef)
# print("Message clair OFB")
# print(clair2)

clef =pad(b'Sardoche')
clair = open('tux.ppm', 'rb').read()
chiffre = Enc_ECB(clair,clef)
open('tux.ecb.ppm','wb').write(chiffre)