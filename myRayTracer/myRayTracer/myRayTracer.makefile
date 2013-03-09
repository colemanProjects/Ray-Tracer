# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_Include_Path=
Release_Include_Path=

# Library paths...
Debug_Library_Path=
Release_Library_Path=

# Additional libraries...
Debug_Libraries=
Release_Libraries=

# Preprocessor definitions...
Debug_Preprocessor_Definitions=-D GCC_BUILD 
Release_Preprocessor_Definitions=-D GCC_BUILD 

# Implictly linked object files...
Debug_Implicitly_Linked_Objects=
Release_Implicitly_Linked_Objects=

# Compiler flags...
Debug_Compiler_Flags=-O0 -g 
Release_Compiler_Flags=-O2 -g 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug Release 

# Builds the Debug configuration...
.PHONY: Debug
Debug: create_folders gccDebug/Box.o gccDebug/camera.o gccDebug/EasyBMP.o gccDebug/lightSource.o gccDebug/Plane.o gccDebug/Ray.o gccDebug/raytrace.o gccDebug/Scene.o gccDebug/Shape.o gccDebug/Sphere.o gccDebug/vectorMath.o 
	g++ gccDebug/Box.o gccDebug/camera.o gccDebug/EasyBMP.o gccDebug/lightSource.o gccDebug/Plane.o gccDebug/Ray.o gccDebug/raytrace.o gccDebug/Scene.o gccDebug/Shape.o gccDebug/Sphere.o gccDebug/vectorMath.o  $(Debug_Library_Path) $(Debug_Libraries) -Wl,-rpath,./ -o ../gccDebug/myRayTracer.exe

# Compiles file Box.cpp for the Debug configuration...
-include gccDebug/Box.d
gccDebug/Box.o: Box.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c Box.cpp $(Debug_Include_Path) -o gccDebug/Box.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM Box.cpp $(Debug_Include_Path) > gccDebug/Box.d

# Compiles file camera.cpp for the Debug configuration...
-include gccDebug/camera.d
gccDebug/camera.o: camera.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c camera.cpp $(Debug_Include_Path) -o gccDebug/camera.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM camera.cpp $(Debug_Include_Path) > gccDebug/camera.d

# Compiles file EasyBMP.cpp for the Debug configuration...
-include gccDebug/EasyBMP.d
gccDebug/EasyBMP.o: EasyBMP.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c EasyBMP.cpp $(Debug_Include_Path) -o gccDebug/EasyBMP.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM EasyBMP.cpp $(Debug_Include_Path) > gccDebug/EasyBMP.d

# Compiles file lightSource.cpp for the Debug configuration...
-include gccDebug/lightSource.d
gccDebug/lightSource.o: lightSource.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c lightSource.cpp $(Debug_Include_Path) -o gccDebug/lightSource.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM lightSource.cpp $(Debug_Include_Path) > gccDebug/lightSource.d

# Compiles file Plane.cpp for the Debug configuration...
-include gccDebug/Plane.d
gccDebug/Plane.o: Plane.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c Plane.cpp $(Debug_Include_Path) -o gccDebug/Plane.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM Plane.cpp $(Debug_Include_Path) > gccDebug/Plane.d

# Compiles file Ray.cpp for the Debug configuration...
-include gccDebug/Ray.d
gccDebug/Ray.o: Ray.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c Ray.cpp $(Debug_Include_Path) -o gccDebug/Ray.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM Ray.cpp $(Debug_Include_Path) > gccDebug/Ray.d

# Compiles file raytrace.cpp for the Debug configuration...
-include gccDebug/raytrace.d
gccDebug/raytrace.o: raytrace.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c raytrace.cpp $(Debug_Include_Path) -o gccDebug/raytrace.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM raytrace.cpp $(Debug_Include_Path) > gccDebug/raytrace.d

# Compiles file Scene.cpp for the Debug configuration...
-include gccDebug/Scene.d
gccDebug/Scene.o: Scene.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c Scene.cpp $(Debug_Include_Path) -o gccDebug/Scene.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM Scene.cpp $(Debug_Include_Path) > gccDebug/Scene.d

# Compiles file Shape.cpp for the Debug configuration...
-include gccDebug/Shape.d
gccDebug/Shape.o: Shape.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c Shape.cpp $(Debug_Include_Path) -o gccDebug/Shape.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM Shape.cpp $(Debug_Include_Path) > gccDebug/Shape.d

# Compiles file Sphere.cpp for the Debug configuration...
-include gccDebug/Sphere.d
gccDebug/Sphere.o: Sphere.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c Sphere.cpp $(Debug_Include_Path) -o gccDebug/Sphere.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM Sphere.cpp $(Debug_Include_Path) > gccDebug/Sphere.d

# Compiles file vectorMath.cpp for the Debug configuration...
-include gccDebug/vectorMath.d
gccDebug/vectorMath.o: vectorMath.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c vectorMath.cpp $(Debug_Include_Path) -o gccDebug/vectorMath.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM vectorMath.cpp $(Debug_Include_Path) > gccDebug/vectorMath.d

# Builds the Release configuration...
.PHONY: Release
Release: create_folders gccRelease/Box.o gccRelease/camera.o gccRelease/EasyBMP.o gccRelease/lightSource.o gccRelease/Plane.o gccRelease/Ray.o gccRelease/raytrace.o gccRelease/Scene.o gccRelease/Shape.o gccRelease/Sphere.o gccRelease/vectorMath.o 
	g++ gccRelease/Box.o gccRelease/camera.o gccRelease/EasyBMP.o gccRelease/lightSource.o gccRelease/Plane.o gccRelease/Ray.o gccRelease/raytrace.o gccRelease/Scene.o gccRelease/Shape.o gccRelease/Sphere.o gccRelease/vectorMath.o  $(Release_Library_Path) $(Release_Libraries) -Wl,-rpath,./ -o ../gccRelease/myRayTracer.exe

# Compiles file Box.cpp for the Release configuration...
-include gccRelease/Box.d
gccRelease/Box.o: Box.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c Box.cpp $(Release_Include_Path) -o gccRelease/Box.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM Box.cpp $(Release_Include_Path) > gccRelease/Box.d

# Compiles file camera.cpp for the Release configuration...
-include gccRelease/camera.d
gccRelease/camera.o: camera.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c camera.cpp $(Release_Include_Path) -o gccRelease/camera.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM camera.cpp $(Release_Include_Path) > gccRelease/camera.d

# Compiles file EasyBMP.cpp for the Release configuration...
-include gccRelease/EasyBMP.d
gccRelease/EasyBMP.o: EasyBMP.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c EasyBMP.cpp $(Release_Include_Path) -o gccRelease/EasyBMP.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM EasyBMP.cpp $(Release_Include_Path) > gccRelease/EasyBMP.d

# Compiles file lightSource.cpp for the Release configuration...
-include gccRelease/lightSource.d
gccRelease/lightSource.o: lightSource.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c lightSource.cpp $(Release_Include_Path) -o gccRelease/lightSource.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM lightSource.cpp $(Release_Include_Path) > gccRelease/lightSource.d

# Compiles file Plane.cpp for the Release configuration...
-include gccRelease/Plane.d
gccRelease/Plane.o: Plane.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c Plane.cpp $(Release_Include_Path) -o gccRelease/Plane.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM Plane.cpp $(Release_Include_Path) > gccRelease/Plane.d

# Compiles file Ray.cpp for the Release configuration...
-include gccRelease/Ray.d
gccRelease/Ray.o: Ray.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c Ray.cpp $(Release_Include_Path) -o gccRelease/Ray.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM Ray.cpp $(Release_Include_Path) > gccRelease/Ray.d

# Compiles file raytrace.cpp for the Release configuration...
-include gccRelease/raytrace.d
gccRelease/raytrace.o: raytrace.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c raytrace.cpp $(Release_Include_Path) -o gccRelease/raytrace.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM raytrace.cpp $(Release_Include_Path) > gccRelease/raytrace.d

# Compiles file Scene.cpp for the Release configuration...
-include gccRelease/Scene.d
gccRelease/Scene.o: Scene.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c Scene.cpp $(Release_Include_Path) -o gccRelease/Scene.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM Scene.cpp $(Release_Include_Path) > gccRelease/Scene.d

# Compiles file Shape.cpp for the Release configuration...
-include gccRelease/Shape.d
gccRelease/Shape.o: Shape.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c Shape.cpp $(Release_Include_Path) -o gccRelease/Shape.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM Shape.cpp $(Release_Include_Path) > gccRelease/Shape.d

# Compiles file Sphere.cpp for the Release configuration...
-include gccRelease/Sphere.d
gccRelease/Sphere.o: Sphere.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c Sphere.cpp $(Release_Include_Path) -o gccRelease/Sphere.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM Sphere.cpp $(Release_Include_Path) > gccRelease/Sphere.d

# Compiles file vectorMath.cpp for the Release configuration...
-include gccRelease/vectorMath.d
gccRelease/vectorMath.o: vectorMath.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c vectorMath.cpp $(Release_Include_Path) -o gccRelease/vectorMath.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM vectorMath.cpp $(Release_Include_Path) > gccRelease/vectorMath.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug
	mkdir -p ../gccDebug
	mkdir -p gccRelease
	mkdir -p ../gccRelease

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug/*.o
	rm -f gccDebug/*.d
	rm -f ../gccDebug/*.a
	rm -f ../gccDebug/*.so
	rm -f ../gccDebug/*.dll
	rm -f ../gccDebug/*.exe
	rm -f gccRelease/*.o
	rm -f gccRelease/*.d
	rm -f ../gccRelease/*.a
	rm -f ../gccRelease/*.so
	rm -f ../gccRelease/*.dll
	rm -f ../gccRelease/*.exe

