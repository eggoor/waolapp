#include "Waolac.h"

int main(int argc, char* argv[])
{
	args_t args;
	args.reserve(argc);

	for (int i = 1; i < argc; ++i) {
		args.emplace_back(argv[i]);
	}

	return Waolac(std::move(args)).Run();
}
