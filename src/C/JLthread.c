/*
 * JL_Lib
 * Copyright (c) 2015 Jeron A. Lau 
*/
/** \file
 * JLthread.c
 *	This file handles a separate thread for drawing graphics.
**/
#include "jl_pr.h"

//
// Static Functions
//

// Initialize a new thread.
static void jl_thread_init_new(jl_t* jl, u8_t thread_id) {
	jl_print_init_thread__(jl, thread_id);
}

//
// Exported Functions
//

/**
 * Create a thread.  User can program up to 16 threads.
 * @param jl: The library context.
 * @param name: The name of the thread.
 * @param fn: The main function of the thread.
 * @returns: The thread ID number.
**/
uint8_t jl_thread_new(jl_t *jl, str_t name, SDL_ThreadFunction fn) {
	int8_t i, rtn = -1;

	// Skip main thread ( i = 1 )
	for(i = 1; i < 16; i++) {
		// Look for not init'd thread.
		if(jl->jl_ctx[i].thread == NULL) {
			// Run thread-specific initalizations
			jl_thread_init_new(jl, i);
			// Create a thread
			jl->jl_ctx[i].thread =	SDL_CreateThread(fn, name, jl);
			jl->jl_ctx[i].thread_id =
				SDL_GetThreadID(jl->jl_ctx[i].thread);
			// Check if success
			if(jl->jl_ctx[i].thread == NULL) {
				jl_print(jl, "SDL_CreateThread failed: %s",
					SDL_GetError());
				exit(-1);
			}
			rtn = i;
			break;
		}
	}
	if(rtn == -1) {
		jl_print(jl, "More than 16 threads!");
		exit(-1);
	}
	JL_PRINT_DEBUG(jl, "Made thread #%d", rtn);
	return rtn;
}

/**
 * Return the ID of the current thread.
 * @param jl: The library context.
 * @returns: The thread ID number, 0 if main thread.
**/
uint8_t jl_thread_current(jl_t *jl) {
	SDL_threadID current_thread = SDL_ThreadID();
	uint8_t i, rtn = 0;

	// Skip main thread ( i = 1 )
	for(i = 1; i < 16; i++) {
		// Look for not init'd thread.
		if(jl->jl_ctx[i].thread_id == current_thread) {
			rtn = i;
			break;
		}
	}
	return rtn;
}

/**
 * Wait for a thread to exit.
 * @param jl: The library context.
 * @param threadnum: The thread id returned from jl_thread_new().
 * @returns: Value returned from the thread.
**/
int32_t jl_thread_old(jl_t *jl, u8_t threadnum) {
	int32_t threadReturnValue = 0;

	SDL_WaitThread(jl->jl_ctx[threadnum].thread, &threadReturnValue);
	return threadReturnValue;
}

/**
 * Create a mutex ( lock for a thread's access to data )
 * @param jl: The library context.
 * @returns: An SDL mutex
**/
SDL_mutex* jl_thread_mutex_new(jl_t *jl) {
	SDL_mutex* mutex = SDL_CreateMutex();
	if (!mutex) {
		jl_print(jl, "jl_thread_mutex_new: Couldn't create mutex");
		exit(-1);
	}
	return mutex;
}

/**
 * Lock a mutex.
 * @param jl: The library context.
 * @param mutex: The mutex created by jl_thread_mutex_new().
**/
void jl_thread_mutex_lock(jl_t *jl, SDL_mutex* mutex) {
	if (SDL_LockMutex(mutex) != 0) {
		jl_print(jl, "jl_thread_mutex_use: Couldn't lock mutex");
		exit(-1);
	}
}

/**
 * Unlock a mutex.
 * @param jl: The library context.
 * @param mutex: The mutex created by jl_thread_mutex_new().
**/
void jl_thread_mutex_unlock(jl_t *jl, SDL_mutex* mutex) {
	SDL_UnlockMutex(mutex);
}

/**
 * Run a function that uses the mutex.
 * @param jl: The library context.
 * @param mutex: The mutex created by jl_thread_mutex_new().
 * @param fn_: The function that uses data locked by the mutex.
**/
void jl_thread_mutex_use(jl_t *jl, SDL_mutex* mutex, jl_fnct fn_) {
	jl_thread_mutex_lock(jl, mutex);
	// Do stuff while mutex is locked
	fn_(jl);
	// Give up for other threads
	jl_thread_mutex_unlock(jl, mutex);
}

/**
 * Do a thread-safe copy of data from "src" to "dst".  This function will wait
 *	until no other threads are using the mutex before doing anything.
 * @param jl: The library context.
 * @param mutex: The mutex protecting the variable, made by
 *	jl_thread_mutex_new().
 * @param src: The mutex-protected variable.
 * @param dst: The non-protected variable.
 * @param size: The size of the data pointed to by "src" and "dst" ( must be the
 *	same)
**/
void jl_thread_mutex_cpy(jl_t *jl, SDL_mutex* mutex, void* src, void* dst,
	u32_t size)
{
	if (SDL_LockMutex(mutex) == 0) {
		// Copy data.
		jl_mem_copyto(src, dst, size);
		// Give up for other threads
		SDL_UnlockMutex(mutex);
	} else {
		jl_print(jl, "jl_thread_mutex_use: Couldn't lock mutex");
		exit(-1);
	}
}

/**
 * Free a mutex from memory.
 * @param jl: The library context.
 * @param mutex: The mutex to free (created by jl_thread_mutex_new())
**/
void jl_thread_mutex_old(jl_t* jl, SDL_mutex* mutex) {
	SDL_DestroyMutex(mutex);
}

/**
 * Create a thread communicator.
 * @param jl: The library context.
 * @param size: The size of the data.
 * @returns: The thread communicator.
**/
jl_comm_t* jl_thread_comm_make(jl_t* jl, u32_t size) {
	jl_comm_t* rtn = jl_memi(jl, sizeof(jl_comm_t));
	m_u8_t i;

	rtn->lock = SDL_CreateMutex();
	rtn->size = size;
	rtn->pnum = 0;
	for(i = 0; i < 16; i++) {
		rtn->data[i] = jl_memi(jl, size);
	}
	return rtn;
}

/**
 * Send a message to another thread.  Up to 16 messages can be sent without the
 *	other thread responding.  If the 17th is trying to be sent, then the
 *	thread jl_thread_comm_send() is called on will be stalled.  It will wait
 *	until the other thread responds, and if it doesn't, the program will
 *	crash.
 * @param jl: The library context.
 * @param comm: The thread communicator.
 * @param src: The data to send, must be same size as specified in
 *	jl_thread_comm_new().
**/
void jl_thread_comm_send(jl_t* jl, jl_comm_t* comm, const void* src) {
	uint8_t stall = 0;
	SDL_LockMutex(comm->lock);
	// Copy to next packet location
	jl_mem_copyto(src, comm->data[comm->pnum], comm->size);
	// Advance number of packets.
	comm->pnum++;
	// If maxed out on packets, then stall.
	if(comm->pnum == 16) {
		JL_PRINT_DEBUG(jl,"WARNING: \"jl_thread_comm_send\" Stalling....");
		stall = 1;
	}
	SDL_UnlockMutex(comm->lock);
	// If 1 second passed, and still stalling, then quit.
	while(stall) {
		if(stall > 10) {
			jl_print(jl, "Other thread wouldn't respond!");
			exit(-1);
		}
		SDL_LockMutex(comm->lock);
		if(comm->pnum != 16) break;
		SDL_UnlockMutex(comm->lock);
		SDL_Delay(100);
		stall++;
	}
}

/**
 * Process all of the packets in a thread communicator, which are sent from
 *	another thread.
 * @param jl: The library context.
 * @param comm: The thread communicator.
 * @param fn: The function that processes each packet ( parameters: jl_t*,
 *	void*, returns void).
**/
void jl_thread_comm_recv(jl_t* jl, jl_comm_t* comm, jl_data_fnct fn) {
	SDL_LockMutex(comm->lock);
	while(comm->pnum != 0) {
		fn(jl, comm->data[comm->pnum - 1]);
		comm->pnum--;
	}
        SDL_UnlockMutex(comm->lock);
}

/**
 * Free a thread communicator.
 * @param jl: The library context.
 * @param comm: The thread communicator.
**/
void jl_thread_comm_kill(jl_t* jl, jl_comm_t* comm) {
	m_u8_t i;

	// Free the lock.
	SDL_DestroyMutex(comm->lock);
	// Free 16 packets.
	for(i = 0; i < 16; i++) jl_mem(jl, comm->data[i], 0);
	// Free main data structure.
	jl_mem(jl, comm, 0);
}

//
// Internal functions
//

void jl_thread_init__(jl_t* jl) {
	uint8_t i;

	// Set all threads to null
	for(i = 0; i < 16; i++) jl->jl_ctx[i].thread = NULL;
}
