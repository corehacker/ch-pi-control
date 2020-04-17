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
 * \file   light-context.cpp
 *
 * \author Sandeep Prakash
 *
 * \date   April 11, 2020
 *
 * \brief
 *
 ******************************************************************************/

#include <glog/logging.h>

#include "light-context.hpp"

#include <wiringPi.h>

namespace PC {

LightContext::LightContext(Config *config) : mTimerMutex() {
	mConfig = config;

	mTimerEvent = nullptr;
	mTimer = new Timer();
	
	wiringPiSetup();

	mPin = mConfig->getLightPin();	

	pinMode(mPin, OUTPUT);

	LOG(INFO) << "Light pin: " << mPin << ", Mode: OUTPUT";

}

LightContext::~LightContext() {
}

void LightContext::_onTimerEvent(TimerEvent *event, void *this_) {
	LightContext *lightContext = (LightContext *) this_;
	lightContext->onTimerEvent(event);
}

void LightContext::onTimerEvent(TimerEvent *event) {
	LOG(INFO) << "Timer fired. Switching off.";
	off();

	LOG(INFO) << "Timer fired. Destroying timer event.";
	std::lock_guard <mutex> lock (mTimerMutex);
	mTimer->destroy(mTimerEvent);
	mTimerEvent = nullptr;
}


void LightContext::on() {
	digitalWrite(mPin, HIGH);

	struct timeval tv = {0};
	tv.tv_sec = mConfig->getLightTimeoutSeconds();

	std::lock_guard <mutex> lock (mTimerMutex);
	if(mTimerEvent == nullptr) {
		mTimerEvent = mTimer->create(&tv, LightContext::_onTimerEvent, this);
		LOG(INFO) << "Timer created.";
	} else {
		LOG(INFO) << "Timer already exists. Will not recreate.";
	}
}

void LightContext::off() {
	digitalWrite(mPin, LOW);
}

}

// using PC::Config;
// using PC::LightContext;

// static Config *config = nullptr;
// static LightContext *lightContext = nullptr;

// int main() {

// 	config = new Config();
// 	config->init();

// 	lightContext = new LightContext(config);
// 	lightContext->on();
// 	lightContext->on();

// 	THREAD_SLEEP_5S;
// 	THREAD_SLEEP_1000MS;

// 	lightContext->on();

// 	THREAD_SLEEP_FOREVER;

// 	return 0;
// }
