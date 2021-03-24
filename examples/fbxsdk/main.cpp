
#include "./fbxcontext.h"
#include <fstream>

int main()
{
	FBXContext context;
	context.load(R"(D:\hk4ep4\I_P_016_HK4E_Z\ART\Char\Monster\Old\Monster_Dvalin\Output\Monster_Dvalin_Model.fbx)");

	std::ofstream outfile(R"(D:\a.json)", std::ios::out);
	auto json = context.toJSON();
	outfile << json.dump();
	return 1;
}