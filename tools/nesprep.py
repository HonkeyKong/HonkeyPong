#!/usr/bin/python

# nesprep.py - NES ROM Preparation Tool
# This is a simple tool used to split a NES ROM's PRG and CHR banks
# into separate binary files, and optionally inflate their sizes to 
# prepare them for burning to an EPROM or write to a flash cartridge.

# Copyright (c) 2015 Ryan D. Souders

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import sys, os
from mmap import mmap

class load():

	romFile = None
	romHeader = None
	prgBanks = 0
	chrBanks = 0
	prgSize = 0
	chrSize = 0
	megSize = float(131072) # Cast as float for compatibility with mappers which support banks smaller than 1 megabit.

	def __init__(self, fileName):
		self.romFile = open(fileName, 'r+b')
		self.romHeader = mmap(self.romFile.fileno(), 16)
		self.prgBanks = ord(self.romHeader[4])
		self.chrBanks = ord(self.romHeader[5])
		self.prgSize = self.prgBanks * 16384
		self.chrSize = self.chrBanks * 8192

	def isValidNES(self):
		if self.romHeader[:4] == 'NES\x1A':
			return True
		else:
			print "%s is Not a valid NES file." % self.romFile.name
			return False
	
	def splitPRG(self):
		prgData = mmap(self.romFile.fileno(), 0)
		prgFile = open("%s.PRG" % self.romFile.name, 'w+b')
		prgData.seek(16)
		prgFile.write(prgData.read(self.prgSize))
		prgFile.close()
		if(quietMode == False):
			print "PRG data (%i bytes) extracted to %s.PRG" % (self.prgSize, self.romFile.name)

	def splitCHR(self):
		if(self.chrBanks == 0):
			if(quietMode == False):
				print ("No CHR banks to extract. Game probably uses CHR-RAM.")
		else:
			chrData = mmap(self.romFile.fileno(), 0)
			chrFile = open("%s.CHR" % self.romFile.name, 'w+b')
			chrData.seek(16+self.prgSize)
			chrFile.write(chrData.read(self.chrSize))
			chrFile.close()
			if(quietMode == False):
				print "CHR data (%i bytes) extracted to %s.CHR" % (self.chrSize, self.romFile.name)

	def inflatePRG(self, infSize):
		cloneCount = infSize / (self.prgSize / self.megSize)
		if(quietMode == False):
			print "PRG data will be written %i times." % cloneCount
		prgData = mmap(self.romFile.fileno(), 0)
		infPRGFile = open("%s.PRG.bin" % self.romFile.name, 'w+b')
		count = 0
		while (count < cloneCount):
			prgData.seek(16)
			infPRGFile.write(prgData.read(self.prgSize))
			count += 1
		infPRGFile.close()
		if(quietMode == False):
			print "PRG data written to %s.PRG.bin (%i bytes)" % (self.romFile.name, (self.megSize * infSize))

	def inflateCHR(self, infSize):
		cloneCount = infSize / (self.chrSize / self.megSize)
		if(quietMode == False):
			print "CHR data will be written %i times." % cloneCount
		chrData = mmap(self.romFile.fileno(), 0)
		infCHRFile = open("%s.CHR.bin" % self.romFile.name, 'w+b')
		count = 0
		while (count < cloneCount):
			chrData.seek(16+self.prgSize)
			infCHRFile.write(chrData.read(self.chrSize))
			count += 1
		infCHRFile.close()
		if(quietMode == False):
			print "CHR data written to %s.CHR.bin (%i bytes)" % (self.romFile.name, (self.megSize * infSize))

	def __exit__(self):
		self.romFile.close()
#end ROM class 

if __name__ == "__main__":

	prgSplit = False
	chrSplit = False
	prgInflate = False
	chrInflate = False
	quietMode = False
	prgSize = 0
	chrSize = 0
	prgInfSize = 0
	chrInfSize = 0

	if (len(sys.argv) < 2):
		print "Usage: nesprep.py filename [-splitPRG] [-splitCHR] [-inflatePRG=megs] [-inflateCHR=megs] [-quiet]"
		print "Extracts PRG and CHR data from iNES format ROM files, and inflates for EPROM burning."
		print "Note: \"megs\" variable is in megaBITS, not megaBYTES."
	elif (len(sys.argv) > 2):
		for arg in sys.argv:
			if (arg == "-splitCHR"):
				chrSplit = True
			if (arg == "-splitPRG"):
				prgSplit = True
			if (arg[:11] == "-inflatePRG"):
				prgInflate = True
				prgInfSize = float(arg.split('=')[-1])
			if (arg[:11] == "-inflateCHR"):
				chrInflate = True
				chrInfSize = float(arg.split('=')[-1])
			if (arg[:6] == "-quiet"):
				quietMode = True
		nesFile = load(sys.argv[1])
		if(nesFile.isValidNES()):
			if(prgSplit):
				if(quietMode == False):
					print "Extracting PRG Data..."
				nesFile.splitPRG()
			if(prgInflate):
				if(quietMode == False):
					print "Inflating PRG-ROM to %i megabits..." % prgInfSize
				nesFile.inflatePRG(prgInfSize)
			if(chrSplit):
				if(quietMode == False):
					print "Extracting CHR Data..."
				nesFile.splitCHR()
			if(chrInflate):
				if(quietMode == False):
					print "Inflating CHR-ROM to %i megabits..." % chrInfSize
				nesFile.inflateCHR(chrInfSize)

	else:
		print "Error: You must specify options."