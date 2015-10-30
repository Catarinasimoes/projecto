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

#ifndef EVMSETTINGS_HPP
#define EVMSETTINGS_HPP

#include <iostream>
#include <mutex> 

class EvmSettings {
private:
	unsigned int framerate;
	std::mutex framerate_m;

	int face;
	std::mutex face_m;

	int alpha;
	std::mutex alpha_m;

	int mode;
	std::mutex mode_m;

	unsigned int levels;
	std::mutex levels_m;

	double fl;
	std::mutex fl_m;

	double fh;
	std::mutex fh_m;

	//double colow;
	float colow;
	std::mutex colow_m;

	//double cohigh;
	float cohigh;
	std::mutex cohigh_m;

	double cowavelength;
	std::mutex cowavelength_m;

	float chromattenuation;
	std::mutex chromattenuation_m;

	bool settingsChanged;
	std::mutex settingsChanged_m;


	// Default for Motion Magnification


public:
	EvmSettings() : framerate(0), face(0), alpha(1000), levels(6), fl(0), fh(0), colow(20.0), cohigh(40.0), cowavelength(5), chromattenuation(0.1), mode(2), settingsChanged(false) {
	}


	unsigned int getFramerate() {
		std::lock_guard<std::mutex> lock(framerate_m);
		return framerate;
	}

	void setFramerate(unsigned int fps) {
		if (fps != framerate) {
			std::lock_guard<std::mutex> lock(framerate_m);
			framerate = fps;
			std::lock_guard<std::mutex> settingsChanged_lock(settingsChanged_m);
			settingsChanged = true;
		}
	}

	int getface(){
		std::lock_guard<std::mutex> lock(face_m);
		return face;
	}

	void setface(int faces) {
		if (faces != face) {
			std::lock_guard<std::mutex> lock(face_m);
			face = faces;
			std::lock_guard<std::mutex> settingsChanged_lock(settingsChanged_m);
			settingsChanged = true;
		}
	}



	int getAlpha() {
		std::lock_guard<std::mutex> lock(alpha_m);
		return alpha;
	}

	void setAlpha(int a) {
		if (a != alpha) {
			std::lock_guard<std::mutex> lock(alpha_m);
			alpha = a;
			std::lock_guard<std::mutex> settingsChanged_lock(settingsChanged_m);
			settingsChanged = true;
		}
	}

	unsigned int getLevels() {
		std::lock_guard<std::mutex> lock(levels_m);
		return levels;
	}

	void setLevels(unsigned int l) {
		if (l != levels) {
			std::lock_guard<std::mutex> lock(levels_m);
			levels = l;
			std::lock_guard<std::mutex> settingsChanged_lock(settingsChanged_m);
			settingsChanged = true;
		}
	}

	double getFl() {
		std::lock_guard<std::mutex> lock(fl_m);
		return fl;
	}

	void setFl(double fl) {
		if (fl != this->fl) {
			std::lock_guard<std::mutex> lock(fl_m);
			this->fl = fl;
			std::lock_guard<std::mutex> settingsChanged_lock(settingsChanged_m);
			settingsChanged = true;
		}
	}

	double getFh() {
		std::lock_guard<std::mutex> lock(fh_m);
		return fh;
	}

	void setFh(double fh) {
		if (fh != this->fh) {
			std::lock_guard<std::mutex> lock(fh_m);
			this->fh = fh;
			std::lock_guard<std::mutex> settingsChanged_lock(settingsChanged_m);
			settingsChanged = true;
		}
	}

	double getcolow(){
		std::lock_guard<std::mutex> lock(colow_m);
		return colow;
	}


	void setcolow(double colow) {
		if (colow != this->colow) {
			std::lock_guard<std::mutex> lock(colow_m);
			this->colow = colow;
			std::lock_guard<std::mutex> settingsChanged_lock(settingsChanged_m);
			settingsChanged = true;
		}
	}
	double getcohigh(){
		std::lock_guard<std::mutex> lock(cohigh_m);
		return cohigh;
	}


	void setcohigh(double cohigh) {
		if (cohigh != this->cohigh) {
			std::lock_guard<std::mutex> lock(cohigh_m);
			this->cohigh = cohigh;
			std::lock_guard<std::mutex> settingsChanged_lock(settingsChanged_m);
			settingsChanged = true;
		}
	}

	double getcowavelength(){
		std::lock_guard<std::mutex> lock(cowavelength_m);
		return cowavelength;
	}


	void setcowavelength(double cowavelength) {
		if (cowavelength != this->cowavelength) {
			std::lock_guard<std::mutex> lock(cowavelength_m);
			this->cowavelength = cowavelength;
			std::lock_guard<std::mutex> settingsChanged_lock(settingsChanged_m);
			settingsChanged = true;
		}
	}
	double getchromattenuation(){
		std::lock_guard<std::mutex> lock(chromattenuation_m);
		return chromattenuation;
	}


	void setchromattenuation(double chromattenuation) {
		if (chromattenuation != this->chromattenuation) {
			std::lock_guard<std::mutex> lock(chromattenuation_m);
			this->chromattenuation = chromattenuation;
			std::lock_guard<std::mutex> settingsChanged_lock(settingsChanged_m);
			settingsChanged = true;
		}
	}

	int getmode(){
		std::lock_guard<std::mutex> lock(mode_m);
		return mode;
	}


	void setmode(int mode) {
		if (mode != this->mode) {
			std::lock_guard<std::mutex> lock(mode_m);
			this->mode = mode;
			std::lock_guard<std::mutex> settingsChanged_lock(settingsChanged_m);
			settingsChanged = true;
		}
	}
	//returns true if settings have changed since last call
	bool haveChanged() {
		std::lock_guard<std::mutex> lock(settingsChanged_m);
		bool temp = settingsChanged;
		settingsChanged = false;
		return temp;
	}

	void printValues() {
		std::lock_guard<std::mutex> framerate_lock(framerate_m);
		std::lock_guard<std::mutex> alpha_lock(alpha_m);
		std::lock_guard<std::mutex> levels_lock(levels_m);
		std::lock_guard<std::mutex> fl_lock(fl_m);
		std::lock_guard<std::mutex> fh_lock(fh_m);
		std::lock_guard<std::mutex> colow_lock(colow_m);
		std::lock_guard<std::mutex> cohigh_lock(cohigh_m);
		std::lock_guard<std::mutex> cowavelength_lock(cowavelength_m);
		std::lock_guard<std::mutex> mode_lock(mode_m);
		std::lock_guard<std::mutex> face_lock(face_m);
		if (mode == 1){
			std::cout << "Current EVM settings:" << std::endl;
			std::cout << "framerate: " << framerate << std::endl;
			std::cout << "alpha: " << alpha << std::endl;
			std::cout << "levels: " << levels << std::endl;
			std::cout << "fl: " << fl << std::endl;
			std::cout << "fh: " << fh << std::endl;
			std::cout << "mode: " << mode << std::endl;
			std::cout << "face detection" << face << std::endl;
		}
		else if (mode == 2){
			std::cout << "Current EVM settings:" << std::endl;
			std::cout << "framerate: " << framerate << std::endl;
			std::cout << "alpha: " << alpha << std::endl;
			std::cout << "levels: " << levels << std::endl;
			std::cout << "colow: " << colow << std::endl;
			std::cout << "cohigh: " << cohigh << std::endl;
			std::cout << "cowavelength: " << cowavelength << std::endl;
			std::cout << "mode: " << mode << std::endl;
			std::cout << "face detection" << face << std::endl;

		}
	}
};

#endif