/**
 * @author Jacob Schloss <jacob@schloss.io>
 * @copyright Copyright (c) 2019 Jacob Schloss. All rights reserved.
 * @license Licensed under the 3-Clause BSD license. See LICENSE for details
*/

#pragma once

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_qspi.h"

class W25Q16JV
{
public:
	static constexpr uint8_t MFG_ID = 0xEF;
	static constexpr uint16_t JEDEC_ID = 0x4015;

	static constexpr size_t PAGE_LEN = 256;

	static constexpr size_t SECTOR_LEN = 4*1024;

	static constexpr size_t BLOCK32_LEN = 32*1024;
	static constexpr size_t BLOCK64_LEN = 64*1024;

	static constexpr size_t PAGE_COUNT   = 8192;
	static constexpr size_t SECTOR_COUNT = 512;
	static constexpr size_t BLOCK32_COUNT = 64;
	static constexpr size_t BLOCK64_COUNT = 32;

	static constexpr size_t RESET_ASSERT_US = 1;
	static constexpr size_t RESET_DELAY_US = 30;

	static constexpr size_t STATUS_PRGM_TIME_TYP_MS = 10;
	static constexpr size_t STATUS_PRGM_TIME_MAX_MS = 15;

	static constexpr size_t PAGE_PRGM_TIME_TYP_US = 400;
	static constexpr size_t PAGE_PRGM_TIME_MAX_US = 3000;
	static constexpr size_t PAGE_PRGM_TIME_MAX_MS = 3+1;

	static constexpr size_t SECTOR_ERASE_TIME_TYP_MS = 45+1;
	static constexpr size_t SECTOR_ERASE_TIME_MAX_MS = 400+1;

	static constexpr size_t BLOCK1_ERASE_TIME_TYP_MS = 120+1;
	static constexpr size_t BLOCK1_ERASE_TIME_MAX_MS = 1600+1;

	static constexpr size_t BLOCK2_ERASE_TIME_TYP_MS = 150+1;
	static constexpr size_t BLOCK2_ERASE_TIME_MAX_MS = 2000+1;

	static constexpr size_t CHIP_ERASE_TIME_TYP_MS = 5000+1;
	static constexpr size_t CHIP_ERASE_TIME_MAX_MS = 25000+1;

	static constexpr uint32_t SECURITY_REG_1_ADDR = 0x001000;
	static constexpr uint32_t SECURITY_REG_2_ADDR = 0x002000;
	static constexpr uint32_t SECURITY_REG_3_ADDR = 0x003000;

	static constexpr uint32_t COMMAND_DELAY_MS = 10;
	static constexpr uint32_t READ_DELAY_MS = 5000;
	static constexpr uint32_t WRITE_DELAY_MS = 5000;

	class STATUS_REG_1
	{
	public:
		bool get_SRP()  const;//b7
		bool get_SEC()  const;//b6
		bool get_TB()   const;//b5
		bool get_BP2()  const;//b4
		bool get_BP1()  const;//b3
		bool get_BP0()  const;//b2
		bool get_WEL()  const;//b1
		bool get_BUSY() const;//b0
	
		uint8_t reg;
	};

	class STATUS_REG_2
	{
	public:
		bool get_SUS()  const;//b7
		bool get_CMP()  const;//b6
		bool get_LB3()  const;//b5
		bool get_LB2()  const;//b4
		bool get_LB1()  const;//b3
		bool get_RES2() const;//b2
		bool get_QE()   const;//b1
		bool get_SRL()  const;//b0
	
		uint8_t reg;
	};

	class STATUS_REG_3
	{
	public:
		bool get_RES7() const;//b7
		bool get_DRV1() const;//b6
		bool get_DRV2() const;//b5
		bool get_RES4() const;//b4
		bool get_RES3() const;//b3
		bool get_WPS()  const;//b2
		bool get_RES1() const;//b1
		bool get_RES0() const;//b0
	
		uint8_t reg;
	};

	//SPI CMD
	enum class STD_CMD : uint8_t
	{
		WRITE_ENABLE  = 0x06,
		WRITE_ENABLE_VSR  = 0x50,
		WRITE_DISABLE = 0x04,

		RELEASE_POWER_DOWN = 0xAB,
		MFG_DEV_ID = 0x90,
		JEDEC_ID = 0x9F,
		UNIQUE_ID = 0x4B,

		READ_DATA = 0x03,//MAX clk fR = 50 MHz
		FAST_READ = 0x0B,//MAX clk FR = 104 MHz in [2V7, 3V0], 133 MHz in [3V0, 3V6]

		PAGE_PRGM = 0x02,

		SECTOR_ERASE = 0x20,
		BLOCK_32K_ERASE = 0x52,
		BLOCK_64K_ERASE = 0xD8,
		CHIP_ERASE = 0xC7,

		READ_STATUS_1  = 0x05,
		WRITE_STATUS_1 = 0x01,
		READ_STATUS_2  = 0x35,
		WRITE_STATUS_2 = 0x31,
		READ_STATUS_3  = 0x15,
		WRITE_STATUS_3 = 0x11,

		POWER_DOWN = 0xB9,

		ENABLE_RESET = 0x66,
		RESET_DEVICE = 0x99
	};

	//SPI CMD
	enum class DUAL_CMD : uint8_t
	{
		FAST_READ_DUAL_OUT = 0x3B,
		FAST_READ_DUAL_IO  = 0xBB,

		MFG_DEV_ID_DUAL = 0x92
	};

	//SPI CMD
	enum class QUAD_CMD : uint8_t
	{
		PAGE_PRGM_QUAD_IN = 0x32,

		FAST_READ_QUAD_OUT = 0x6B,
		FAST_READ_QUAD_IO = 0xEB,

		MFG_DEV_ID_QUAD = 0x94
	};

	W25Q16JV()
	{
		m_qspi_handle = nullptr;
	}

	bool init();

	void set_handle(QSPI_HandleTypeDef* const qspi_handle)
	{
		m_qspi_handle = qspi_handle;
	}

	bool get_mfg_dev_id(uint8_t* const out_mfg_id, uint8_t* const out_dev_id);
	bool get_mfg_dev_id_dual(uint8_t* const out_mfg_id, uint8_t* const out_dev_id);
	bool get_mfg_dev_id_quad(uint8_t* const out_mfg_id, uint8_t* const out_dev_id);

	bool get_jdec_id(uint8_t* const out_mfg_id, uint16_t* const out_part_id);
	bool get_unique_id(uint64_t* const out_unique_id);

	bool cmd_chip_erase();
	bool cmd_sector_erase(const uint32_t addr);
	bool cmd_block32_erase(const uint32_t addr);
	bool cmd_block64_erase(const uint32_t addr);

	bool cmd_enable_quad_mode();

	bool get_status_1(STATUS_REG_1* const reg);
	bool get_status_2(STATUS_REG_2* const reg);
	bool get_status_3(STATUS_REG_3* const reg);

	bool read(const uint32_t addr, const size_t len, uint8_t* const out_data);
	bool read2(const uint32_t addr, const size_t len, uint8_t* const out_data);
	bool read4(const uint32_t addr, const size_t len, uint8_t* const out_data);
	bool write(const uint32_t addr, const size_t len, const uint8_t* data);
	bool write4(const uint32_t addr, const size_t len, const uint8_t* data);
	bool write_page(const uint32_t addr, const size_t len, const uint8_t* data);
	bool write_page4(const uint32_t addr, const size_t len, const uint8_t* data);

	bool poll_until_busy_clear(const uint32_t timeout)
	{
		QSPI_CommandTypeDef cmd = get_read_status_reg1_cmd();
		QSPI_AutoPollingTypeDef config;
		config.Match = 0x00;
		config.Mask  = 0x01;
		config.MatchMode  = QSPI_MATCH_MODE_AND;
		config.StatusBytesSize  = 1;
		config.Interval  = 0x10;
		config.AutomaticStop  = QSPI_AUTOMATIC_STOP_ENABLE;

		//HAL_StatusTypeDef ret = HAL_QSPI_AutoPolling_IT(&QSPIHandle, &cmd, &config);
		HAL_StatusTypeDef ret = HAL_QSPI_AutoPolling(m_qspi_handle, &cmd, &config, timeout);
		if(ret != HAL_OK)
  		{
    		return false;
  		}

  		return true;
	}

	static QSPI_CommandTypeDef get_write_enable_cmd();
	static QSPI_CommandTypeDef get_volatile_write_enable_cmd();
	static QSPI_CommandTypeDef get_write_disable_cmd();
	static QSPI_CommandTypeDef get_read_status_reg1_cmd();
	static QSPI_CommandTypeDef get_write_status_reg1_cmd();
	static QSPI_CommandTypeDef get_read_status_reg2_cmd();
	static QSPI_CommandTypeDef get_write_status_reg2_cmd();
	static QSPI_CommandTypeDef get_read_status_reg3_cmd();
	static QSPI_CommandTypeDef get_write_status_reg3_cmd();
	static QSPI_CommandTypeDef get_read_data_cmd(const uint32_t addr, const size_t len);
	static QSPI_CommandTypeDef get_read2_data_cmd(const uint32_t addr, const size_t len);
	static QSPI_CommandTypeDef get_read4_data_cmd(const uint32_t addr, const size_t len);
	static bool get_page_prgm_cmd(const uint32_t addr, const size_t len, QSPI_CommandTypeDef* const cmd_cfg);
	static bool get_quad_page_prgm_cmd(const uint32_t addr, const size_t len, QSPI_CommandTypeDef* const cmd_cfg);
	static bool get_sector_erase_cmd(const uint32_t sector_num, QSPI_CommandTypeDef* const cmd_cfg);
	static bool get_block32_erase_cmd(const uint32_t addr, QSPI_CommandTypeDef* const cmd_cfg);
	static bool get_block64_erase_cmd(const uint32_t addr, QSPI_CommandTypeDef* const cmd_cfg);
	static QSPI_CommandTypeDef get_chip_erase_cmd();
	static QSPI_CommandTypeDef get_power_down_cmd();
	static QSPI_CommandTypeDef get_release_power_down_cmd();
	static QSPI_CommandTypeDef get_read_mfg_dev_id_cmd();
	static QSPI_CommandTypeDef get_read_mfg_dev_id_dual_cmd();
	static QSPI_CommandTypeDef get_read_mfg_dev_id_quad_cmd();
	static QSPI_CommandTypeDef get_read_jdec_id_cmd();
	static QSPI_CommandTypeDef get_unique_id_cmd();
	static QSPI_CommandTypeDef get_enable_reset_cmd();
	static QSPI_CommandTypeDef get_reset_cmd();

	void handle_StatusMatchCallback();
	void handle_TimeOutCallback();
	void handle_ErrorCallback();
	void handle_AbortCpltCallback();

protected:
	QSPI_HandleTypeDef* m_qspi_handle;
};
