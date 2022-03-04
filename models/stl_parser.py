import argparse
import sys
import numpy
from pathlib import Path as path
from stl import mesh

#def create_header()

#parser = argparse.ArgumentParser(description='Convert STL model to c++ files')
#parser.add_argument("scale", type=int, help="Scale of model")
#parser.add_argument("files", metavar="file")
if len(sys.argv) < 2:
	parser.print_help()
	exit()
print(sys.argv[1])
name = path(sys.argv[1])

your_mesh = mesh.Mesh.from_file(name)
polygons_count = len(your_mesh.units)
cpp_file_name = "data_" + name.stem + ".cpp"
h_file_name = "data_" + name.stem + ".h"
print("count of polygons: " + str(polygons_count))
print("cpp file name:    " + cpp_file_name)
print("header file name: " + h_file_name)

with open(h_file_name, "w") as file:
	file.write("#ifndef __DATA_" + name.stem.upper() + "__\n")
	file.write("#define __DATA_" + name.stem.upper() + "__\n")
	file.write("const int stl_" + name.stem + "_amount = " + str(polygons_count) + ";\n\n")
	file.write("extern float stl_" + name.stem + "_vertices[]; \n")
	file.write("extern float stl_" + name.stem + "_normals[]; \n")
	file.write("\n#endif //__DATA_" + name.stem.upper() + "__\n")

with open(cpp_file_name, "w") as file:
	file.write("#include " + "\"" + h_file_name + "\"\n\n")

	file.write("float stl_" + name.stem + "_vertices[]" +  " = { \n")
	for i in range(polygons_count):
		file.write(str(your_mesh.v0[i][0]) + ", " + str(your_mesh.v0[i][1]) + ", " + str(your_mesh.v0[i][2]) + ", 1" + ", \n")
		file.write(str(your_mesh.v1[i][0]) + ", " + str(your_mesh.v1[i][1]) + ", " + str(your_mesh.v1[i][2]) + ", 1" + ", \n")
		file.write(str(your_mesh.v2[i][0]) + ", " + str(your_mesh.v2[i][1]) + ", " + str(your_mesh.v2[i][2]) + ", 1")
		if i != polygons_count - 1:
			file.write(",\n\n")
	file.write("	}; \n\n\n")

	file.write("float stl_" + name.stem + "_normals[]" +  " = { \n")
	for i in range(polygons_count):
		file.write(str(your_mesh.units[i][0]) + ", " + str(your_mesh.units[i][1]) + ", " + str(your_mesh.units[i][2]) + ", \n")
		file.write(str(your_mesh.units[i][0]) + ", " + str(your_mesh.units[i][1]) + ", " + str(your_mesh.units[i][2]) + ", \n")
		file.write(str(your_mesh.units[i][0]) + ", " + str(your_mesh.units[i][1]) + ", " + str(your_mesh.units[i][2]))
		if i != polygons_count - 1:
			file.write(",\n\n")
	file.write("	}; \n")