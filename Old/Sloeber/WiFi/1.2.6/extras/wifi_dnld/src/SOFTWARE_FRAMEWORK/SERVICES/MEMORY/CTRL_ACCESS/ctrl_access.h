/* This header file is part of the ATMEL AVR-UC3-SoftwareFramework-1.7.0 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Abstraction layer for memory interfaces.
 *
 * This module contains the interfaces:
 *   - MEM <-> USB;
 *   - MEM <-> RAM;
 *   - MEM <-> MEM.
 *
 * This module may be configured and expanded to support the following features:
 *   - write-protected globals;
 *   - password-protected data;
 *   - specific features;
 *   - etc.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 */

#ifndef _CTRL_ACCESS_H_
#define _CTRL_ACCESS_H_

#include "compiler.h"
#include "conf_access.h"


//! Status returned by CTRL_ACCESS interfaces.
typedef enum
{
  CTRL_GOOD       = PASS,     //!< Success, memory ready.
  CTRL_FAIL       = FAIL,     //!< An error occurred.
  CTRL_NO_PRESENT = FAIL + 1, //!< Memory unplugged.
  CTRL_BUSY       = FAIL + 2  //!< Memory not initialized or changed.
} Ctrl_status;


// FYI: Each Logical Unit Number (LUN) corresponds to a memory.

// Check LUN defines.
#ifndef LUN_0
  #error LUN_0 must be defined as ENABLE or DISABLE in conf_access.h
#endif
#ifndef LUN_1
  #error LUN_1 must be defined as ENABLE or DISABLE in conf_access.h
#endif
#ifndef LUN_2
  #error LUN_2 must be defined as ENABLE or DISABLE in conf_access.h
#endif
#ifndef LUN_3
  #error LUN_3 must be defined as ENABLE or DISABLE in conf_access.h
#endif
#ifndef LUN_4
  #error LUN_4 must be defined as ENABLE or DISABLE in conf_access.h
#endif
#ifndef LUN_5
  #error LUN_5 must be defined as ENABLE or DISABLE in conf_access.h
#endif
#ifndef LUN_6
  #error LUN_6 must be defined as ENABLE or DISABLE in conf_access.h
#endif
#ifndef LUN_7
  #error LUN_7 must be defined as ENABLE or DISABLE in conf_access.h
#endif
#ifndef LUN_USB
  #error LUN_USB must be defined as ENABLE or DISABLE in conf_access.h
#endif

/*! \name LUN IDs
 */
//! @{
#define LUN_ID_0        (0)                 //!< First static LUN.
#define LUN_ID_1        (LUN_ID_0 + LUN_0)
#define LUN_ID_2        (LUN_ID_1 + LUN_1)
#define LUN_ID_3        (LUN_ID_2 + LUN_2)
#define LUN_ID_4        (LUN_ID_3 + LUN_3)
#define LUN_ID_5        (LUN_ID_4 + LUN_4)
#define LUN_ID_6        (LUN_ID_5 + LUN_5)
#define LUN_ID_7        (LUN_ID_6 + LUN_6)
#define MAX_LUN         (LUN_ID_7 + LUN_7)  //!< Number of static LUNs.
#define LUN_ID_USB      (MAX_LUN)           //!< First dynamic LUN (USB host mass storage).
//! @}


// Include LUN header files.
#if LUN_0 == ENABLE
  #include LUN_0_INCLUDE
#endif
#if LUN_1 == ENABLE
  #include LUN_1_INCLUDE
#endif
#if LUN_2 == ENABLE
  #include LUN_2_INCLUDE
#endif
#if LUN_3 == ENABLE
  #include LUN_3_INCLUDE
#endif
#if LUN_4 == ENABLE
  #include LUN_4_INCLUDE
#endif
#if LUN_5 == ENABLE
  #include LUN_5_INCLUDE
#endif
#if LUN_6 == ENABLE
  #include LUN_6_INCLUDE
#endif
#if LUN_7 == ENABLE
  #include LUN_7_INCLUDE
#endif
#if LUN_USB == ENABLE
  #include LUN_USB_INCLUDE
#endif


// Check the configuration of write protection in conf_access.h.
#ifndef GLOBAL_WR_PROTECT
  #error GLOBAL_WR_PROTECT must be defined as ENABLED or DISABLED in conf_access.h
#endif


#if GLOBAL_WR_PROTECT == ENABLED

//! Write protect.
extern Bool g_wr_protect;

#endif


/*! \name Control Interface
 */
//! @{

#ifdef FREERTOS_USED

/*! \brief Initializes the LUN access locker.
 *
 * \return \c TRUE if the locker was successfully initialized, else \c FALSE.
 */
extern Bool ctrl_access_init(void);

#endif  // FREERTOS_USED

/*! \brief Returns the number of LUNs.
 *
 * \return Number of LUNs in the system.
 */
extern U8 get_nb_lun(void);

/*! \brief Returns the current LUN.
 *
 * \return Current LUN.
 *
 * \todo Implement.
 */
extern U8 get_cur_lun(void);

/*! \brief Tests the memory state and initializes the memory if required.
 *
 * The TEST UNIT READY SCSI primary command allows an application client to poll
 * a LUN until it is ready without having to allocate memory for returned data.
 *
 * This command may be used to check the media status of LUNs with removable
 * media.
 *
 * \param lun Logical Unit Number.
 *
 * \return Status.
 */
extern Ctrl_status mem_test_unit_ready(U8 lun);

/*! \brief Returns the address of the last valid sector (512 bytes) in the
 *         memory.
 *
 * \param lun           Logical Unit Number.
 * \param u32_nb_sector Pointer to the address of the last valid sector.
 *
 * \return Status.
 */
extern Ctrl_status mem_read_capacity(U8 lun, U32 *u32_nb_sector);

/*! \brief Returns the size of the physical sector.
 *
 * \param lun Logical Unit Number.
 *
 * \return Sector size (unit: 512 bytes).
 */
extern U8 mem_sector_size(U8 lun);

/*! \brief Returns the write-protection state of the memory.
 *
 * \param lun Logical Unit Number.
 *
 * \return \c TRUE if the memory is write-protected, else \c FALSE.
 *
 * \note Only used by removable memories with hardware-specific write
 *       protection.
 */
extern Bool mem_wr_protect(U8 lun);

/*! \brief Tells whether the memory is removable.
 *
 * \param lun Logical Unit Number.
 *
 * \return \c TRUE if the memory is removable, else \c FALSE.
 */
extern Bool mem_removal(U8 lun);

/*! \brief Returns a pointer to the LUN name.
 *
 * \param lun Logical Unit Number.
 *
 * \return Pointer to the LUN name string.
 */
extern const char *mem_name(U8 lun);

//! @}


#if ACCESS_USB == ENABLED

/*! \name MEM <-> USB Interface
 */
//! @{

/*! \brief Tranfers data from the memory to USB.
 *
 * \param lun       Logical Unit Number.
 * \param addr      Address of first memory sector to read.
 * \param nb_sector Number of sectors to transfer.
 *
 * \return Status.
 */
extern Ctrl_status memory_2_usb(U8 lun, U32 addr, U16 nb_sector);

/*! \brief Tranfers data from USB to the memory.
 *
 * \param lun       Logical Unit Number.
 * \param addr      Address of first memory sector to write.
 * \param nb_sector Number of sectors to transfer.
 *
 * \return Status.
 */
extern Ctrl_status usb_2_memory(U8 lun, U32 addr, U16 nb_sector);

//! @}

#endif  // ACCESS_USB == ENABLED


#if ACCESS_MEM_TO_RAM == ENABLED

/*! \name MEM <-> RAM Interface
 */
//! @{

/*! \brief Copies 1 data sector from the memory to RAM.
 *
 * \param lun   Logical Unit Number.
 * \param addr  Address of first memory sector to read.
 * \param ram   Pointer to RAM buffer to write.
 *
 * \return Status.
 */
extern Ctrl_status memory_2_ram(U8 lun, U32 addr, void *ram);

/*! \brief Copies 1 data sector from RAM to the memory.
 *
 * \param lun   Logical Unit Number.
 * \param addr  Address of first memory sector to write.
 * \param ram   Pointer to RAM buffer to read.
 *
 * \return Status.
 */
extern Ctrl_status ram_2_memory(U8 lun, U32 addr, const void *ram);

//! @}

#endif  // ACCESS_MEM_TO_RAM == ENABLED


#if ACCESS_STREAM == ENABLED

/*! \name Streaming MEM <-> MEM Interface
 */
//! @{

//! Erroneous streaming data transfer ID.
#define ID_STREAM_ERR         0xFF

  #if ACCESS_MEM_TO_MEM == ENABLED

/*! \brief Copies data from one memory to another.
 *
 * \param src_lun   Source Logical Unit Number.
 * \param src_addr  Source address of first memory sector to read.
 * \param dest_lun  Destination Logical Unit Number.
 * \param dest_addr Destination address of first memory sector to write.
 * \param nb_sector Number of sectors to copy.
 *
 * \return Status.
 */
extern Ctrl_status stream_mem_to_mem(U8 src_lun, U32 src_addr, U8 dest_lun, U32 dest_addr, U16 nb_sector);

  #endif  // ACCESS_MEM_TO_MEM == ENABLED

/*! \brief Returns the state of a streaming data transfer.
 *
 * \param id  Transfer ID.
 *
 * \return Status.
 *
 * \todo Implement.
 */
extern Ctrl_status stream_state(U8 id);

/*! \brief Stops a streaming data transfer.
 *
 * \param id  Transfer ID.
 *
 * \return Number of remaining sectors.
 *
 * \todo Implement.
 */
extern U16 stream_stop(U8 id);

//! @}

#endif  // ACCESS_STREAM == ENABLED


#endif  // _CTRL_ACCESS_H_

//Added by Sloeber 
#pragma once

