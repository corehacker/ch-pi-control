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
 * \file   motion-detector.hpp
 *
 * \author Sandeep Prakash
 *
 * \date   April 11, 2020
 *
 * \brief
 *
 ******************************************************************************/

#include <glog/logging.h>

#include "motion-detector.hpp"

#include <wiringPi.h>

using ChCppUtils::getEpochNano;

namespace PC {

MotionDetector::MotionDetector(Config *config,
	                           LightContext *lightContext) {
	mConfig = config;

	mLightContext = lightContext;

	mWindowNs = mConfig->getMotionDetectorWindowSeconds() * 1000 * 1000 * 1000;

	mLastMotionDetectionNs = 0;

	wiringPiSetup();

	mPin = mConfig->getLightPin();	

	pinMode(mPin, INPUT);

	mPool = new ThreadPool(1, false);
}

MotionDetector::~MotionDetector() {

}

bool MotionDetector::hasMotion() {
	if (digitalRead(mPin) == LOW) {
		LOG(INFO) << "No motion detected.";
		return false;
	} else {
		LOG(INFO) << "Motion detected.";
		return true;
	}
}

bool MotionDetector::hasWindowExpired() {
	uint64_t currentTimeNs = getEpochNano();
	uint64_t elapsedNs = currentTimeNs - mLastMotionDetectionNs;
	if(elapsedNs > mWindowNs) {
		LOG(INFO) << "Window expired.";
		return true;
	} else {
		LOG(INFO) << "Window not expired.";
		return false;
	}
}

void MotionDetector::takeAction() {
	mLightContext->on();
}

void *MotionDetector::_routine(void *arg, struct event_base *base) {
	MotionDetector *md = (MotionDetector *) arg;
	return md->routine();
}
	
void *MotionDetector::routine() {
	THREAD_SLEEP_1000MS;

	bool motion = hasMotion();
	bool hasExpired = hasWindowExpired();
	if(motion && hasExpired) {
		takeAction();
		mLastMotionDetectionNs = getEpochNano();
	}

	start();

	return nullptr;
}

void MotionDetector::start() {
	ThreadJob *job = new ThreadJob(MotionDetector::_routine, this);
	LOG(INFO) << "New motion detector job created";
	mPool->addJob(job);
}

void MotionDetector::stop() {
	
}

}
