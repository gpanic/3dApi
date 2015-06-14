#include <iostream>

float a = 230.39;
float b = 110.90;

int main()
{
	float a = 0;
	float b = 0;

	while (true)
	{
		std::cout << "D3D ";
		std::cin >> a;

		std::cout << "OGL ";
		std::cin >> b;

		std::cout << -(100 - (100.0f / a * b)) << std::endl;
	}
}