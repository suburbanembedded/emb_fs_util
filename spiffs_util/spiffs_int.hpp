/**
 * @author Jacob Schloss <jacob@schloss.io>
 * @copyright Copyright (c) 2019 Jacob Schloss. All rights reserved.
 * @license Licensed under the 3-Clause BSD license. See LICENSE for details
*/

#pragma once

#include "freertos_cpp_util/Mutex_static.hpp"

#include "spiffs.h"

#include <vector>

extern "C"
{
	void spiffs_lock_dispatch(struct spiffs_t* fs);
	void spiffs_unlock_dispatch(struct spiffs_t* fs);

	s32_t spiffs_read_dispatch(struct spiffs_t* fs, u32_t addr, u32_t size, u8_t* dst);
	s32_t spiffs_write_dispatch(struct spiffs_t* fs, u32_t addr, u32_t size, u8_t* src);
	s32_t spiffs_erase_dispatch(struct spiffs_t* fs, u32_t addr, u32_t size);
}

class SPIFFS_int
{
public:

	virtual ~SPIFFS_int()
	{
		
	}

	void initialize();

	spiffs_config get_config();

	int format();

	int mount();
	void unmount();

	void set_flash();

	void lock_fs();
	void unlock_fs();

	virtual s32_t read(struct spiffs_t* fs, u32_t addr, u32_t size, u8_t* dst) = 0;
	virtual s32_t write(struct spiffs_t* fs, u32_t addr, u32_t size, u8_t* src) = 0;
	virtual s32_t erase(struct spiffs_t* fs, u32_t addr, u32_t size) = 0;

	virtual size_t get_start_bytes() = 0;
	virtual size_t get_len_bytes() = 0;
	virtual size_t get_page_size() = 0;
	virtual size_t get_block_size() = 0;
	virtual size_t get_erase_size() = 0;

	spiffs* get_fs()
	{
		return &m_fs;
	}

protected:

	Mutex_static m_fs_mutex;

	spiffs m_fs;

	std::vector<uint8_t> m_spiffs_work_buf;
	std::vector<uint8_t> m_spiffs_fds;
	std::vector<uint8_t> m_spiffs_cache_buf;
};
