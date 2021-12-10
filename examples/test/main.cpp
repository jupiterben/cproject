

#include "profile_builder.h"

int main(int argc, char **argv)
{
	ProfileBuilder profileBuilder;
	std::vector<Line2d> lines =
		{
			Line2d(Point2d(263, 157), Point2d(248, 157)),
			Line2d(Point2d(248, 157), Point2d(248, 75)),
			Line2d(Point2d(248, 75), Point2d(142, 75)),
			Line2d(Point2d(142, 75), Point2d(142, -229)),

			Line2d(Point2d(142, 57), Point2d(263, 57)),
			Line2d(Point2d(263, 57), Point2d(263, 75)),
			Line2d(Point2d(142, -229), Point2d(508, -229)),
			Line2d(Point2d(508, -229), Point2d(508, 233)),

			Line2d(Point2d(508, 233), Point2d(263, 233)),
			Line2d(Point2d(263, 233), Point2d(263, 157)),
		};

	auto result = profileBuilder.build(lines, 0.0001);

	return 1;
}
