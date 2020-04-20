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

#include <ch-cpp-utils/semaphore.hpp>
#include <ch-cpp-utils/thread-pool.hpp>
#include <ch-cpp-utils/utils.hpp>

#include "config.hpp"
#include "light-context.hpp"

using ChCppUtils::ThreadPool;
using ChCppUtils::ThreadJobBase;
using ChCppUtils::ThreadJob;

#ifndef SRC_SWITCH_CONTROL_HPP_
#define SRC_SWITCH_CONTROL_HPP_

namespace PC {

class SwitchControl {

private:

	Config *mConfig;

	LightContext *mLightContext;

	ThreadPool *mPool;

	uint32_t mPin;

	bool hasActivity();

	void takeAction();

	static void *_routine(void *arg, struct event_base *base);

	void *routine();


public:

	SwitchControl(Config *config,
	              LightContext *lightContext);

	~SwitchControl();

	void start();

	void stop();

};

}

#endif
