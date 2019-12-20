#include "ofAppNoWindow.h"
#include "ofApp.h"
#include <iostream>
#include "cxxopts.hpp"

std::string getAbsolutePath(std::string path)
{
	filesystem::path exePath = filesystem::current_path();

	if (!ofFilePath::isAbsolute(path))
	{
		path = ofFilePath::join(exePath, path);
	}
	return path;
}

cxxopts::ParseResult parse(int argc, char *argv[])
{
	try
	{
		cxxopts::Options options(argv[0], " - OSC monitor and sender");
		options.add_options()("h,host", "host", cxxopts::value<std::string>()->default_value("localhost"))("p,port", "port", cxxopts::value<int>()->default_value("8000"))("o,output", "output", cxxopts::value<std::string>())("i,interactive", "interactive")("j,input", "input", cxxopts::value<std::string>())("n,notBundled", "notBundled")("m,message", "message", cxxopts::value<std::string>())("b,broadcast", "broadcast", cxxopts::value<std::string>());
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
	if (result.count("input") > 0)
	{
		ofRunApp(new ofApp(result["host"].as<std::string>(), result["port"].as<int>(), getAbsolutePath(result["input"].as<std::string>())));
	}
	else if (result.count("message") == 0)
	{
		if (result.count("output") > 0)
		{
			ofRunApp(new ofApp(result["port"].as<int>(), true, getAbsolutePath(result["output"].as<std::string>())));
		}
		else if (result.count("broadcast") > 0)
		{
			auto port = result["port"].as<int>();
			auto receivers = result["broadcast"].as<std::string>();
			auto parts = ofSplitString(receivers, " ");
			std::vector<std::string> hosts;
			std::vector<int> ports;
			if (parts.size() % 2 == 0)
			{
				for (auto i = 0; i < parts.size(); i++)
				{
					if (i % 2 == 0)
					{
						hosts.push_back(parts[i]);
					}
					else
					{
						ports.push_back(ofToInt(parts[i]));
					}
				}
			}
			ofRunApp(new ofApp(result["port"].as<int>(), hosts, ports));
		}
		else
		{
			ofRunApp(new ofApp(result["port"].as<int>(), false, ""));
		}
	}

	else
	{
		ofRunApp(new ofApp(result["host"].as<std::string>(), result["port"].as<int>(), result["message"].as<std::string>(), result["interactive"].as<bool>(), result["notBundled"].as<bool>()));
	}
	return 0;
}