
#include "./fbxcontext.h"
#include <fstream>

int main()
{
	FBXContext context;
	
	auto input = R"(D:\hk4ep4\I_P_016_HK4E_Z\ART\Char\Monster\Old\Monster_Dvalin\Output\Monster_Dvalin_Model.fbx)";
	auto output = R"(D:\a.json)";

	context.load(input);
	context.exportToASCII(output);
	//std::ofstream outfile(output, std::ios::out);
	//auto json = context.toJSON();
	//outfile << json.dump();
	return 1;
}