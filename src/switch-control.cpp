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
 * \file   switch-control.hpp
 *
 * \author Sandeep Prakash
 *
 * \date   April 19, 2020
 *
 * \brief
 *
 ******************************************************************************/

#include <glog/logging.h>

#include "switch-control.hpp"

#include <wiringPi.h>

using ChCppUtils::getEpochNano;

namespace PC {

SwitchControl::SwitchControl(Config *config,
	                           LightContext *lightContext) {
	mConfig = config;

	mLightContext = lightContext;

	wiringPiSetup();

	mPin = mConfig->getSwitchControlPin();

	mSwitchState = OFF;

	pinMode(mPin, INPUT);

	pullUpDnControl(mPin, PUD_UP);

	LOG(INFO) << "Switch control pin: " << mPin << ", Mode: INPUT";

	mPool = new ThreadPool(1, false);
}

SwitchControl::~SwitchControl() {

}

bool SwitchControl::hasActivity() {
	if (digitalRead(mPin) == LOW) {
		return true;
	} else {
		return false;
	}
}

void SwitchControl::takeAction() {
	switch(mSwitchState) {
		case ON:
			mLightContext->on();
			break;
		case OFF:
			mLightContext->off();
			break;
	}
}

void *SwitchControl::_routine(void *arg, struct event_base *base) {
	SwitchControl *md = (SwitchControl *) arg;
	return md->routine();
}
	
void *SwitchControl::routine() {
	THREAD_SLEEP_1000MS;

	bool activity = hasActivity();

	mSwitchState = ((mSwitchState == ON) ? OFF : ON);

	LOG(INFO) << "Activity: " << (activity ? "pressed" : "released") <<
		"State: " << ((mSwitchState == ON) ? "OFF" : "ON");
	if(activity) {
		takeAction();
	}

	start();

	return nullptr;
}

void SwitchControl::start() {
	ThreadJob *job = new ThreadJob(SwitchControl::_routine, this);
	mPool->addJob(job);
}

void SwitchControl::stop() {
	
}

}
