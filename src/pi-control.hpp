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
 * \file   pi-control.hpp
 *
 * \author Sandeep Prakash
 *
 * \date   April 11, 2020
 *
 * \brief
 *
 ******************************************************************************/

#include <vector>
#include <string>
#include <ch-cpp-utils/semaphore.hpp>
#include <ch-cpp-utils/http/server/http.hpp>
#include <ch-cpp-utils/timer.hpp>
#include <ch-cpp-utils/utils.hpp>
#include <ch-cpp-utils/fts.hpp>
#include <ch-cpp-utils/fs-watch.hpp>
#include <ch-cpp-utils/proc-stat.hpp>

#include "config.hpp"
#include "light-context.hpp"

#ifndef SRC_PI_CONTROL_HPP_
#define SRC_PI_CONTROL_HPP_

using namespace std::chrono;

using ChCppUtils::Semaphore;
using ChCppUtils::Http::Server::RequestEvent;
using ChCppUtils::Http::Server::HttpHeaders;
using ChCppUtils::Http::Server::HttpQuery;
using ChCppUtils::Http::Server::HttpServer;
using ChCppUtils::Timer;
using ChCppUtils::TimerEvent;
using ChCppUtils::FsWatch;
using ChCppUtils::OnFileData;
using ChCppUtils::ProcStat;

namespace PC {

class PiControl {
private:
	Semaphore mExitSem;

	Config *mConfig;

	HttpServer *mServer;

	ProcStat *procStat;

	LightContext *mLightContext;

	static void _onRequestLightOn(RequestEvent *event, void *this_);
	void onRequestLightOn(RequestEvent *event);

	static void _onRequestLightOff(RequestEvent *event, void *this_);
	void onRequestLightOff(RequestEvent *event);


public:
	PiControl(Config *config, LightContext *lightContext);
	~PiControl();

	void start();
	void stop();
};

}

#endif

