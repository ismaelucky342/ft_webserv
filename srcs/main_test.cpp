#include "pathUtils.hpp"
#include <iostream>

// int main()
// {
// 	std::string result;

// 	const char *paths[] = {
// 		"./www/",
// 		"www/../site",
// 		"../../www",
// 		"./",
// 		".",
// 		"/var/www/../site",
// 		"/../../var/www",
// 		"/",
// 		"/var//www///",
// 		""
// 	};

// 	for (int i = 0; i < 10; ++i)
// 	{
// 		result.clear();

// 		bool valid = pathUtils::normalizeFileSystemPath(paths[i], result);

// 		std::cout << "[" << paths[i] << "] -> ";

// 		if (valid)
// 			std::cout << "[" << result << "]";
// 		else
// 			std::cout << "INVALID";

// 		std::cout << std::endl;
// 	}

// 	return 0;
// }

int main()
{
	std::string result;

	struct Test
	{
		std::string root;
		std::string requestPath;
	};

	Test tests[] = {
		{"/var/www", "/index.html"},
		{"/var/www/", "/images/logo.png"},
		{"/var/www", "/images/../logo.png"},
		{"/var/www", "/images/../../etc/passwd"},
		{"/var/www", "/../etc/passwd"},
		{"/", "/index.html"},
		{".", "/index.html"},
		{"../../www", "/index.html"},
		{"/var/www/", "/"},
		{"html/", "/"},
		{"/", "/"}
	};

	for (int i = 0; i < 11; ++i)
	{
		result.clear();

		bool valid = pathUtils::normalizePath(
			tests[i].root,
			tests[i].requestPath,
			result
		);

		std::cout << "Root: [" << tests[i].root << "] "
				  << "Request: [" << tests[i].requestPath << "] -> ";

		if (valid)
			std::cout << "[" << result << "]";
		else
			std::cout << "RECHAZADO";

		std::cout << std::endl;
	}

	return 0;
}