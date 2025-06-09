/**
 * @author Jacob Schloss <jacob@schloss.io>
 * @copyright Copyright (c) 2019 Jacob Schloss. All rights reserved.
 * @license Licensed under the 3-Clause BSD license. See LICENSE for details
*/

#include "lfs_int.hpp"

extern "C"
{

int lfs_lock_dispatch(const struct lfs_config *c)
{
	LFS_int* const fs_int = static_cast<LFS_int*>(c->context);

	return fs_int->lock_fs();
}
int lfs_unlock_dispatch(const struct lfs_config *c)
{
	LFS_int* const fs_int = static_cast<LFS_int*>(c->context);

	return fs_int->unlock_fs();
}

int lfs_read_dispatch(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
	LFS_int* const fs_int = static_cast<LFS_int*>(c->context);

	return fs_int->read(block, off, buffer, size);
}
int lfs_write_dispatch(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
	LFS_int* const fs_int = static_cast<LFS_int*>(c->context);

	return fs_int->write(block, off, buffer, size);
}
int lfs_erase_dispatch(const struct lfs_config *c, lfs_block_t block)
{
	LFS_int* const fs_int = static_cast<LFS_int*>(c->context);

	return fs_int->erase(block);
}
int lfs_sync_dispatch(const struct lfs_config *c)
{
	LFS_int* const fs_int = static_cast<LFS_int*>(c->context);

	return fs_int->sync();
}

}

const lfs_config&  LFS_int::get_config() const
{
	return m_config;
}

void LFS_int::initialize()
{
	m_fs = {};
	m_config = (lfs_config) {
		.context = this,

		.read    = lfs_read_dispatch,
		.prog    = lfs_write_dispatch,
		.erase   = lfs_erase_dispatch,
		.sync    = lfs_sync_dispatch,
		.lock    = lfs_lock_dispatch,
		.unlock  = lfs_unlock_dispatch,

		.read_size = get_min_read_size(),
		.prog_size = get_page_size(),
		.block_size = get_block_size(),
		.block_count = get_len_bytes() / get_block_size(),
		.block_cycles = 500,
		.cache_size = get_page_size(),
		.lookahead_size = get_len_bytes() / get_block_size() / 8
	};

	m_lfs_read_buf.resize(m_config.cache_size);
	m_lfs_prog_buf.resize(m_config.cache_size);
	m_lfs_lookahead_buf.resize(m_config.lookahead_size);

    m_config.read_buffer      = m_lfs_read_buf.data();
    m_config.prog_buffer      = m_lfs_prog_buf.data();
    m_config.lookahead_buffer = m_lfs_lookahead_buf.data();
}

int LFS_int::format()
{
	if( is_mounted )
	{
		int umount_ret = unmount();

		if(umount_ret != LFS_ERR_OK)
		{
			return umount_ret;
		}
		else
		{
			is_mounted.store(false);
		}
	}

	int format_ret = lfs_format(&m_fs, &m_config);

	return format_ret;
}

int LFS_int::mount()
{
	if(is_mounted)
	{
		return LFS_ERR_OK;
	}

	int res = lfs_mount(&m_fs, &m_config);

	is_mounted.store(true);

	return res;
}

int LFS_int::unmount()
{
	int res = lfs_unmount(&m_fs);

	if(res == LFS_ERR_OK)
	{
		is_mounted.store(false);
	}

	return res;
}

int LFS_int::lock_fs()
{
	m_fs_mutex.lock();

	return LFS_ERR_OK;
}
int LFS_int::unlock_fs()
{
	m_fs_mutex.unlock();

	return LFS_ERR_OK;
}
