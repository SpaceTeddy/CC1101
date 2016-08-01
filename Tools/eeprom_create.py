#!/usr/bin/env python
# coding:utf-8

'''creates an C++ array from an Arduino iHex EEPROM file (*.eep)
usage: ./eeprom_create.py <input *.eep file>
you get an output file with *.array extension'''

from sys import argv

# finds substring in string
def find_all(a_string, sub):
    result = []
    k = 0
    while k < len(a_string):
        k = a_string.find(sub, k)
        if k == -1:
            return result
        else:
            result.append(k)
            k += 1 #change to k += len(sub) to not search overlapping results
    return result

str_search_start_line = ':'

script, filename = argv

txt = open(filename)

print "Here's your file %r:" % filename
txt =  txt.read()
file_len = len(txt)
print("File lenght:" + str(file_len))

print(txt)

f = open(filename + ".array", 'w')

str_search_start_line = ':'
start = []
start = find_all(txt,":")
len = len(start)
#print len
#print start
loop = 1
txt_neu = ""
total_bytes = 0
f.write("Import file: " + filename + "\n")
f.write("\n")
for x in start:
    if(txt[x+7] == '0' and txt[x+8] == '1'):
        print "EOF"
        print "Output array:"
    lenght = txt[x+1] + txt[x+2]
    lenght = int(lenght, 16)
    total_bytes += lenght
    lenght = (lenght * 2) - 1
    count = 0
    loop += + 1
    while (count <= lenght):
        txt_neu += "0x" + txt[x+9+count]
        count = count + 1
        txt_neu += txt[x+9+count]
        if(loop == len and count == lenght):
            txt_neu += "};"
        else:
            txt_neu += ","
        count = count + 1
    txt_neu = txt_neu + "\n"

print ("Total Bytes: " + str(total_bytes))
print "EEPROM_LEN = " + str(total_bytes)
print "uint8_t eeprom_cc1101[EEPROM_LEN] = {"
print("//00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F")
print txt_neu
f.write("EEPROM_LEN = " + str(total_bytes) + "\n")
f.write("uint8_t eeprom_cc1101[EEPROM_LEN] = { \n")
f.write("//00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F" + "\n")
f.write(txt_neu)
f.close()
