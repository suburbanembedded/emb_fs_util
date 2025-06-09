/**
 * @author Jacob Schloss <jacob@schloss.io>
 * @copyright Copyright (c) 2025 Jacob Schloss. All rights reserved.
 * @license Licensed under the 3-Clause BSD license. See LICENSE for details
*/

#pragma once

#include "lfs_int.hpp"
#include "W25Q16JV.hpp"

class lfs_int_w25q16jv : public SPIFFS_int
{
public:

	lfs_int_w25q16jv()
	{
		m_flash = nullptr;
	}

	void set_flash(W25Q16JV* const flash)
	{
		m_flash = flash;
	}

	int read(lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) override
	{
		const uint32_t addr = block * get_block_size() + off;

		if(!m_flash->read4(addr, size, buffer))
		{
			return LFS_ERR_IO;
		}

		return LFS_ERR_OK;
	}
	int write(lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) override
	{
		const uint32_t addr = block * get_block_size() + off;

		if( size > get_page_size() )
		{
			return LFS_ERR_INVAL;
		}

		//we are alligned to page boundary
		if(!m_flash->write_page(addr, size, buffer))
		{
			return LFS_ERR_IO;
		}

		return LFS_ERR_OK;
	}
	int erase(lfs_block_t block) override
	{
		const uint32_t addr = block * get_block_size();

		//we are alligned to block2 boundary
		if(!m_flash->cmd_block64_erase(addr))
		{
			return LFS_ERR_IO;
		}

		return LFS_ERR_OK;
	}
	int sync() override
	{
		return LFS_ERR_OK;
	}

	size_t get_start_bytes() override
	{
		return 0;
	}
	size_t get_len_bytes() override
	{
		return 2*1024*1024;
	}

	size_t get_min_read_size() override
	{
		return 1;
	}
	size_t get_page_size() override
	{
		return m_flash->PAGE_LEN;
	}
	size_t get_block_size() override
	{
		return m_flash->BLOCK64_LEN;
	}
	size_t get_erase_size() override
	{
		return m_flash->BLOCK64_LEN;
	}

protected:
	W25Q16JV* m_flash;
};