/*
 * Copyright (c) 2012, 2013 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mtp_util_thread.h"

/*
 * FUNCTIONS
 */
mtp_bool _util_thread_create_with_stack(pthread_t *tid, const mtp_char *tname,
		mtp_int32 thread_state, thread_func_t thread_func, void *arg, int stacksize)
{
	int error = 0;
	pthread_attr_t attr;
	struct sched_param param;

	retv_if(tname == NULL, FALSE);
	retv_if(thread_func == NULL, FALSE);

	error = pthread_attr_init(&attr);
	if (error != 0) {
		ERR("pthread_attr_init Fail [%d], errno [%d]\n", error, errno);	//	LCOV_EXCL_LINE
		return FALSE;
	}

	if (thread_state == PTHREAD_CREATE_JOINABLE) {
		error = pthread_attr_setdetachstate(&attr,
				PTHREAD_CREATE_JOINABLE);
		if (error != 0) {
			/* LCOV_EXCL_START */
			ERR("pthread_attr_setdetachstate Fail [%d], errno [%d]\n", error, errno);
			pthread_attr_destroy(&attr);
			return FALSE;
			/* LCOV_EXCL_STOP */
		}
	}

	param.sched_priority = MTP_DEFAULT_THREAD_PRIORITY;
	pthread_attr_setschedparam(&attr, &param);

	pthread_attr_setstacksize(&attr, stacksize);
	error = pthread_create(tid, &attr, thread_func, arg);
	if (error != 0) {
		/* LCOV_EXCL_START */
		ERR("Thread creation Fail [%d], errno [%d]\n", error, errno);
		pthread_attr_destroy(&attr);
		return FALSE;
		/* LCOV_EXCL_STOP */
	}

	error = pthread_attr_destroy(&attr);
	if (error != 0)
		ERR("pthread_attr_destroy Fail [%d] errno [%d]\n", error, errno);	//	LCOV_EXCL_LINE

	return TRUE;
}

mtp_bool _util_thread_create(pthread_t *tid, const mtp_char *tname,
		mtp_int32 thread_state, thread_func_t thread_func, void *arg)
{
        return _util_thread_create_with_stack(tid, tname, thread_state,
                                              thread_func, arg, MTP_DEFAULT_STACK_SIZE);
}

mtp_bool _util_thread_join(pthread_t tid, void **data)
{
	mtp_int32 res = 0;

	res = pthread_join(tid, data);
	if (res != 0) {
		ERR("pthread_join Fail res = [%d] for thread [%lu] errno [%d]\n",
				res, (unsigned long)tid, errno);	//	LCOV_EXCL_LINE
		return FALSE;	//	LCOV_EXCL_LINE
	}

	return TRUE;
}

mtp_bool _util_thread_cancel(pthread_t tid)
{
	mtp_int32 res;

	if (tid == 0) {
		ERR("tid is NULL\n");
		return FALSE;
	}

	res = pthread_cancel(tid);
	if (res != 0) {
		ERR("pthread_cancel Fail [%lu] errno [%d]\n", (unsigned long)tid, res);
		return FALSE;
	}

	return TRUE;
}

/* LCOV_EXCL_START */
void _util_thread_exit(void *val_ptr)
{
	pthread_exit(val_ptr);
	return;
}
/* LCOV_EXCL_STOP */
