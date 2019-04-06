#include "ofAppNoWindow.h"  
#include "ofApp.h"
#include <iostream>
#include "cxxopts.hpp"

cxxopts::ParseResult
parse(int argc, char *argv[])
{
	try
	{
		cxxopts::Options options(argv[0], " - OSC monitor and sender");
		options.add_options()
			("h,host", "host", cxxopts::value<std::string>()->default_value("localhost"))
			("p,port", "port", cxxopts::value<int>()->default_value("8000"))
			("o,output", "output",  cxxopts::value<std::string>())
			("j,input", "input",  cxxopts::value<std::string>())
			("i,interactive", "interactive")
			("n,notBundled", "notBundled")
			("m,message", "message", cxxopts::value<std::string>());
		auto result = options.parse(argc, argv);
		return result;
	}
	catch (const cxxopts::OptionException &e)
	{
		std::cout << "error parsing options: " << e.what() << std::endl;
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	auto result = parse(argc, argv);
	auto arguments = result.arguments();

	ofAppNoWindow window;
	ofSetupOpenGL(&window, 0, 0, OF_WINDOW);
	if(result.count("message") == 0){
		if(result.count("output") > 0){
			ofRunApp(new ofApp(result["port"].as<int>(), true, result["output"].as<std::string>()));
		}else{
			ofRunApp(new ofApp(result["port"].as<int>(), false, ""));
		}
		if(result.count("input") > 0){
			ofRunApp(new ofApp(result["host"].as<std::string>(), result["port"].as<int>(), result["input"].as<std::string>()));
		}
	}else{
		ofRunApp(new ofApp(result["host"].as<std::string>(), result["port"].as<int>(), result["message"].as<std::string>(), result["interactive"].as<bool>(), result["notBundled"].as<bool>()));
	}
	return 0;
}