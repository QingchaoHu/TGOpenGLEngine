#include "TGProgram.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <map>

typedef void DemoFunc();

struct DemoCase {
	std::string name;
	DemoFunc *func;
};

#define Demo(func) { #func, func }

std::map<char, DemoCase> demoCase = {
	{ '0', Demo(TGProgram::Section2) },
	{ 'a', Demo(TGProgram::DrawCubeByDepthShader) },
	{ '1', Demo(TGProgram::DrawCubeByDepthShader) },
	{ '2', Demo(TGProgram::StencilTestSection_CubeOutline) },
	{ '3', Demo(TGProgram::BlendSection_PlateLeaf) },
	{ '4', Demo(TGProgram::BlendSection_TranslucentGlass) },
	{ '5', Demo(TGProgram::FaceCullSection_CubeInside) },
	{ '6', Demo(TGProgram::FrameBufferSection_PostProcess) },
	{ '7', Demo(TGProgram::MultiRenderTarget_DeferRenderPipeline) },
	{ '8', Demo(TGProgram::TextureCubeMapSection_Skybox) },
	{ '9', Demo(TGProgram::MultiRenderTarget_TexLightBuffer1000) },
};

void listCase() {
	std::cout << "Choices: " << std::endl;
	auto iter = demoCase.begin();
	while (iter != demoCase.end()) {
		std::cout << iter->first << ") " << iter->second.name << std::endl;
		++iter;
	}
	std::cout << "e) exit" << std::endl;
}

bool showCase(char choice) {
	auto c = demoCase.find(choice);

	if (c != demoCase.end()) {
		std::cout << "selected " << choice << ") " << c->second.name << std::endl;
		c->second.func();
		return true;
	}
	
	return false;
};

int main(int argc, char **argv)
{	
	std::cout << "OpenGL cases" << std::endl;
	// std::cout << "cwd = " << argv[0] << std::endl;

	TGProgram::cameraMoveSpeed = 0.1f;

	if (argc > 1) {
		char choice = argv[1][0];
		if (!showCase(choice)) {
			return 0;
		}
		return 1;
	}

	char choice;
	while (true) {
		listCase();
		std::cin >> choice;
		if (!showCase(choice)) {
			std::cout << "Exit" << std::endl;
			break;
		}
	};

	return 0;
}

