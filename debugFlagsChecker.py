## This is a simple script, to check whether some redundant 'DEBUG', or 'TODO' comments  have been left in the codebase, before pushing to github

import os

directories_to_check = [ "includes",
						 "src"	
					]

flags = [ 'DEBUG', 'COMMENT', 'TODO']

files_to_check = []

for dir in os.listdir():
	if dir in directories_to_check:
		for filename in os.listdir('./' + dir):
			if filename.endswith(".cpp") or filename.endswith(".hpp") or filename.endswith('.h'):
				files_to_check.append( os.path.join('./',dir, filename) )

for filename in files_to_check:
	print( filename + '\n-----------------')
	with open(filename) as tmp_file:
		lines = tmp_file.readlines()
		for line in lines:
			for flag in flags:
				if flag in line :
					print("Line " + str(lines.index(line)) + " - " + line)