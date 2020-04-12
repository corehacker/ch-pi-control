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
 * \file   pi-control.cpp
 *
 * \author Sandeep Prakash
 *
 * \date   April 11, 2020
 *
 * \brief
 *
 ******************************************************************************/

#include "pi-control.hpp"

using ChCppUtils::send200OK;

namespace PC {

PiControl::PiControl(Config *config,
                     LightContext *lightContext) {
	mConfig = config;

	mLightContext = lightContext;

	mServer = new HttpServer(mConfig->getPort(), 2);

	procStat = new ProcStat();
}

PiControl::~PiControl() {
	delete procStat;

	delete mServer;
}

void PiControl::_onRequestLightOn(RequestEvent *event, void *this_) {
	PiControl *piControl = (PiControl *) this_;
	piControl->onRequestLightOn(event);
}

void PiControl::onRequestLightOn(RequestEvent *event) {
	LOG(INFO) << "onRequestLightOn";
	mLightContext->on();
	send200OK(event->getRequest()->getRequest());
}

void PiControl::_onRequestLightOff(RequestEvent *event, void *this_) {
	PiControl *piControl = (PiControl *) this_;
	piControl->onRequestLightOff(event);
}

void PiControl::onRequestLightOff(RequestEvent *event) {
	LOG(INFO) << "onRequestLightOff";
	mLightContext->off();
	send200OK(event->getRequest()->getRequest());
}

void PiControl::start() {
	mServer->route(EVHTTP_REQ_POST, mConfig->getLightOnRoute(), PiControl::_onRequestLightOn, this);
	mServer->route(EVHTTP_REQ_POST, mConfig->getLightOffRoute(), PiControl::_onRequestLightOff, this);
}

void PiControl::stop() {
	mExitSem.notify();
	LOG(INFO) << "Stopping server";
}

}
