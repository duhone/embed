#include "Platform/MemoryMappedFile.h"
#include "Platform/PathUtils.h"
#include "core/Log.h"

#include <3rdParty/cli11.h>
#include <3rdParty/fmt.h>

#include <chrono>
#include <cstdio>

using namespace std;
using namespace std::chrono_literals;
namespace fs = std::filesystem;
using namespace CR;
using namespace CR::Core;

int main(int argc, char** argv) {
	CLI::App app{"embed"};
	string inputFileName  = "";
	string outputFileName = "";
	app.add_option("-i,--input", inputFileName, "Input file to store in code")->required();
	app.add_option("-o,--output", outputFileName,
	               "Output file and path. filename only, both a .h and a .cpp will be generated at the output location")
	    ->required();

	CLI11_PARSE(app, argc, argv);

	fs::path inputPath{inputFileName};
	fs::path outputPath{outputFileName};

	inputPath  = Platform::GetCurrentProcessPath() / inputPath;
	outputPath = Platform::GetCurrentProcessPath() / outputPath;

	if(!fs::exists(inputPath)) {
		CLI::Error error{"input file", "Input file doesn't exist", CLI::ExitCodes::FileError};
		app.exit(error);
	}
	if(outputPath.has_extension()) {
		CLI::Error error{"extension",
		                 "do not add an extension to the output file name, .h and .cpp will be appended automaticly",
		                 CLI::ExitCodes::FileError};
		app.exit(error);
	}

	return 0;
}
