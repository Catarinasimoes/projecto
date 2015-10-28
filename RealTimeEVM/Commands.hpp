/**
* Real Time non-envasive heartbeat reader using "Eulerian Video Magnification" algorithm.
* Uses the OpenCV library and Microsoft Kinect One
* Based on the work of:
*      Jens Schindel		<kontakt@jens-schindel.de>
*      Joseph Pan			<https://github.com/wzpan/QtEVM>
*      Nick D'Ademo			<https://github.com/nickdademo/qt-opencv-multithreaded>
* @author Francisco Salgado <salgfrancisco@gmail.com>
* @version: 1.0
*/

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "EvmSettings.hpp"

using namespace std;


/**
Parses a string into settings commands and changes settings accordingly.
@param input The string to parse.
@param settings This program settings.
@return true if the string was a valid command.
*/
bool parseInput(string input, EvmSettings &settings) {
	if (input.length() == 0) {
		return false;
	}

	//parse string
	stringstream ss(input);
	vector<string> arguments;
	string item;
	while (getline(ss, item, ' ')) {
		if (!item.empty()) {
			arguments.push_back(item);
		}
	}

	//handle each argument
	switch (arguments.size())
	{
	case 1: if (arguments.at(0).compare("show") == 0) {
		settings.printValues();
		break;
	}
			std::cerr << "Error: command not valid!" << endl;
			break;
	case 2:
		if (arguments.at(0).compare("mode") == 0) {
			int mode = std::stoi(arguments.at(1));
			settings.setmode(mode);
			std::cout << "Mode is now: Color" << endl;
			break;
		}
		if (arguments.at(0).compare("alpha") == 0) {
			int alpha = std::stoi(arguments.at(1));
			settings.setAlpha(alpha);
			std::cout << "Alpha is now: " << settings.getAlpha() << endl;
			break;
		}
		if (arguments.at(0).compare("framerate") == 0) {
			unsigned int framerate = std::stoi(arguments.at(1));
			settings.setFramerate(framerate);
			std::cout << "Framerate is now: " << settings.getFramerate() << endl;
			break;
		}
		if (arguments.at(0).compare("face") == 0) {
			unsigned int framerate = std::stoi(arguments.at(1));
			settings.setFramerate(framerate);
			std::cout << "Face detection is now: " << settings.getface() << endl;
			break;
		}
		if (arguments.at(0).compare("levels") == 0) {
			unsigned int levels = std::stoi(arguments.at(1));
			settings.setLevels(levels);
			std::cout << "Levels is now: " << settings.getLevels() << endl;
			break;
		}
		if (arguments.at(0).compare("fl") == 0) {
			double fl = std::stod(arguments.at(1));
			settings.setFl(fl);
			std::cout << "Fl is now: " << settings.getFl() << endl;
			break;
		}
		if (arguments.at(0).compare("fh") == 0) {
			double fh = std::stod(arguments.at(1));
			settings.setFh(fh);
			std::cout << "Fh is now: " << settings.getFh() << endl;
			break;
		}


		/*std::cerr << "Error: command not valid!" << endl;
		break;*/

	case 3:
		if (arguments.at(0).compare("mode") == 1) {
			int mode = std::stoi(arguments.at(1));
			settings.setmode(mode);
			std::cout << "Mode is now: Motion " << endl;
			break;
		}
		if (arguments.at(0).compare("face") == 2) {
			unsigned int framerate = std::stoi(arguments.at(1));
			settings.setFramerate(framerate);
			std::cout << "Face detection is now: " << settings.getface() << endl;
			break;
		}
		if (arguments.at(0).compare("alpha") == 0) {
			int alpha = std::stoi(arguments.at(1));
			settings.setAlpha(alpha);
			std::cout << "Alpha is now: " << settings.getAlpha() << endl;
			break;
		}
		if (arguments.at(0).compare("framerate") == 0) {
			unsigned int framerate = std::stoi(arguments.at(1));
			settings.setFramerate(framerate);
			std::cout << "Framerate is now: " << settings.getFramerate() << endl;
			break;
		}
		if (arguments.at(0).compare("levels") == 0) {
			unsigned int levels = std::stoi(arguments.at(1));
			settings.setLevels(levels);
			std::cout << "Levels is now: " << settings.getLevels() << endl;
			break;
		}
		if (arguments.at(0).compare("colow") == 0) {
			double colow = std::stod(arguments.at(1));
			settings.setcolow(colow);
			std::cout << "colow is now: " << settings.getcolow() << endl;
			break;
		}
		if (arguments.at(0).compare("cohigh") == 0) {
			double cohigh = std::stod(arguments.at(1));
			settings.setcohigh(cohigh);
			std::cout << "cohigh is now: " << settings.getcohigh() << endl;
			break;
		}
		if (arguments.at(0).compare("cowavelength") == 0) {
			double cowavelength = std::stod(arguments.at(1));
			settings.setcowavelength(cowavelength);
			std::cout << "cowavelength is now: " << settings.getcowavelength() << endl;
			break;
		}

		std::cerr << "Error: command not valid!" << endl;
		break;

	default:
		std::cerr << "Error: command not valid!" << endl;
		break;
	}

	return true;
}


#endif
