add_library(spi_flash
	spi_flash/W25Q16JV.cpp
)

target_link_libraries(spi_flash
	stm32h7xx_hal
	stm32h7xx_ll

	common_util
)

target_include_directories(spi_flash
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/spi_flash
)

if(DEFINED Doxygen::doxygen)
	doxygen_add_docs(spi_flash_docs
		spi_flash/
	)
endif()

if(TARGET spiffs)
	add_library(spiffs_util
		spiffs_util/spiffs_int.cpp
	)

	target_link_libraries(spiffs_util
		spiffs
		freertos_cpp_util
	)

	if(DEFINED Doxygen::doxygen)
		doxygen_add_docs(spiffs_util_docs
			spiffs_util/
		)
	endif()

	target_include_directories(spiffs_util
		PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/spiffs_util
	)

	add_library(spiffs_flash
		spiffs_flash/spiffs_int_w25q16jv.cpp
	)

	target_link_libraries(spiffs_flash
		spiffs_util
		spi_flash
	)

	target_include_directories(spiffs_flash
		PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/spiffs_flash
	)

	if(DEFINED Doxygen::doxygen)
		doxygen_add_docs(spiffs_flash_docs
			spiffs_flash/
		)
	endif()
endif()

if(TARGET lfs)
	add_library(lfs_util
		lfs_util/lfs_int.cpp
	)

	target_link_libraries(lfs_util
		lfs
		freertos_cpp_util
	)

	target_include_directories(lfs_util
		PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/lfs_util
	)

	add_library(lfs_flash
		lfs_flash/lfs_int_w25q16jv.cpp
	)

	target_link_libraries(lfs_flash
		lfs_util
		spi_flash
	)

	target_include_directories(lfs_flash
		PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/lfs_flash
	)
endif()