/*
 * Copyright (C) 2014-2019 Daniel Rossier <daniel.rossier@heig-vd.ch>
 * Copyright (C) 2017 Alexandre Malki <alexandre.malki@heig-vd.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <errno.h>
#include <heap.h>
#include <limits.h>
#include <list.h>
#include <pipe.h>
#include <vfs.h>
#include <schedule.h>
#include <string.h>

/*
 * Get the other end (extremity) based on a gfd.
 */
static int otherend(uint32_t gfd) {
	struct pipe_desc *pd = (struct pipe_desc *) vfs_get_privdata(gfd);

	if (!pd)
		return -EINVAL;

	return ((pd->gfd[0] == gfd) ? pd->gfd[1] : pd->gfd[0]);
}

/*
 * Check if the pipe is full
 */
bool pipe_full(pipe_desc_t *pd) {
	return (((pd->pos_write + 1) % PIPE_SIZE) == pd->pos_read);
}

/*
 * Check if the pipe is empty
 */
bool pipe_empty(pipe_desc_t *pd) {
	return (pd->pos_write == pd->pos_read);
}

static int pipe_read(int gfd, void *buffer, int count)
{
	int pos = 0;
	 
	/* pipe_desc_t *pd = (pipe_desc_t *) vfs_get_privdata(gfd); */

	/* Sanity checks*/
	if (!buffer || (count <= 0)) {
		set_errno(EPIPE);
		return -1;
	}


    /* To be completed ... */
    

    
	return pos; /* Effective number of read bytes */
}


static int pipe_write(int gfd, void *buffer, int count)
{
	int pos = 0;
	pipe_desc_t *pd = (pipe_desc_t *) vfs_get_privdata(gfd);

	/* Do Sanity checks */
	if (!buffer || (count <= 0)) {
		set_errno(EPIPE);
		return -1;
	}

	mutex_lock(&pd->lock);

	
    
    /* To be completed ... */
    
    
    
	/* Waking up sleeping threads */
	complete(&pd->wait_for_writer);

	mutex_unlock(&pd->lock);

	return pos; /* Effective number of written bytes */
}

/* 
 * Close the reader or writer when the refcount is equal to 0.
 */
static int pipe_close(int gfd)
{
	struct pipe_desc *pd = vfs_get_privdata(gfd);
	uint32_t opflags = vfs_get_access_mode(gfd);
	void *privdata;

	privdata = vfs_get_privdata(gfd);
	ASSERT(privdata != NULL);

	/* Check if some threads are waiting on it, and if yes and the pipe has nobody
	 * connected on one extremity, it wakes up the waiting threads.
	 */

	if (opflags == O_RDONLY)
		complete(&pd->wait_for_reader);

	if (opflags == O_WRONLY)
		complete(&pd->wait_for_writer);

	/* If there is no reader/writer anymore (i.e. gfd->refcount = 0), we can
	 * destroy the pipe.
	 */

	if (otherend(gfd) == -1) {
		free(pd->pipe_buf);
		free(pd);  /* Finally, free the main pipe descriptor */
	} else {
		if (pd->gfd[0] == gfd)
			pd->gfd[0] = -1;
		else
			pd->gfd[1] = -1;
	}

	return 0;
}

/*
 * Pipe file operations
 */
struct file_operations pipe_fops = {
		.read = pipe_read,
		.write = pipe_write,
		.close = pipe_close
};

/*
 * @brief This is the syscall interface
 *
 * @return an array of two file descriptors (in/out) to access the pipe.
 */

int do_pipe(int pipefd[2]) {

	/* Allocated two file descriptor */
	pipe_desc_t *pd = (struct pipe_desc *) memalign(sizeof(pipe_desc_t), 2);

	if (!pd) {
		printk("%s: heap overflow...\n", __func__);
		kernel_panic();
	}
	memset(pd, 0, sizeof(pipe_desc_t));

	/* Init internal structure members */

	mutex_init(&pd->lock);

	mutex_lock(&pd->lock);

	pd->pipe_buf = malloc(PIPE_SIZE);
	if (pd->pipe_buf == NULL) {
		set_errno(ENOMEM);
		return -1;
	}


	init_completion(&pd->wait_for_reader);
	init_completion(&pd->wait_for_writer);

	/* For next part use functions available in
	 * the vfs file.
	 * */

	/* Register ops in the gfd table
	 * open file descriptor for both side of
	 * the pipe 
	 * */
	pipefd[0] = vfs_open(&pipe_fops, VFS_TYPE_PIPE);
	pipefd[1] = vfs_open(&pipe_fops, VFS_TYPE_PIPE);

	/* Keep a reference to the two gfd of this pipe:
	 * set the global fd inside the pipe descriptor
	 * for both side of the pipe
	 * */
	pd->gfd[0] = vfs_get_gfd(pipefd[0]);
	pd->gfd[1] = vfs_get_gfd(pipefd[1]);

	/* Set direction of the pipes
	 * Set access mode to to both side:
	 * One side is read only, the other write only
	 * */
	vfs_set_access_mode(pd->gfd[0], O_RDONLY);
	vfs_set_access_mode(pd->gfd[1], O_WRONLY);

	/* Set private data of both global fds to
	 * pd to not loose track of the structure
	 */
	vfs_set_privdata(pd->gfd[0], pd);
	vfs_set_privdata(pd->gfd[1], pd);

	mutex_unlock(&pd->lock);

	return 0;
}
