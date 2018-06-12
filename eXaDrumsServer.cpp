#include "CppSockets.hpp"
#include "eXaDrums.h"
#include "json.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace eXaDrumsApi;

int main(int argc, char* argv[])
{

	using namespace Sockets;
	using json = nlohmann::json;

	std::vector<std::string> args(argv, argv + argc);

	const auto dataFolder{args[1]};

	eXaDrumsApi::eXaDrums drumKit{dataFolder.data()};


	const std::map<std::string, std::function<void(const json&, json&)>> methods
	{
		{"start", 			[&] (const json& params, json& res) { drumKit.Start(); }},
		{"stop", 			[&] (const json& params, json& res) { drumKit.Stop(); }},
		{"isStarted", 		[&] (const json& params, json& res) { res = drumKit.IsStarted(); }},
		{"enableMetronome", [&] (const json& params, json& res) { drumKit.EnableMetronome(params[0].get<int>()); }},
		{"getKitsNames", 	[&] (const json& params, json& res) { res = drumKit.GetKitsNames(); }}, 
		{"changeTempo", 	[&] (const json& params, json& res) { drumKit.ChangeTempo(params[0].get<int>()); }}, 
		{"getKitsNames", 	[&] (const json& params, json& res) { res = drumKit.GetKitsNames(); }}
	};


	std::cout << "Server started." << std::endl;

	// Create the socket server on port 8080
	ServerSocket server(8080);

	while(true)
	{

		ServerSocket socket;
		server.Accept(socket);


		while(true)
		{

			// Receive data
			std::string data;
			socket >> data;

			if(data.empty())
			{
				break;
			}

			// Parse data
			json query = json::parse(data);
			std::cout << "Received: " << data << std::endl;

			// Extract method name and parameters
			const std::string methodName = query["method"];
			auto params = query["params"];

			// Look for method
			auto itMethod = methods.find(methodName);
			if(itMethod == methods.end())
			{
				break;
			}

			// Get result from method
			json result;
			itMethod->second(params, result);

			// Create json response
			json response = {{"jsonrpc", "2.0"}, {"result", result}, {"id", query.at("id").get<int>()}};
			std::string reply = response.dump() + '\n';

			// Send response
			socket << reply;
			std::cout << "Replied:  " << reply << std::endl;

		}
	}


  return 0;
}
