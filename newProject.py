import os
import sys
import shutil

def create_folder(path):
	if not os.path.exists(path) :
		shutil.copytree("app/templates/project/", path)

if (len(sys.argv) == 1) :
	i = 1
	while True :
		path = "project" + str(i)
		i = i + 1
		if not os.path.exists(path) :
			shutil.copytree("app/templates/project/", path)
			break
else:
	if (len(sys.argv) == 2) :
		path = sys.argv[1]
		if not os.path.exists(path) :
			shutil.copytree("app/templates/project/", path)
	else:
		if (len(sys.argv) == 3) :
			if (sys.argv[1] == "-n" or sys.argv[1] == "--name" ):
				path = sys.argv[2]
				if not os.path.exists(path) :
					shutil.copytree("app/templates/project/", path)