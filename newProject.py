import os
import sys
import shutil

def create_folder(path):
	if not os.path.exists(path) :
		shutil.copytree("app/templates/project/", path)

#if (len(sys.argv) == 1) :
#	i = 1
#	while True :
#		path = "project" + str(i)
#		i = i + 1
#		if not os.path.exists(path) :
#			shutil.copytree("app/templates/project/", path)
#			break
#else:
nameSrc = "app/templates/project/"
nameDst = "project"
i = 1;
if os.path.exists(nameDst) :
	while True :
		path = nameDst + str(i)
		i = i + 1
		if not os.path.exists(path) :
			nameDst = path		
			break
for counter in range(1, len(sys.argv)) :
	k = sys.argv[counter]
	if (k == '-n' or k == '--name') :
		nameDst = sys.argv[counter + 1]
		counter = counter + 1
	else :
		if(k == '-d' or k == '--debug') :
			nameSrc = "app/templates/projectdebug/"
		
if not os.path.exists(nameDst) :
	shutil.copytree(nameSrc, nameDst)
	
#if (len(sys.argv) == 2) :
#	path = sys.argv[1]
#	if not os.path.exists(path) :
#		shutil.copytree("app/templates/project/", path)
#else:
#	if (len(sys.argv) == 3) :
#		if (sys.argv[1] == "-n" or sys.argv[1] == "--name" ):
#			path = sys.argv[2]
#			if not os.path.exists(path) :
#				shutil.copytree("app/templates/project/", path)