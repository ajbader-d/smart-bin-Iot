/**
 ******************************************************************************
 * @file    m24sr_class.h
 * @author  ST Central Labs
 * @version V1.0.0
 * @date    05 Nov 2015
 * @brief   This file provides a set of functions to interface with the M24SR
 *          device.
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

/* Revision ------------------------------------------------------------------*/
/*
 Based on:         X-CUBE-MEMS1/trunk/Drivers/BSP/Components/m24sr/m24sr.h
 Revision:         M24SR Driver V1.0.0
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __M24SR_CLASS_H
#define __M24SR_CLASS_H

/* Includes ------------------------------------------------------------------*/

#include "mbed.h"
#include "I2C.h"

#include "m24sr.h"
#include "Interfaces/Nfc_class.h"
#include "NDefNfcTagM24SR.h"

/* Classes -------------------------------------------------------------------*/

/**
 * Class representing a M24SR component.
 */
class M24SR: public Nfc {

public:

	/**
	 * Default password used to change the write/read permission
	 */
	static const uint8_t DEFAULT_PASSWORD[16];

	/**
	 * Default gpo status -> the gpo will remain high
	 */
	static const NFC_GPO_MGMT DEFAULT_GPO_STATUS=HIGH_IMPEDANCE;

	/*** Constructor and Destructor Methods ***/

	/**
	 * @brief Constructor.
	 * @param address I2C address of the component.
	 * @param I2C     I2C device to be used for communication.
	 * @param eventCallback Function that will be called when the gpo pin status changes.
	 * @param GPOPinName Pin used as GPIO.
	 * @param RFDISPinName Pin used to disable the RF function.
	 */
	M24SR(const uint8_t address, I2C &I2C,gpoEventCallback eventCallback, const PinName& GPOPinName,
			const PinName& RFDISPinName) :
			Nfc(),
			who_am_i(0),
			type(0),
			address(address),
			dev_I2C(I2C),
			GPOPin(GPOPinName),
			RFDisablePin(RFDISPinName),
			NDefTagUtil(*this),
			mCommunicationType(SYNC),
			mLastCommandSend(NONE),
			mGpoEventInterrupt(GPOPinName),
			mManageGPOCallback(*this),
			mReadIDCallback(*this){

		memset(uM24SRbuffer, 0, 0xFF * sizeof(int8_t));
		uDIDbyte = 0;
		RFDisablePin = 0;
		if(eventCallback!=NULL)
			mGpoEventInterrupt.fall(eventCallback);
		mGpoEventInterrupt.mode(PullUp);
		mGpoEventInterrupt.disable_irq();
	}

	/**
	 * @brief Destructor.
	 */
	virtual ~M24SR(void) {
	}

	/*** Public Component Related Methods ***/
	
	/**
	 * @brief  This function initializes the M24SR device.
	 * @param ptr Configure parameters, not used.
     * @return NFC_SUCCESS if no errors
	 */
	virtual int Init(void *ptr) {
		return (NFC_StatusTypeDef) M24SR_Init((M24SR_InitTypeDef*)ptr);
	}

	virtual int ReadID(uint8_t *id) {
		return (NFC_StatusTypeDef) M24SR_ReadID((uint8_t *) id);
	}

	/** lock the tag channel */
	virtual NFC_StatusTypeDef GetSession(void) {
		return (NFC_StatusTypeDef) M24SR_GetSession();
	}

	virtual NFC_StatusTypeDef ForceGetSession(void) {
		return (NFC_StatusTypeDef) M24SR_ForceSession();
	}

	virtual NFC_StatusTypeDef Deselect(void) {
		return (NFC_StatusTypeDef) M24SR_Deselect();
	}

	////////// tag4 command ////////////////////
	
	virtual NFC_StatusTypeDef SelectApplication(void) {
		return M24SR_SendSelectApplication();
	}

	virtual NFC_StatusTypeDef SelectCCfile(void) {
		return M24SR_SendSelectCCfile();
	}

	virtual NFC_StatusTypeDef SelectNDEFfile(uint16_t NDEFfileId) {
		return M24SR_SendSelectNDEFfile(NDEFfileId);
	}

	virtual NFC_StatusTypeDef SelectSystemfile(void) {
		return M24SR_SendSelectSystemfile();
	}

	virtual NFC_StatusTypeDef ReadBinary(uint16_t Offset, uint8_t NbByteToRead,
			uint8_t *pBufferRead) {
		return (NFC_StatusTypeDef) M24SR_SendReadBinary((uint16_t) Offset,
				(uint8_t) NbByteToRead, (uint8_t *) pBufferRead);
	}

	virtual NFC_StatusTypeDef UpdateBinary(uint16_t Offset,
			uint8_t NbByteToWrite,uint8_t *pDataToWrite) {
		return (NFC_StatusTypeDef) M24SR_SendUpdateBinary((uint16_t) Offset,
				(uint8_t) NbByteToWrite, (uint8_t *) pDataToWrite);
	}

	/////////////////// iso 7816-4 commands //////////////////////////
	
	virtual NFC_StatusTypeDef Verify(PasswordType_t pwdId, uint8_t NbPwdByte,
			const uint8_t *pPwd) {
		return (NFC_StatusTypeDef) M24SR_SendVerify(passwordTypeToConst(pwdId),
				(uint8_t) NbPwdByte, pPwd);
	}

	virtual NFC_StatusTypeDef ChangeReferenceData(PasswordType_t pwdId,
			const uint8_t *pPwd) {
		return (NFC_StatusTypeDef) M24SR_SendChangeReferenceData(passwordTypeToConst(pwdId),
				(uint8_t *) pPwd);
	}

	virtual NFC_StatusTypeDef EnableVerificationRequirement(
			PasswordType_t uReadOrWrite) {
		return (NFC_StatusTypeDef) M24SR_SendEnableVerificationRequirement(
				passwordTypeToConst(uReadOrWrite));
	}

	virtual NFC_StatusTypeDef DisableVerificationRequirement(
			PasswordType_t uReadOrWrite) {
		return (NFC_StatusTypeDef) M24SR_SendDisableVerificationRequirement(
				passwordTypeToConst(uReadOrWrite));
	}

    ////////////////////// ST proprietary //////////////////////////////////

	virtual NFC_StatusTypeDef STReadBinary(uint16_t Offset,
			uint8_t NbByteToRead, uint8_t *pBufferRead) {
		return (NFC_StatusTypeDef) M24SR_SendSTReadBinary((uint16_t) Offset,
				(uint8_t) NbByteToRead, (uint8_t *) pBufferRead);
	}

	virtual NFC_StatusTypeDef EnablePermanentState(PasswordType_t uReadOrWrite) {
		return (NFC_StatusTypeDef) M24SR_SendEnablePermanentState(
				passwordTypeToConst(uReadOrWrite));
	}

	virtual NFC_StatusTypeDef DisablePermanentState(PasswordType_t uReadOrWrite) {
		return (NFC_StatusTypeDef) M24SR_SendDisablePermanentState(
				passwordTypeToConst(uReadOrWrite));
	}

	///////////////////// chip configuration /////////////////////////////////
	virtual NFC_StatusTypeDef StateControl(uint8_t uSetOrReset) {
		return (NFC_StatusTypeDef) M24SR_StateControl((uint8_t) uSetOrReset);
	}

	virtual NFC_StatusTypeDef ManageI2CGPO(NFC_GPO_MGMT GPO_I2Cconfig) {
		return (NFC_StatusTypeDef) M24SR_ManageI2CGPO(GPO_I2Cconfig);
	}

	virtual NFC_StatusTypeDef ManageRFGPO(uint8_t GPO_RFconfig) {
		return (NFC_StatusTypeDef) M24SR_ManageRFGPO(
				(NFC_GPO_MGMT) GPO_RFconfig);
	}

	virtual NFC_StatusTypeDef RFConfig(uint8_t OnOffChoice) {
		return (NFC_StatusTypeDef) M24SR_RFConfig((uint8_t) OnOffChoice);
	}


	virtual NFC_StatusTypeDef SendInterrupt(void) {
		return (NFC_StatusTypeDef) M24SR_SendInterrupt();
	}

	virtual NFC_StatusTypeDef ManageEvent();

	/////////////////// hight level/utility function /////////////////////

	/**
	 * Get an implementation of NDefNfcTag to use the library NDefLib.
	 * @return an object of type NdefNfcTag 
	 */
	NDefLib::NDefNfcTag& getNDefTag(){
		return NDefTagUtil;
	}


protected:

	/*** Protected Component Related Methods ***/

	NFC_StatusTypeDef M24SR_Init(M24SR_InitTypeDef *);
	NFC_StatusTypeDef M24SR_ReadID(uint8_t *nfc_id);
	NFC_StatusTypeDef M24SR_GetSession(void);
	NFC_StatusTypeDef M24SR_ForceSession(void);



	NFC_StatusTypeDef M24SR_Deselect(void);
	NFC_StatusTypeDef M24SR_ReceiveDeselect(void);

	NFC_StatusTypeDef M24SR_SendSelectApplication(void);
	NFC_StatusTypeDef M24SR_ReceiveSelectApplication(void);

	NFC_StatusTypeDef M24SR_SendSelectCCfile(void);
	NFC_StatusTypeDef M24SR_ReceiveSelectCCfile(void);

	NFC_StatusTypeDef M24SR_SendSelectNDEFfile(uint16_t NDEFfileId);
	NFC_StatusTypeDef M24SR_ReceiveSelectNDEFfile();

	NFC_StatusTypeDef M24SR_SendSelectSystemfile(void);
	NFC_StatusTypeDef M24SR_ReceiveSelectSystemfile(void);

	NFC_StatusTypeDef M24SR_SendReadBinary(uint16_t Offset, uint8_t NbByteToRead,
			uint8_t *pBufferRead);
	NFC_StatusTypeDef M24SR_SendSTReadBinary(uint16_t Offset, uint8_t NbByteToRead,
				uint8_t *pBufferRead);
	NFC_StatusTypeDef M24SR_ReceiveReadBinary(void);

	NFC_StatusTypeDef M24SR_SendUpdateBinary(uint16_t Offset, uint8_t NbByteToWrite,
			uint8_t *pDataToWrite);
	NFC_StatusTypeDef M24SR_ReceiveUpdateBinary();

	NFC_StatusTypeDef M24SR_SendVerify(uint16_t uPwdId, uint8_t NbPwdByte,
			const uint8_t *pPwd);
	NFC_StatusTypeDef M24SR_ReceiveVerify();

	NFC_StatusTypeDef M24SR_SendChangeReferenceData(uint16_t uPwdId, uint8_t *pPwd);
	NFC_StatusTypeDef M24SR_ReceiveChangeReferenceData();

	NFC_StatusTypeDef M24SR_SendEnableVerificationRequirement(
			uint16_t uReadOrWrite);
	NFC_StatusTypeDef M24SR_ReceiveEnableVerificationRequirement();

	NFC_StatusTypeDef M24SR_SendDisableVerificationRequirement(
			uint16_t uReadOrWrite);
	NFC_StatusTypeDef M24SR_ReceiveDisableVerificationRequirement();

	NFC_StatusTypeDef M24SR_SendEnablePermanentState(uint16_t uReadOrWrite);
	NFC_StatusTypeDef M24SR_ReceiveEnablePermanentState();

	NFC_StatusTypeDef M24SR_SendDisablePermanentState(uint16_t uReadOrWrite);
	NFC_StatusTypeDef M24SR_ReceiveDisablePermanentState();

	NFC_StatusTypeDef M24SR_SendInterrupt(void);
	NFC_StatusTypeDef M24SR_StateControl(uint8_t uSetOrReset);

	NFC_StatusTypeDef M24SR_ManageI2CGPO(NFC_GPO_MGMT GPO_I2Cconfig);
	NFC_StatusTypeDef M24SR_ManageRFGPO(NFC_GPO_MGMT GPO_RFconfig);

	NFC_StatusTypeDef M24SR_RFConfig(uint8_t OnOffChoice);
	NFC_StatusTypeDef M24SR_SendFWTExtension(uint8_t FWTbyte);

	
	/**
	 * Send a command to the component.
	 * @param NbByte Length of the command.
	 * @param pBuffer Buffer containing the command.
	 * @return NFC_SUCCESS if no errors
	 */
	NFC_StatusTypeDef M24SR_IO_SendI2Ccommand(uint8_t NbByte, uint8_t *pBuffer);
	
	/**
	 * Read a command response.
	 * @param NbByte Number of bytes to read.
	 * @param pBuffer Buffer to store the response into.
	 * @return NFC_SUCCESS if no errors
	 */
	NFC_StatusTypeDef M24SR_IO_ReceiveI2Cresponse(uint8_t NbByte,
			uint8_t *pBuffer);

	/**
	 * Do an active polling on the I2C bus until the answer is ready.
	 * @return NFC_SUCCESS if no errors
	 */
	NFC_StatusTypeDef M24SR_IO_PollI2C(void);

	/**
	 * Read the gpo pin.
	 * @param[out] pPinState Variable to store the pin state into.
	 */
	void M24SR_IO_GPO_ReadPin(uint8_t *pPinState) {
		*pPinState= GPOPin.read();
	}

	/**
	 * Write the gpo pin.
	 * @param pPinState Pin state to write.
	 */
	void M24SR_IO_RFDIS_WritePin(uint8_t PinState) {
		if (PinState == 0)
			RFDisablePin = 0;
		else
			RFDisablePin = 1;
	}

	/*** Component's Instance Variables ***/

	/* Identity */
	uint8_t who_am_i;

	/* Type */
	uint8_t type;

	/* I2C address */
	uint8_t address;

	/* IO Device. */
	I2C &dev_I2C;

	/* GPIO */
	DigitalIn GPOPin;

	/**
	 * Pin used to disable the rf chip functionality.
	 */
	DigitalOut RFDisablePin;

	/**
	 * Buffer used to build the command to send to the chip.
	 */
	uint8_t uM24SRbuffer[0xFF];//max command length is 255

	/**
	 * ???
	 */
	uint8_t uDIDbyte;

	/**
	 * Object implementing the interface to use the NDefLib.
	 */
	NDefNfcTagM24SR NDefTagUtil;

	/**
	 * Command that the component can accept
	 */
	typedef enum{
		NONE,                            //!< NONE
		DESELECT,                        //!< DESELECT
		SELECT_APPLICATION,              //!< SELECT_APPLICATION
		SELECT_CC_FILE,                  //!< SELECT_CC_FILE
		SELECT_NDEF_FILE,                //!< SELECT_NDEF_FILE
		SELECT_SYSTEM_FILE,              //!< SELECT_SYSTEM_FILE
		READ,                            //!< READ
		UPDATE,                          //!< UPDATE
		VERIFY,                          //!< VERIFY
		MANAGE_I2C_GPO,                  //!< MANAGE_I2C_GPO
		MANAGE_RF_GPO,                   //!< MANAGE_RF_GPO
		CHANGE_REFERENCE_DATA,           //!< CHANGE_REFERENCE_DATA
		ENABLE_VERIFICATION_REQUIREMENT, //!< ENABLE_VERIFICATION_REQUIREMENT
		DISABLE_VERIFICATION_REQUIREMENT,//!< DISABLE_VERIFICATION_REQUIREMENT
		ENABLE_PERMANET_STATE,           //!< ENABLE_PERMANET_STATE
		DISABLE_PERMANET_STATE,          //!< DISABLE_PERMANET_STATE
	}M24SR_command_t;

	/**
 	 * User parameter used to invoke a command,
 	 * it is used to provide the data back with the response
	 */
	typedef struct{
		uint8_t *data; //!< data
		uint16_t length; //!< number of bytes in the data array
		uint16_t offset; //!< offset parameter used in the read/write command
	}M24SR_command_data_t;

	/**
	 * Communication mode used by this device
	 */
	typedef enum{
		SYNC,//!< SYNC wait the command response before returning
		ASYNC//!< ASYNC use a callback to notify the end of a command
	}M24SR_communication_t;

	/**
	 * Type of communication being used
	 */
	M24SR_communication_t mCommunicationType;

	/**
	 * Last pending command
	 */
	M24SR_command_t mLastCommandSend;

	/**
	 * Parameter used to invoke the last command
	 */
	M24SR_command_data_t mLastCommandData;

	/**
	 * Interrupt object fired when the gpo status changes
	 */
	InterruptIn mGpoEventInterrupt;
private:
	

	/**
	 * Object with the callback used to send a ManageGPO command.
	 * This class is equivalent to calling the methods:
	 * <ul>
	 *   <li> SelectedApplication </li>
	 *   <li> SelectSystemfile </li>
	 *   <li> ReadBinar: read the old value </li>
	 *   <li> onVerified </li>
	 *   <li> onUpdatedBinary: write the new value</li>
	 * </ul>
	 */
	class ManageGPOCallback : public Nfc::Callbacks{

	public:

		/**
		 * Build the chain of callbacks.
		 * @param parent Parent component to run the command on.
		 */
		ManageGPOCallback(M24SR &parent):mParent(parent),
				mGpoConfig(HIGH_IMPEDANCE),mReadGpoConfig(0),mI2CGpo(true){}


		/**
		 * Command parameters.
		 * @param i2cGpo true to change the i2c gpo, false for the rf gpo.
		 * @param newConfig new gpo function.
		 */
		void setNewGpoConfig(bool i2cGpo,NFC_GPO_MGMT newConfig){
			mGpoConfig=newConfig;
			mI2CGpo =i2cGpo;
		}

		virtual void onSelectedApplication(Nfc *nfc,NFC_StatusTypeDef status){
			if(status==NFC_SUCCESS)
				nfc->SelectSystemfile();
			else
				onFinishCommand(nfc,status);
		}

		virtual void onSelectedSystemFile(Nfc *nfc,NFC_StatusTypeDef status){
			if(status==NFC_SUCCESS)
				nfc->ReadBinary(0x0004, 0x01, &mReadGpoConfig);
			else
				onFinishCommand(nfc,status);
		}

		virtual void onReadByte(Nfc *nfc,NFC_StatusTypeDef status,
				uint16_t,uint8_t*, uint16_t ){
			if(status==NFC_SUCCESS)
				nfc->Verify(M24SR::I2CPwd, 0x10, M24SR::DEFAULT_PASSWORD);
			else
				onFinishCommand(nfc,status);
		}

		virtual void onVerified(Nfc *nfc,NFC_StatusTypeDef status,PasswordType_t, const uint8_t*){
			if(status!=NFC_SUCCESS)
				return onFinishCommand(nfc,status);

			if(mI2CGpo)
				mReadGpoConfig = (mReadGpoConfig & 0xF0) | (uint8_t)mGpoConfig;
			else
				mReadGpoConfig = (mReadGpoConfig & 0x0F) | (((uint8_t)mGpoConfig)<<4);

			nfc->UpdateBinary(0x0004, 0x01, &mReadGpoConfig);
		}

		virtual void onUpdatedBinary(Nfc *nfc,NFC_StatusTypeDef status,
				uint16_t , uint8_t*, uint16_t ){

			if(status==NFC_SUCCESS){
				if (mGpoConfig == I2C_ANSWER_READY){
					mParent.mCommunicationType = ASYNC;
				}else{
					mParent.mCommunicationType = SYNC;
				}//if-else
			}//status
			onFinishCommand(nfc,status);
		}

	private:

		/**
		 * Remove the private callback and call the user callback.
		 * @param nfc Object where the command was send to.
		 * @param status Command status.
		 */
		void onFinishCommand(Nfc *nfc,NFC_StatusTypeDef status){
			mParent.mComponentCallback=NULL;
			if(mI2CGpo){
				mParent.mCallback->onManageI2CGPO(nfc,status,mGpoConfig);
			}else{
				mParent.mCallback->onManageRFGPO(nfc,status,mGpoConfig);
			}//if-else
		}//onFinishCommand

		/**
		 * Component where send the commands
		 */
		M24SR &mParent;

		/** new gpo function that this class has to write */
		NFC_GPO_MGMT mGpoConfig;

		/**
		 * variable where storeing the read gpo configuration
		 */
		uint8_t mReadGpoConfig;

		/**
		 * true to change the i2c gpo, false to change the rf gpo
		 */
		bool mI2CGpo;

	};

	/**
	 * Object containing the callback chain needed to change the gpo function
	 */
	ManageGPOCallback mManageGPOCallback;


	/**
	 * Object with the callback used to read the component ID
	 * This class is equivalent to calling the methods:
	 * <ul>
	 *   <li> SelectedApplication </li>
	 *   <li> SelectSystemfile </li>
	 *   <li> ReadBinary </li>
	 * </ul>
	 */
	class ReadIDCallback : public Nfc::Callbacks{

	public:

		/**
		 * Build the chain of callbacks.
		 * @param parent object where to send the command to.
		 */
		ReadIDCallback(M24SR &parent):mParent(parent),mIdPtr(NULL){}

		/**
		 * Set the variable containing the result
		 * @param idPtr
		 */
		void readIdOn(uint8_t *idPtr){
			mIdPtr=idPtr;
		}

		virtual void onSelectedApplication(Nfc *nfc,NFC_StatusTypeDef status){
			if(status==NFC_SUCCESS){
				nfc->SelectSystemfile();
			}else
				onFinishCommand(nfc,status);

		}

		virtual void onSelectedSystemFile(Nfc *nfc,NFC_StatusTypeDef status){
			if(status==NFC_SUCCESS){
				nfc->ReadBinary(0x0011, 0x01, mIdPtr);
			}else
				onFinishCommand(nfc,status);
		}

		virtual void onReadByte(Nfc *nfc,NFC_StatusTypeDef status,
			uint16_t ,uint8_t *, uint16_t ){
			onFinishCommand(nfc,status);
		}

	private:

		/**
		 * Remove the private callback and call the user onReadId function.
		 * @param nfc Object where the command was send.
		 * @param status Command status.
		 */
		void onFinishCommand(Nfc *nfc,NFC_StatusTypeDef status){
			mParent.mComponentCallback=NULL;
			mParent.getCallback()->onReadId(nfc,status,mIdPtr);
		}

		M24SR& mParent;

		/**
		 * pointer to read id
		 */
		uint8_t *mIdPtr;
	};


	/**
	 * Object containing the callback chain needed to read the component id
	 */
	ReadIDCallback mReadIDCallback;
	friend class ReadIDCallback;

	/**
	 * Convert a generic enum to the value used by the M24SR chip.
	 * @param type Password type.
	 * @return equivalent value used inside the m24sr chip */
	uint16_t passwordTypeToConst(const PasswordType_t &type)const{
		switch(type){
			case ReadPwd:
				return READ_PWD;
			case WritePwd:
				return WRITE_PWD;
			case I2CPwd:
			default:
				return I2C_PWD;
		}//switch
	}//passwordTypeToConst
	
	/**
	 * convert  a uint16 into an enum value
	 * @param type
	 * @return
	 */
	PasswordType_t constToPasswordType(const uint16_t type)const{
		switch(type){
			case READ_PWD:
				return ReadPwd;
			case WRITE_PWD:
				return WritePwd;
			case I2C_PWD:
			default:
				return I2CPwd;
		}//switch
	}//passwordTypeToConst
};

#endif // __M24SR_CLASS_H

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
