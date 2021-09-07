import os
import errno

filename = "folder.txt"

file1 = open(filename, encoding='utf-8')
Lines = file1.readlines()
print(Lines)
for line in Lines:
	line = ' '.join(line.split()).replace("/", "-")
	os.mkdir(line)
	print(line)


