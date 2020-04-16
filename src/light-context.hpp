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
 * \file   light-context.hpp
 *
 * \author Sandeep Prakash
 *
 * \date   April 11, 2020
 *
 * \brief
 *
 ******************************************************************************/

#include <mutex>

#include <ch-cpp-utils/timer.hpp>

#include "config.hpp"

#ifndef SRC_LIGHT_CONTEXT_HPP_
#define SRC_LIGHT_CONTEXT_HPP_

using std::mutex;

using ChCppUtils::Timer;
using ChCppUtils::TimerEvent;

namespace PC {

class LightContext {
private:
	Config *mConfig;

	Timer *mTimer;

	TimerEvent *mTimerEvent;

	mutex mTimerMutex;

	uint32_t mPin;

	static void _onTimerEvent(TimerEvent *event, void *this_);

	void onTimerEvent(TimerEvent *event);

public:
	LightContext(Config *config);
	~LightContext();

	void on();
	void off();
};

}

#endif

