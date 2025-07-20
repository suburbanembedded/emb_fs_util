#include "lfs_file_move.h"

struct lfs_mlist* get_prev_node(struct lfs_mlist* const head, struct lfs_mlist* const node)
{
	if( ! head )
	{
		return NULL;
	}

	struct lfs_mlist* prev = head;

	while(prev->next && (prev->next != node) )
	{
		prev = prev->next;
	}

	if(prev->next != node)
	{
		return NULL;
	}

	return prev;
}

int lfs_file_movehandle(lfs_t* const fs, lfs_file_t* const old_file_t, lfs_file_t* const new_file_t)
{
	#ifdef LFS_THREADSAFE
		int ret = fs->cfg->lock(fs->cfg);
		if(ret < 0)
		{
			return ret;
		}
	#endif

	// find prev node
	lfs_file_t* prev = (lfs_file_t*)get_prev_node(fs->mlist, (struct lfs_mlist*)old_file_t);
	if(prev == NULL)
	{
		return LFS_ERR_INVAL;
	}
	
	// insert new node in place of old node, clear old node
	*new_file_t = *old_file_t;
	prev->next = new_file_t;
	*old_file_t = (lfs_file_t) { };

	#ifdef LFS_THREADSAFE
		ret = fs->cfg->unlock(fs->cfg);
		if(ret < 0)
		{
			return ret;
		}
	#endif

	return LFS_ERR_OK;
}

