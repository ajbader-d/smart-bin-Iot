/**
 ******************************************************************************
 * @file    nfc.h
 * @author  ST Central Labs
 * @version V1.0.0
 * @date    14-October-2015
 * @brief   This header file contains the functions prototypes for the
 *          NFC device driver.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
#ifndef __NFC_H_
#define __NFC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "component.h"
/** @addtogroup BSP
 * @{
 */

/** @addtogroup Components
 * @{
 */

/** @addtogroup NFC
 * @{
 */

/** @defgroup NFC_Exported_Types
 * @{
 */

/**
 * @brief  NFC status enumerator definition
 */
typedef enum {
	NFC_SUCCESS=0x9000,
	NFC_ERROR=0x6F00,
	NFC_FILE_OVERFLOW_LE = 0x6280,
	NFC_EOF = 0x6282,
	NFC_PASSWORD_REQUIRED = 0x6300,
	NFC_PASSWORD_INCORRECT = 0x63C0,
	NFC_PASSWORD_INCORRECT1RETRY = 0x63C1,
	NFC_PASSWORD_INCORRECT2RETRY = 0x63C2,
	NFC_WRONG_LENGHT = 0x6700,
	NFC_UNSUCESSFUL_UPDATING = 0x6581,
	NFC_INCOPATIBLE_COMMAND= 0x6981,
	NFC_SECURITY_UNSATISFIED = 0x6982,
	NFC_REFERENCE_DATA_NOT_USABLE = 0x6984,

	NFC_INCORRECT_PARAMETER = 0x6a80,
	NFC_FILE_NOT_FOUND=0x6a82,
	NFC_FILE_OVERFLOW_LC = 0x6A84, //TODO difference with Le??

	NFC_INCORRECT_P1_OR_P2 = 0x6A86, //TODO better name?
	NFC_RF_SESSION_KILLED=0x6500,
	NFC_INS_NOT_SUPPORTED=0x6D00,
	NFC_CLASS_NOT_SUPPORTED=0x6E00,

	//IOError
	NFC_IO_ERROR_I2CTIMEOUT=0x0011,
	NFC_IO_ERROR_CRC=0x0012,
	NFC_IO_ERROR_NACK=0x0013,
	NFC_IO_ERROR_PARAMETER=0x0014,
	NFC_IO_ERROR_NBATEMPT=0x0015,
	NFC_IO_NOACKNOWLEDGE=0x0016
} NFC_StatusTypeDef;

/**
 * @brief  NFC component identifier enumerator definition.
 */
typedef enum
{
	NFC_NONE_COMPONENT = 0,
	NFC_M24SR_COMPONENT = 1
} NFC_ComponentTypeDef;

/**
 * @brief  GPO state structure
 */
typedef enum {
	HIGH_IMPEDANCE = 0,
	SESSION_OPENED =1,
	WIP=2,
	I2C_ANSWER_READY=3,
	INTERRUPT=4,
	STATE_CONTROL=5
} NFC_GPO_MGMT;


/**
 * @brief  NFC driver structure definition
 */
typedef struct {
	/* Generic */
	NFC_StatusTypeDef (*Init)      (void *handle, void *init);
	NFC_StatusTypeDef (*ReadID)    (void *handle, uint8_t *id);

	/* Interrupts */
	NFC_StatusTypeDef (*SendInterrupt)(void *handle);

	/* Specific */
	//I2C session Commands:
	NFC_StatusTypeDef (*GetSession)(void *handle);
	NFC_StatusTypeDef (*KillSession)(void *handle);
	NFC_StatusTypeDef (*Deselect)(void *handle);

	//NFC forum type 4 commands:
	NFC_StatusTypeDef (*SelectApplication)(void *handle);
	NFC_StatusTypeDef (*SelectCCfile)(void *handle);
	NFC_StatusTypeDef (*SelectNDEFfile)(void *handle, uint16_t NDEFfileId);
	NFC_StatusTypeDef (*SelectSystemfile)(void *handle);
	NFC_StatusTypeDef (*ReadBinary)(void *handle, uint16_t Offset, uint8_t NbByteToRead, uint8_t *pBufferRead);
	NFC_StatusTypeDef (*UpdateBinary)(void *handle, uint16_t Offset, uint8_t NbByteToWrite, uint8_t *pDataToWrite);

	//ISO/IEC 7816-4 commands:
	NFC_StatusTypeDef (*Verify)(void *handle, uint16_t uPwdId, uint8_t NbPwdByte, uint8_t *pPwd);
	NFC_StatusTypeDef (*ChangeReferenceData)(void *handle, uint16_t uPwdId, uint8_t *pPwd);
	NFC_StatusTypeDef (*EnableVerificationRequirement)(void *handle, uint16_t uReadOrWrite);
	NFC_StatusTypeDef (*DisableVerificationRequirement)(void *handle, uint16_t uReadOrWrite);

	//ST Specific
	NFC_StatusTypeDef (*STReadBinary)(void *handle, uint16_t Offset, uint8_t NbByteToRead, uint8_t *pBufferRead);
	NFC_StatusTypeDef (*EnablePermanentState)(void *handle, uint16_t uReadOrWrite);
	NFC_StatusTypeDef (*DisablePermanentState)(void *handle, uint16_t uReadOrWrite);

	//??

	NFC_StatusTypeDef (*StateControl)(void *handle, uint8_t uSetOrReset);
	NFC_StatusTypeDef (*ManageI2CGPO)(void *handle, uint8_t GPO_I2Cconfig);
	NFC_StatusTypeDef (*ManageRFGPO)(void *handle, uint8_t GPO_RFconfig);
	NFC_StatusTypeDef (*RFConfig)(void *handle, uint8_t OnOffChoice);

} NFC_DrvVTableTypeDef;

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __NFC_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
