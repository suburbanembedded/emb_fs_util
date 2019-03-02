#include "spiffs_int.hpp"

extern "C"
{

void spiffs_lock_dispatch(struct spiffs_t* fs)
{
	SPIFFS_int* const fs_int = static_cast<SPIFFS_int*>(fs->user_data);

	fs_int->lock_fs();
}
void spiffs_unlock_dispatch(struct spiffs_t* fs)
{
	SPIFFS_int* const fs_int = static_cast<SPIFFS_int*>(fs->user_data);

	fs_int->unlock_fs();
}

/* spi read call function type */
s32_t spiffs_read_dispatch(struct spiffs_t* fs, u32_t addr, u32_t size, u8_t* dst)
{
	SPIFFS_int* const fs_int = static_cast<SPIFFS_int*>(fs->user_data);

	return fs_int->read(fs, addr, size, dst);
}
/* spi write call function type */
s32_t spiffs_write_dispatch(struct spiffs_t* fs, u32_t addr, u32_t size, u8_t* src)
{
	SPIFFS_int* const fs_int = static_cast<SPIFFS_int*>(fs->user_data);

	return fs_int->write(fs, addr, size, src);
}
/* spi erase call function type */
s32_t spiffs_erase_dispatch(struct spiffs_t* fs, u32_t addr, u32_t size)
{
	SPIFFS_int* const fs_int = static_cast<SPIFFS_int*>(fs->user_data);

	return fs_int->erase(fs, addr, size);
}

}

spiffs_config SPIFFS_int::get_config()
{
	spiffs_config cfg;
	cfg.phys_size        = get_len_bytes();
	cfg.phys_addr        = get_start_bytes(); 
	cfg.phys_erase_block = get_erase_size();
	cfg.log_block_size   = get_block_size();
	cfg.log_page_size    = get_page_size();

	cfg.hal_read_f  = spiffs_read_dispatch;
	cfg.hal_write_f = spiffs_write_dispatch;
	cfg.hal_erase_f = spiffs_erase_dispatch;

	return cfg;	
}

void SPIFFS_int::initialize()
{
	memset(&m_fs, 0, sizeof(m_fs));
	m_fs.user_data = this;

	m_spiffs_work_buf.resize(get_page_size()*2, 0);
	m_spiffs_fds.resize(32*4, 0);
	m_spiffs_cache_buf.resize((get_page_size()+32)*4, 0);
}

int SPIFFS_int::format()
{
	if(SPIFFS_mounted(&m_fs) == 0)
	{
		int mount_ret = mount();
		if(mount_ret != SPIFFS_ERR_NOT_A_FS)
		{
			unmount();
		}
	}
	else
	{
		unmount();
	}

	int format_ret = SPIFFS_format(&m_fs);

	return format_ret;
}

int SPIFFS_int::mount()
{
	spiffs_config cfg = get_config();

	int res = SPIFFS_mount(
		&m_fs,
		&cfg,
		m_spiffs_work_buf.data(),
		m_spiffs_fds.data(), m_spiffs_fds.size(),
		m_spiffs_cache_buf.data(), m_spiffs_cache_buf.size(),
		0);

	return res;
}

void SPIFFS_int::unmount()
{
	SPIFFS_unmount(&m_fs);
}

void SPIFFS_int::lock_fs()
{
	m_fs_mutex.lock();
}
void SPIFFS_int::unlock_fs()
{
	m_fs_mutex.unlock();
}
