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

static const char* c_headerProto = R"(#pragma once
#include <core/Span.h>

namespace CR::embed{{
	const CR::Core::Span<const std::byte> Get{}();
}}
)";

static const char* c_srcProtoBegin = R"(#include <core/Span.h>

const CR::Core::Span<const std::byte> CR::embed::Get{}(){{
	static const std::byte data[] = {{)";

static const char* c_srcProtoEnd = R"(
	};
	return const CR::Core::Span<const std::byte>(data);
}
)";

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

	fs::path outputHeader = outputPath.replace_extension(".h");
	fs::path outputSrc    = outputPath.replace_extension(".cpp");
	string varName        = outputPath.filename().replace_extension("").string();

	Platform::MemoryMappedFile inputData(inputPath);

	{
		std::string header = fmt::format(c_headerProto, varName);
		ofstream headerFile(outputHeader);
		headerFile << header;
	}
	{
		std::string protoBegin = fmt::format(c_srcProtoBegin, varName);
		ofstream srcFile(outputSrc);
		srcFile << protoBegin;

		for(size_t i = 0; i < inputData.size(); ++i) {
			if(i % 18 == 0) { srcFile << "\n\t\t"; }
			std::array<char, 10> str;
			auto [strEnd, err] = to_chars(data(str), data(str) + size(str), (uint8_t)inputData[i], 16);
			srcFile << "0x" << std::string_view(data(str), strEnd - data(str));
			if(i != inputData.size() - 1) { srcFile << ", "; }
		}

		srcFile << c_srcProtoEnd;
	}

	return 0;
}
