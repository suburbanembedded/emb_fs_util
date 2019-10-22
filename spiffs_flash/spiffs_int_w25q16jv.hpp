/**
 * @author Jacob Schloss <jacob@schloss.io>
 * @copyright Copyright (c) 2019 Jacob Schloss. All rights reserved.
 * @license Licensed under the 3-Clause BSD license. See LICENSE for details
*/

#pragma once

#include "spiffs_int.hpp"
#include "W25Q16JV.hpp"

class spiffs_int_w25q16jv : public SPIFFS_int
{
public:

	spiffs_int_w25q16jv()
	{
		m_flash = nullptr;
	}

	void set_flash(W25Q16JV* const flash)
	{
		m_flash = flash;
	}

	s32_t read(struct spiffs_t* fs, u32_t addr, u32_t size, u8_t* dst) override
	{
		if(!m_flash->read4(addr, size, dst))
		{
			return -1;
		}

		return SPIFFS_OK;
	}
	s32_t write(struct spiffs_t* fs, u32_t addr, u32_t size, u8_t* src) override
	{
		if( size > get_page_size() )
		{
			return -1;
		}

		//we are alligned to page boundary
		if(!m_flash->write_page(addr, size, src))
		{
			return -1;
		}

		return SPIFFS_OK;
	}
	s32_t erase(struct spiffs_t* fs, u32_t addr, u32_t size) override
	{
		if( (addr % get_block_size()) != 0)
		{
			return -1;
		}

		if( size != get_erase_size() )
		{
			return -1;
		}

		//we are alligned to block2 boundary
		if(!m_flash->cmd_block64_erase(addr))
		{
			return -1;
		}

		return SPIFFS_OK;
	}

	size_t get_start_bytes() override
	{
		return 0;
	}
	size_t get_len_bytes() override
	{
		return 2*1024*1024;
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