#include "helpers.h"


int Helpers::clamp(const int input, const int min, const int max)
{
	const int a = (input < min) ? min : input;
	return (a > max ? max : a);
}

float Helpers::mapValues(const float x, const float in_min, const float in_max, const float out_min, const float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float Helpers::getDifficultyModifier(float mod)
{
	return 1.0f + ((mod /  10.0f)* (mod / 10.0f) / 1.9f);
}

bool Helpers::lineIntersectsRectangle(vf2d lineStart, vf2d lineEnd, Rectangle rect)
{

	auto lines = Helpers::getLinesFromRectangle(rect);

	for (auto& line : lines)
	{
		if (CheckCollisionLines(line.first, line.second, lineStart, lineEnd, nullptr))
		{
			return true;
		}
	}


	return false;
}

std::vector<std::pair<vf2d, vf2d>> Helpers::getLinesFromRectangle(Rectangle rect)
{
	float x = rect.x;
	float y = rect.y;
	vf2d topLeft ={ x, y };
	vf2d topRight ={ x + rect.width, y };
	vf2d bottomLeft ={ x, y + rect.height };
	vf2d bottomRight ={ x + rect.width, y + rect.height };

	const std::pair topLine = { topLeft, topRight };
	const std::pair rightLine = { topRight, bottomRight };
	const std::pair bottomLine = { bottomRight, bottomLeft };
	const std::pair leftLine = {  bottomLeft, topLeft };

	return std::vector {
		topLine,
		rightLine,
		bottomLine,
		leftLine
	};
}




void Helpers::DrawMainMenu()
{


}

bool Helpers::randomChance(const float required) 	{

    std::default_random_engine generator(time(0));
    std::uniform_real_distribution<double> distribution;

    const float chance = distribution(generator);

    if (chance > (required / 100.0f))
        return true;
    return false;

}