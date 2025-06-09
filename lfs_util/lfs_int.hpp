/**
 * @author Jacob Schloss <jacob@schloss.io>
 * @copyright Copyright (c) 2025 Jacob Schloss. All rights reserved.
 * @license Licensed under the 3-Clause BSD license. See LICENSE for details
*/

#pragma once

#include "freertos_cpp_util/Mutex_static.hpp"

#include "lfs.h"

#include <atomic>
#include <vector>

extern "C"
{
	int lfs_lock_dispatch(const struct lfs_config *c);
	int lfs_unlock_dispatch(const struct lfs_config *c);

    int lfs_read_dispatch(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
    int lfs_write_dispatch(const struct lfs_config *c, lfs_block_t block,lfs_off_t off, const void *buffer, lfs_size_t size);
    int lfs_erase_dispatch(const struct lfs_config *c, lfs_block_t block);
}

class LFS_int
{
public:

	LFS_int() : is_mounted(false)
	{

	}

	virtual ~LFS_int()
	{
		
	}

	void initialize();

	const lfs_config& get_config() const;

	int format();

	int mount();
	int unmount();

	void set_flash();

	int lock_fs();
	int unlock_fs();

	virtual int read(lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
	virtual int write(lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
	virtual int erase(lfs_block_t block);
	virtual int sync();

	virtual size_t get_start_bytes() = 0;
	virtual size_t get_len_bytes() = 0;
	virtual size_t get_min_read_size() = 0;
	virtual size_t get_page_size() = 0;
	virtual size_t get_block_size() = 0;
	virtual size_t get_erase_size() = 0;

	lfs_t* get_fs()
	{
		return &m_fs;
	}

protected:

	Mutex_static m_fs_mutex;

	lfs_t m_fs;
	lfs_config m_config;

	std::atomic<bool> is_mounted;

	std::vector<uint8_t> m_lfs_read_buf;
	std::vector<uint8_t> m_lfs_prog_buf;
	std::vector<uint8_t> m_lfs_lookahead_buf;
};
