

#include "loopbuilder.h"

int main(int argc, char **argv)
{
	LoopBuilder lpb;
	lpb.addLine(Point2d(0,0), Point2d(0,1));
	lpb.addLine(Point2d(0, 1.0001), Point2d(1, 1));
	lpb.addLine(Point2d(0, 1.0001), Point2d(3, 1));
	lpb.addLine(Point2d(1, 1), Point2d(1, 0));
	lpb.addLine(Point2d(1, 0), Point2d(0, 0));


	lpb.build();

	auto result = lpb.getInnerPaths();

  return 1;
}
