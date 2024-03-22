#include "TGProgram.h"
#include <iostream>

int main()
{	
	TGProgram::cameraMoveSpeed = 0.1f;

	char choice;
	do {
		std::cout
			<< "Choices:\n"
			<< "0 Section2\n"
			<< "1 DrawCubeByDepthShader\n"
			<< "2 StencilTestSection_CubeOutline\n"
			<< "3 BlendSection_PlateLeaf\n"
			<< "4 BlendSection_TranslucentGlass\n"
			<< "5 FaceCullSection_CubeInside\n"
			<< "6 FrameBufferSection_PostProcess\n"
			<< "7 MultiRenderTarget_DeferRenderPipeline\n"
			<< "8 TextureCubeMapSection_Skybox\n"
			<< "9 MultiRenderTarget_TexLightBuffer1000\n"
			<< "e exit\n"
			;
		std::cin >> choice;
		std::cout << "selected " << choice << std::endl;
		switch(choice) {
			case '0':
				TGProgram::Section2();
				break;
			case '1':
				TGProgram::DrawCubeByDepthShader();
				break;
			case '2':
				TGProgram::StencilTestSection_CubeOutline();
				break;
			case '3':
				TGProgram::BlendSection_PlateLeaf();
				break;
			case '4':
				TGProgram::BlendSection_TranslucentGlass();
				break;
			case '5':
				TGProgram::FaceCullSection_CubeInside();
				break;
			case '6':
				TGProgram::FrameBufferSection_PostProcess();
				break;
			case '7':
				TGProgram::MultiRenderTarget_DeferRenderPipeline();
				break;
			case '8':
				TGProgram::TextureCubeMapSection_Skybox();
				break;
			case '9':
				TGProgram::MultiRenderTarget_TexLightBuffer1000();
				break;
		}
	} while (choice != 'e');
}

