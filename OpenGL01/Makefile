build : main.out

start : build
	./main.out

clean :
	rm *.o main.out

BREW := $(shell brew --prefix)
INC := -I$(BREW)/include
LIB := -L$(BREW)/lib
OBJS := main.o ITGBaseLight.o TGCamera.o TGFrameBuffer.o TGInstancedMesh.o TGLocalPlayer.o TGMaterial.o TGMeshFactory.o TGMeshGeometry.o TGModel.o TGShader.o TGShaderProgram.o TGSkyBox.o TGTexture.o

main.out : ${OBJS}
	@# g++ $(LIB) -std=c++17 -static -lglfw -lGLEW -lglm -lassimp -framework OpenGL -o main.out $(OBJS)
	g++ $(LIB) -std=c++17 -lglfw -lGLEW -lglm -lassimp -framework OpenGL -o main.out $(OBJS)

main.o : main.cpp TGProgram.h TGShader.h TGShaderProgram.h stb_image.h TGCamera.h TGLocalPlayer.h TGMeshGeometry.h TGMeshFactory.h ITGBaseLight.h TGModel.h TGSkyBox.h TGFrameBuffer.h TGTexture.h TGMaterial.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

ITGBaseLight.o: ITGBaseLight.cpp ITGBaseLight.h TGShaderProgram.h TGTexture.h TGShader.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

TGCamera.o: TGCamera.cpp TGCamera.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

TGFrameBuffer.o: TGFrameBuffer.cpp TGFrameBuffer.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

TGInstancedMesh.o: TGInstancedMesh.cpp TGInstancedMesh.h TGModel.h TGTexture.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

TGLocalPlayer.o: TGLocalPlayer.cpp TGLocalPlayer.h TGLocalPlayer.h TGCamera.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

TGMaterial.o: TGMaterial.cpp TGMaterial.h TGTexture.h TGShaderProgram.h TGShader.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

TGMeshFactory.o: TGMeshFactory.cpp TGMeshFactory.h TGMeshGeometry.h TGTexture.h TGMaterial.h TGShaderProgram.h TGShader.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

TGMeshGeometry.o: TGMeshGeometry.cpp TGMeshGeometry.h TGTexture.h TGMaterial.h TGShaderProgram.h TGShader.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

TGModel.o: TGModel.cpp TGModel.h TGTexture.h TGShaderProgram.h TGCamera.h TGMeshGeometry.h TGMaterial.h TGShader.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

TGShader.o: TGShader.cpp TGShader.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

TGShaderProgram.o: TGShaderProgram.cpp TGShaderProgram.h TGShader.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

TGSkyBox.o: TGSkyBox.cpp TGSkyBox.h TGMeshGeometry.h TGTexture.h TGMaterial.h TGShaderProgram.h TGShader.h TGMeshFactory.h TGLocalPlayer.h TGCamera.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

TGTexture.o: TGTexture.cpp TGTexture.h stb_image.h
	g++ -c -g -std=c++17 $(INC) -o $@ $<

.PHONY : build start clean