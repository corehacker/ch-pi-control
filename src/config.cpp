/*******************************************************************************
 *
 *  BSD 2-Clause License
 *
 *  Copyright (c) 2020, Sandeep Prakash
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

/*******************************************************************************
 * Copyright (c) 2020, Sandeep Prakash <123sandy@gmail.com>
 *
 * \file   config.hpp
 *
 * \author Sandeep Prakash
 *
 * \date   Apr 11, 2020
 *
 * \brief
 *
 ******************************************************************************/

#include <string>
#include <glog/logging.h>

#include "config.hpp"

using std::ifstream;

namespace PC {

Config::Config() :
		ChCppUtils::Config("/etc/ch-pi-control/ch-pi-control.json",
				"./config/ch-pi-control.json") {
	hostname = "localhost";
	port = 8888;
	prefix = "/";
	name = "pi-control";
}

Config::~Config() {
	LOG(INFO) << "*****************~Config";
}

void Config::init() {
	ChCppUtils::Config::init();

	populateConfigValues();
}

bool Config::populateConfigValues() {
	LOG(INFO) << "<-----------------------Config";

	name = mJson["name"];
	LOG(INFO) << "name : " << name;

	port = mJson["server"]["port"];
	LOG(INFO) << "server.port : " << port;

	lightPin = mJson["light"]["pin"];
	LOG(INFO) << "light.pin : " << lightPin;

	lightOnRoute = mJson["light"]["on"]["route"];
	LOG(INFO) << "light.on.route : " << lightOnRoute;

	lightOffRoute = mJson["light"]["off"]["route"];
	LOG(INFO) << "light.off.route : " << lightOffRoute;

	lightTimeoutSeconds = mJson["light"]["timeout-s"];
	LOG(INFO) << "light.timeout-s : " << lightTimeoutSeconds;

	motionDetectorPin = mJson["motion-detector"]["pin"];
	LOG(INFO) << "motion-detector.pin : " << motionDetectorPin;

	motionDetectorWindowSeconds = mJson["motion-detector"]["window-s"];
	LOG(INFO) << "motion-detector.window-s : " << motionDetectorWindowSeconds;

	switchControlPin = mJson["switch-control"]["pin"];
	LOG(INFO) << "switch-control.pin : " << switchControlPin;

	switchControlPollFrquencyMs = mJson["switch-control"]["poll-frquency-ms"];
	LOG(INFO) << "switch-control.poll-frquency-ms : " << switchControlPollFrquencyMs;

	LOG(INFO) << "----------------------->Config";
	return true;
}

uint16_t Config::getPort() {
	return port;
}

string &Config::getName() {
	return name;
}

uint32_t Config::getLightPin() {
	return lightPin;
}

string Config::getLightOnRoute() {
	return lightOnRoute;
}

string Config::getLightOffRoute() {
	return lightOffRoute;
}

uint64_t Config::getLightTimeoutSeconds() {
	return lightTimeoutSeconds;
}

uint32_t Config::getMotionDetectorPin() {
	return motionDetectorPin;
}

uint64_t Config::getMotionDetectorWindowSeconds() {
	return motionDetectorWindowSeconds;
}

uint32_t Config::getSwitchControlPin() {
	return switchControlPin;
}

uint32_t Config::getSwitchControlPollFrquencyMs() {
	return switchControlPollFrquencyMs;
}

} // End namespace PC.
