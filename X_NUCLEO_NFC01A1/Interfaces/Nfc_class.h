/**
 ******************************************************************************
 * @file    Nfc_class.h
 * @author  ST Central Labs
 * @version V1.0.0
 * @date    13-April-2015
 * @brief   This file contains the abstract class describing the interface of a
 *          nfc component.
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


/* Generated with Stm32CubeTOO -----------------------------------------------*/

/* Define to prevent from recursive inclusion --------------------------------*/

#ifndef __NFC_CLASS_H
#define __NFC_CLASS_H


/* Includes ------------------------------------------------------------------*/

#include "Component_class.h"
#include "../Common/nfc.h"

/* Classes  ------------------------------------------------------------------*/

/**
* An abstract class for Nfc components. 
* This component has two operation modes, sync or async.
* In sync mode each function call returns only after the command has completed.
* In async mode each function call returns immediately and the answer will be notified
* through a callback.
* The default behavior is sync mode.
* To enable the async mode ManageI2CGPO(I2C_ANSWER_READY) function must be called.
* When the component notifies an interrupt user must call  {@link ManageEvent} function.
* Note that passing a parameter other than I2C_ANSWER_READY to ManageI2CGPO initialize the component in sync mode.
*/
class Nfc : public Component
{
public:

	/**
	 * Possible password to set.
	 */
	typedef enum{
		ReadPwd,   //!< Password to use before reading the tag
		WritePwd,  //!< Password to use before writing the tag
		I2CPwd,    //!< Root password, used only through nfc
	}PasswordType_t;

	/**
	 * Function that will be called when an interrupt is fired,
	 * this function must be set if you want use the component in async mode.
	 */
	typedef void(*gpoEventCallback)(void);

	/**
	 * Object that contains all the callbacks fired by this class, each command has its own callback.
	 * The callback default implementation is an empty function.
	 */
	class Callbacks{
	public:

		/** called when GetSession or ForceGetSession completes
		 * @see Nfc#GetSession
		 * @see Nfc#ForceGetSession */
		virtual void onSessionOpen(Nfc *nfc,NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}

		/** called when Deselect completes
		 * @see Nfc#Deselect */
		virtual void onDeselect(Nfc *nfc,NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}

		/** called when SelectedApplication completes
		 * @see Nfc#SelectedApplication */
		virtual void onSelectedApplication(Nfc *nfc,NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}
		
		/** called when SelectedCCFile completes
		 * @see Nfc#SelectedCCFile */
		virtual void onSelectedCCFile(Nfc *nfc,NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}

		/** called when SelectedNDEFFile completes
		 * @see Nfc#SelectedNDEFFile */
		virtual void onSelectedNDEFFile(Nfc *nfc,NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}

		/** called when SelectedSystemFile completes
		 * @see Nfc#SelectedSystemFile */
		virtual void onSelectedSystemFile(Nfc *nfc,NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}

		/** called when ReadBinary or STReadBinary completes
		 * @see Nfc#ReadBinary
		 * @see Nfc#STReadBinary */
		virtual void onReadByte(Nfc *nfc,NFC_StatusTypeDef status,
				uint16_t offset,uint8_t *readByte, uint16_t nReadByte){
			(void)nfc; (void)status;(void)offset; (void)readByte; (void)nReadByte;
		}

		/** called when UpdatedBinary completes
		 * @see Nfc#UpdatedBinary */
		virtual void onUpdatedBinary(Nfc *nfc,NFC_StatusTypeDef status,uint16_t offset,
				uint8_t *writeByte,uint16_t nWriteByte){
			(void)nfc; (void)status; (void)writeByte; (void)nWriteByte; (void)offset;
		}

		/** called when Verify completes
		 * @see Nfc#Verify */	
		virtual void onVerified(Nfc *nfc,NFC_StatusTypeDef status,PasswordType_t uPwdId,
				const uint8_t *pwd){
			(void)nfc; (void)status;(void)uPwdId;(void)pwd;
		}

		/** called when ManageI2CGPO completes
		 * @see Nfc#ManageI2CGPO */
		virtual void onManageI2CGPO(Nfc *nfc,NFC_StatusTypeDef status,NFC_GPO_MGMT newStatus){
			(void)nfc; (void)status;(void)newStatus;
		}

		/** called when ManageRFGPO completes
		 * @see Nfc#ManageRFGPO */
		virtual void onManageRFGPO(Nfc *nfc,NFC_StatusTypeDef status,NFC_GPO_MGMT newStatus){
			(void)nfc; (void)status;(void)newStatus;
		}

		/** called when ChangeReferenceData completes
		 * @see Nfc#ChangeReferenceData */
		virtual void onChangeReferenceData(Nfc *nfc ,NFC_StatusTypeDef status,PasswordType_t type,
				uint8_t *data){
			(void)nfc; (void)status;(void)type;(void)data;
		}

		/** called when EnableVerificationRequirement completes
		 * @see Nfc#EnableVerificationRequirement */	
		virtual void onEnableVerificationRequirement(Nfc *nfc ,NFC_StatusTypeDef status,PasswordType_t type){
			(void)nfc; (void)status;(void)type;
		}
		
		/** called when DisableVerificationRequirement completes
		 * @see Nfc#DisableVerificationRequirement */	
		virtual void onDisableVerificationRequirement(Nfc *nfc , NFC_StatusTypeDef status,PasswordType_t type){
			(void)nfc; (void)status;(void)type;
		}

		/** called when EnablePermanentState completes
		 * @see Nfc#EnablePermanentState */	
		virtual void onEnablePermanentState(Nfc *nfc, NFC_StatusTypeDef status, PasswordType_t type){
			(void)nfc; (void)status;(void)type;
		}

		/** called when DisablePermanentState completes
		 * @see Nfc#DisablePermanentState */	
		virtual void onDisablePermanentState(Nfc *nfc, NFC_StatusTypeDef status, PasswordType_t type){
			(void)nfc; (void)status;(void)type;
		}

		/** called when ReadId completes
		 * @see Nfc#ReadId */
		virtual void onReadId(Nfc *nfc, NFC_StatusTypeDef status, uint8_t *id){
					(void)nfc; (void)status;(void)id;
		}

		/** called when EnableReadPassword completes
		 * @see Nfc#EnableReadPassword */
		virtual void onEnableReadPassword(Nfc *nfc, NFC_StatusTypeDef status,const uint8_t *newPwd){
			(void)nfc; (void)status;(void)newPwd;
		}

		/** called when EnableWritePassword completes
		 * @see Nfc#EnableWritePassword */
		virtual void onEnableWritePassword(Nfc *nfc, NFC_StatusTypeDef status,const uint8_t *newPwd){
			(void)nfc; (void)status;(void)newPwd;
		}

		/** called when DisableReadPassword completes
		 * @see Nfc#DisableReadPassword */
		virtual void onDisableReadPassword(Nfc *nfc, NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}

		/** called when DisableWritePassword completes
		 * @see Nfc#DisableWritePassword */
		virtual void onDisableWritePassword(Nfc *nfc, NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}

		/** called when DisableAllPassword completes
		 * @see Nfc#DisableAllPassword */
		virtual void onDisableAllPassword(Nfc *nfc, NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}


		/** called when EnableReadOnly completes
		 * @see Nfc#EnableReadOnly */
		virtual void onEnableReadOnly(Nfc *nfc,NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}

		/** called when EnableWriteOnly completes
		 * @see Nfc#EnableWriteOnly */
		virtual void onEnableWriteOnly(Nfc *nfc,NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}


		/** called when DisableReadOnly completes
		 * @see Nfc#DisableReadOnly */
		virtual void onDisableReadOnly(Nfc *nfc,NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}

		/** called when DisableWriteOnly completes
		 * @see Nfc#DisableWriteOnly */
		virtual void onDisableWriteOnly(Nfc *nfc,NFC_StatusTypeDef status){
			(void)nfc; (void)status;
		}

		virtual ~Callbacks(){};
	};

	Nfc():mCallback(&defaultCallback),
			mComponentCallback(NULL){ }

	/**
	 * Open a I2C connection with the tag if an RF connection isn't already open.
	 * @return operation status
	 */
	virtual NFC_StatusTypeDef GetSession(void) = 0;
	
	/**
	 * Force to open an I2C connection , abort the RF connection.
	 * @return NFC_SUCCESS if the session is acquired
	 */
	virtual NFC_StatusTypeDef ForceGetSession(void) = 0;

	/**
	 * Close an I2C connection.
	 * @return NFC_SUCCESS if the session is release
	 */
	virtual NFC_StatusTypeDef Deselect(void) = 0;

	/**
	 * Select the application file.
	 * @return NFC_SUCCESS if the application is selected
	 */
	virtual NFC_StatusTypeDef SelectApplication(void) = 0;

	/**
	 * Select the CC file.
	 * @return NFC_SUCCESS if the CC file is selected.
	 */
	virtual NFC_StatusTypeDef SelectCCfile(void) = 0;

	/**
	 * Select the NDEF file.
	 * @param NDEFfileId File id to open.
	 * @return NFC_SUCCESS if the file is selected
	 */
	virtual NFC_StatusTypeDef SelectNDEFfile(uint16_t NDEFfileId) = 0;

	/**
	 * Select the system file.
	 * @return NFC_SUCCESS if the system file is selected
	 */
	virtual NFC_StatusTypeDef SelectSystemfile(void) = 0;

	/**
	 * Read data from the tag.
	 * @param Offset Read offset.
	 * @param NbByteToRead Number of bytes to read.
	 * @param[out] pBufferRead Buffer to store the read data into.
	 * @return NFC_SUCCESS if no errors 
	 */
	virtual NFC_StatusTypeDef ReadBinary(uint16_t Offset, uint8_t NbByteToRead, uint8_t *pBufferRead) = 0;

	/**
	 * Write data to the tag.
	 * @param Offset Write offset.
	 * @param NbByteToWrite Number of bytes to write.
	 * @param pDataToWrite Buffer to write.
	 * @return NFC_SUCCESS if no errors
	 */
	virtual NFC_StatusTypeDef UpdateBinary(uint16_t Offset, uint8_t NbByteToWrite, uint8_t *pDataToWrite) = 0;

	/**
	 * Check that the password is correct.
	 * @param uPwdId Password type.
	 * @param NbPwdByte Password size ( must be 0x10).
	 * @param pPwd Password buffer.
	 * @return NFC_SUCCESS if no errors and write/read permission granted
	 */
	virtual NFC_StatusTypeDef Verify(PasswordType_t uPwdId, uint8_t NbPwdByte,const uint8_t *pPwd) = 0;

	/**
	 * Replace the read or write password.
	 * @param uPwdId Password to change.
	 * @param pPwd New password.
	 * @return NFC_SUCCESS if no errors
	 */
	virtual NFC_StatusTypeDef ChangeReferenceData(PasswordType_t uPwdId,const uint8_t *pPwd) = 0;
	
	/**
	 *	Activate NDEF file password protection.
	 *  When this command is successful, read or write access to the NDEF file is protected by a 128-bit password.
	 *  @param uReadOrWrite Read or write password.
	 *  @return NFC_SUCCESS if no errors
	 */
	virtual NFC_StatusTypeDef EnableVerificationRequirement(PasswordType_t uReadOrWrite) = 0;
	
	/**
     * Deactivate NDEF file password protection.
     * When this command is successful, read or write access to the NDEF file is granted.
     * with no security enforcement.
	 * @param uReadOrWrite Read or write password.
	 * @return NFC_SUCCESS if no errors
	 */     
	virtual NFC_StatusTypeDef DisableVerificationRequirement(PasswordType_t uReadOrWrite) = 0;
	
	/**
	 * Same as {@link NFC#ReadBinary}, however permitting to read more bytes than available.
	 * @param Offset read offset.
	 * @param NbByteToRead Number of bytes to read.
	 * @param[out] pBufferRead Buffer to store the read data into.
	 * @return NFC_SUCCESS if no errors
	 */
	virtual NFC_StatusTypeDef STReadBinary(uint16_t Offset, uint8_t NbByteToRead, uint8_t *pBufferRead) = 0;
	
	/**
	 * Set the tag as read/write only, stored in the eeprom.
	 * @param  uReadOrWrite ReadPwd if write only, WritePwd if read only.
	 * @return NFC_SUCCESS if no errors
	 */
	virtual NFC_StatusTypeDef EnablePermanentState(PasswordType_t uReadOrWrite) = 0;
	
	/**
	 * Unset the tag as read/write only, stored in the eeprom.
	 * @param  uReadOrWrite ReadPwd if write only, WritePwd if read only.
	 * @return NFC_SUCCESS if no errors
	 * @par Caller must have I2Csuper user permissions to call this function.
	 */
	virtual NFC_StatusTypeDef DisablePermanentState(PasswordType_t uReadOrWrite) = 0;
	
	/**
	 * Set the gpo output pin.
	 * @param uSetOrReset New pin status.
 	 * @return NFC_SUCCESS if no errors
	 */
	virtual NFC_StatusTypeDef StateControl(uint8_t uSetOrReset) = 0;
	
	/**
	 * @brief  This function configures GPO for I2C session.
	 * @param  GPO_I2Cconfig GPO configuration to set.
 	 * @return NFC_SUCCESS if no errors
 	 * @par if the configuration is I2C_ANSWER_READY, the component will start to work
 	 * in async mode.
	 */
	virtual NFC_StatusTypeDef ManageI2CGPO(NFC_GPO_MGMT GPO_I2Cconfig) = 0;
	
	/**
 	* @brief  This function configures GPO for RF session.
 	* @param  GPO_RFconfig GPO configuration to set.
  	* @return NFC_SUCCESS if no errors
 	*/
	virtual NFC_StatusTypeDef ManageRFGPO(uint8_t GPO_RFconfig) = 0;
	
	/**
	 * @brief  This function enables or disables the RF communication.
	 * @param  OnOffChoice GPO configuration to set.
  	 * @return NFC_SUCCESS if no errors
	 */
	virtual NFC_StatusTypeDef RFConfig(uint8_t OnOffChoice) = 0;

	/**
	 * Generates a negative pulse on the GPO pin.
	 * Pulse starts immediately after the command is issued and ends at the end of the RF response.
  	 * @return NFC_SUCCESS if no errors
	 */
	virtual NFC_StatusTypeDef SendInterrupt(void)=0;

	/**
	 * Change the function to call when a command ends.
	 * @param commandCallback Object containing the callback, if NULL it will use empty callback
	 */
	void SetCallback(Callbacks *commandCallback){
		if(commandCallback!=NULL)
			mCallback = commandCallback;
		else
			mCallback = &defaultCallback;
	}

	/**
	 * Function to call when the component fire an interrupt.
	 * @return last operation status
	 */
	virtual NFC_StatusTypeDef ManageEvent()=0;


	///////////////////////HIGH LEVEL / UTILITY FUNCTIONS ////////////////////////////////////

	/**
	 * Enable the request of a password before reading the tag.
	 * @param  pCurrentWritePassword Current password
	 * @param  pNewPassword Password to request before reading the tag.
	 * @return return NFC_SUCCESS if no errors
	 * @par The password must have a length of 16 chars.
	 */
	NFC_StatusTypeDef EnableReadPassword(const uint8_t* pCurrentWritePassword,
				const uint8_t* pNewPassword) {

		//enable the callback for change the gpo
		mComponentCallback = &mChangePasswordRequestStatusCallback;
		mChangePasswordRequestStatusCallback.setTask(ReadPwd,pNewPassword);

		return Verify(Nfc::WritePwd, 0x10, pCurrentWritePassword);
	}

	/**
	 * Disable the request of a password before reading the tag.
	 * @param  pCurrentWritePassword Current password
	 * @return return NFC_SUCCESS if no errors
	 * @par The password must have a length of 16 chars.
	 */
	NFC_StatusTypeDef DisableReadPassword(const uint8_t* pCurrentWritePassword) {
		mComponentCallback = &mChangePasswordRequestStatusCallback;
		mChangePasswordRequestStatusCallback.setTask(ReadPwd,NULL);

		return Verify(Nfc::WritePwd, 0x10, pCurrentWritePassword);
	}

	/**
	 * Enable the request of a password before writing to the tag.
	 * @param  pCurrentWritePassword Current password
	 * @param  pNewPassword Password to request before reading the tag.
	 * @return return NFC_SUCCESS if no errors
	 * @par The password must have a length of 16 chars.
	 */
	NFC_StatusTypeDef EnableWritePassword(const uint8_t* pCurrentWritePassword,
				uint8_t* pNewPassword) {
		//enable the callback for change the gpo
		mComponentCallback = &mChangePasswordRequestStatusCallback;
		mChangePasswordRequestStatusCallback.setTask(WritePwd,pNewPassword);

		return Verify(Nfc::WritePwd, 0x10, pCurrentWritePassword);
	}

	/**
	 * Disable the request of a password before writing the tag.
	 * @param  pCurrentWritePassword Current password.
	 * @return return NFC_SUCCESS if no errors
	 * @par The password must have a length of 16 chars.
	 */
	NFC_StatusTypeDef DisableWritePassword(const uint8_t* pCurrentWritePassword) {
		mComponentCallback = &mChangePasswordRequestStatusCallback;
		mChangePasswordRequestStatusCallback.setTask(WritePwd,NULL);

		return Verify(Nfc::WritePwd, 0x10, pCurrentWritePassword);
	}

	/**
	 * @brief   This function disables both read and write passwords.
	 * @param	pSuperUserPassword I2C super user password.
	 * @return  return NFC_SUCCESS if no errors
	 * @par The password must have a length of 16 chars.
	 */
	NFC_StatusTypeDef DisableAllPassword(const uint8_t* pSuperUserPassword){
		mComponentCallback = &mRemoveAllPasswordCallback;
		return Verify(Nfc::I2CPwd, 0x10, pSuperUserPassword);
	}

	/**
	 * @brief   This function enables read only mode.
	 * @param	pCurrentWritePassword Write password is needed to enable read only mode.
	 * @return  return NFC_SUCCESS if no errors
	 * @par The password must have a length of 16 chars.
	 */
	NFC_StatusTypeDef EnableReadOnly(const uint8_t* pCurrentWritePassword){

		mComponentCallback = &mChangeAccessStateCallback;
		//disable write = read only
		mChangeAccessStateCallback.changeAccessState(ChangeAccessStateCallback::WRITE,false);

		return Verify(Nfc::WritePwd, 0x10, pCurrentWritePassword);
	}

	/**
	 * @brief   This function disables read only mode.
	 * @param	pCurrentWritePassword Write password is needed to disable read only mode.
	 * @return  return NFC_SUCCESS if no errors
	 * @par The password must have a length of 16 chars.
	 */
	NFC_StatusTypeDef DisableReadOnly(const uint8_t* pCurrentWritePassword) {
		mComponentCallback = &mChangeAccessStateCallback;
		mChangeAccessStateCallback.changeAccessState(ChangeAccessStateCallback::WRITE,true);
		return Verify(Nfc::I2CPwd, 0x10,pCurrentWritePassword);
	}


	/**
	 * @brief   This function enables write only mode.
	 * @param	pCurrentWritePassword Write password is needed to enable write only mode.
	 * @return  return NFC_SUCCESS if no errors
	 * @par The password must have a length of 16 chars.
	 */
	NFC_StatusTypeDef EnableWriteOnly(const uint8_t* pCurrentWritePassword) {

		mComponentCallback = &mChangeAccessStateCallback;
		//disable read = enable write only
		mChangeAccessStateCallback.changeAccessState(ChangeAccessStateCallback::READ,false);

		return Verify(Nfc::WritePwd, 0x10, pCurrentWritePassword);

	}

	/**
	 * @brief   This function disables write only mode.
	 * @param	pCurrentWritePassword Write password is needed to disable write only mode.
	 * @return  return NFC_SUCCESS if no errors
	 * @par The password must have a length of 16 chars.
	 */
	NFC_StatusTypeDef DisableWriteOnly(const uint8_t* pCurrentWritePassword) {
		mComponentCallback = &mChangeAccessStateCallback;
		mChangeAccessStateCallback.changeAccessState(ChangeAccessStateCallback::READ,true);
		return Verify(Nfc::I2CPwd, 0x10, pCurrentWritePassword);
	}

	virtual ~Nfc(){};

protected:
		/** object containing the callbacks to use*/
		Callbacks *mCallback;

		/**
		 * Object with private callbacks used to hide high level commands each
		 * calling multiple low level commands. This callbacks object has 
		 * higher priority comparing to the user callbacks.
		 */
		Nfc::Callbacks *mComponentCallback;

		/**
		 * get the callback object to use
		 * @return callback object to use
		 */
		Nfc::Callbacks * getCallback(){
			if(mComponentCallback!=NULL)
				return mComponentCallback;
			return mCallback;
		}//getCallback

private:
	/** object containing empty callback to use in the default case*/
	Callbacks defaultCallback;


	/**
	 * This class permits to enable/disable the password request to read/write into the tag
	 * This class is equivalent to calling the methods:
	 * To enable the request:
	 * <ul>
	 *   <li> Verify </li>
	 *   <li> ChangeReferenceData </li>
	 *   <li> EnablePermanentState </li>
	 * </ul>
	 * To disable the request:
	 * <ul>
	 *   <li> Verify </li>
	 *   <li> DisableVerificationRequirement </li>
	 * </ul>
	 */
	class ChangePasswordRequestStatusCallback : public Nfc::Callbacks{

		public:

			/**
			 * Build the chain of callbacks.
			 */
			ChangePasswordRequestStatusCallback():
				mNewPwd(NULL),mType(I2CPwd),mEnable(false){}

			/**
			 * Set the password to enable/disable.
			 * @param type Type of password to enable/disable.
			 * @param newPwd Array of 16bytes with the new password, if null the request will be disabled.
			 */
			void setTask(PasswordType_t type, const uint8_t *newPwd){
				mNewPwd=newPwd;
				mType=type;
				mEnable=newPwd!=NULL;
			}//setTask

			virtual void onVerified(Nfc *nfc, NFC_StatusTypeDef status,PasswordType_t ,
					const uint8_t *){
				if(status!=NFC_SUCCESS)
					return onFinishCommand(nfc,status);
				if(mEnable)
					nfc->ChangeReferenceData(mType,mNewPwd);
				else
					nfc->DisableVerificationRequirement(mType);
			}

			virtual void onDisableVerificationRequirement(Nfc *nfc,
					NFC_StatusTypeDef status, PasswordType_t ){
				onFinishCommand(nfc,status);
			}

			virtual void onChangeReferenceData(Nfc *nfc, NFC_StatusTypeDef status,
					PasswordType_t type,
					uint8_t *){
				if(status==NFC_SUCCESS)
					nfc->EnablePermanentState(type);
				else
					onFinishCommand(nfc,status);
			}

			virtual void onEnablePermanentState(Nfc *nfc, NFC_StatusTypeDef status,
					PasswordType_t ){
				onFinishCommand(nfc,status);
			}


		private:

			/**
			 * Remove the private callbacks and call the user callback.
			 * @param nfc Object triggering the command.
			 * @param status Command status.
			 */
			void onFinishCommand(Nfc *nfc,NFC_StatusTypeDef status){
				nfc->mComponentCallback=NULL;

				if(mEnable){
					if(mType==ReadPwd){
						nfc->getCallback()->onEnableReadPassword(nfc,status,mNewPwd);
					}else
						nfc->getCallback()->onEnableWritePassword(nfc,status,mNewPwd);
				}else{
					if(mType==ReadPwd){
						nfc->getCallback()->onDisableReadPassword(nfc,status);
					}else
						nfc->getCallback()->onDisableWritePassword(nfc,status);
				}//if-else enable
			}//onFinish

			const uint8_t *mNewPwd;
			Nfc::PasswordType_t mType;
			bool mEnable;

		};


		/**
		 * Object containing the callback chain needed to change the password request
		 */
		ChangePasswordRequestStatusCallback mChangePasswordRequestStatusCallback;
		friend class ChangePasswordRequestStatusCallback;

		/**
		 * This class permits to disable all the password requests to read/write into the tag
		 * This class is equivalent to calling the methods:
		 * <ul>
		 *   <li> Verify(i2c) </li>
		 *   <li> DisablePermanentState(Read) </li>
		 *   <li> DisablePermanentState(write) </li>
		 *   <li> DisableVerificationRequirement(Read) </li>
		 *   <li> DisableVerificationRequirement(write) </li>
		 *   <li> ChangeReferenceData(Read) </li>
		 *   <li> ChangeReferenceData(write) </li>
		 * </ul>
		 */
		class RemoveAllPasswordCallback : public Nfc::Callbacks{

			/**
			 * Store the default password used for open a super user session
			 * it will be set as default read/write password
			 */
			const uint8_t *mI2CPwd;

			public:

				/**
				 * Build the chain of callbacks.
				 */
				RemoveAllPasswordCallback():mI2CPwd(NULL){}

				virtual void onVerified(Nfc *nfc,NFC_StatusTypeDef status,
						PasswordType_t,const uint8_t* data){
					if(status!=NFC_SUCCESS)
						return onFinishCommand(nfc,status);
					mI2CPwd = data;
					nfc->DisablePermanentState(ReadPwd);
				}

				virtual void onDisablePermanentState(Nfc *nfc , NFC_StatusTypeDef status,
						PasswordType_t type){
					if(status!=NFC_SUCCESS)
						return onFinishCommand(nfc,status);
					if(type==ReadPwd)
						nfc->DisablePermanentState(WritePwd);
					else
						nfc->DisableVerificationRequirement(ReadPwd);
				}

				virtual void onDisableVerificationRequirement(Nfc *nfc ,
						NFC_StatusTypeDef status,PasswordType_t type){
					if(status!=NFC_SUCCESS)
						return onFinishCommand(nfc,status);
					if(type==ReadPwd)
						nfc->DisableVerificationRequirement(WritePwd);
					else
						nfc->ChangeReferenceData(ReadPwd,mI2CPwd);
				}

				virtual void onChangeReferenceData(Nfc *nfc ,NFC_StatusTypeDef status,PasswordType_t type,
						uint8_t *data){
					if(status!=NFC_SUCCESS)
						return onFinishCommand(nfc,status);
					if(type==ReadPwd)
						nfc->ChangeReferenceData(WritePwd,data);
					else
						onFinishCommand(nfc,status);
				}

			private:

				/**
				 * Remove the private callback and call the onDisableAllPassword callback.
				 * @param nfc Object triggering the command.
				 * @param status Command status.
				 */
				void onFinishCommand(Nfc *nfc,NFC_StatusTypeDef status){
					nfc->mComponentCallback=NULL;
					mI2CPwd=NULL;
					nfc->getCallback()->onDisableAllPassword(nfc,status);
				}//onFinish

		};


		/**
		 * Object containing the callback chain needed to remove the password request
		 */
		RemoveAllPasswordCallback mRemoveAllPasswordCallback;
		friend class RemoveAllPasswordCallback;

		/**
		 * This class permits to set the tag as read/write only
		 * This class is equivalent to calling the methods:
		 * <ul>
		 *   <li> Verify(i2c) </li>
		 *   <li> EnablePermanentState(Read/write) </li>
		 * </ul>
		 * or:
		 * <ul>
		 *   <li> Verify(i2c) </li>
		 *   <li> DisablePermanentState</li>
		 *   <li> DisableVerificationRequirement(Read/write) </li>
		 * </ul>
		 */
		class ChangeAccessStateCallback : public Nfc::Callbacks{

			public:

				typedef enum{
					WRITE,
					READ
				}AccessType_t;

				/**
				 * Build the chain of callbacks.
				 */
				ChangeAccessStateCallback():mType(WRITE),mEnable(false){}

				/**
				 * Set the access to enable/disable an access type.
				 * @param type Access type.
				 * @param enable True to enable the state, False to disable it.
				 */
				void changeAccessState(AccessType_t type,bool enable){
					mType=type;
					mEnable=enable;
				}

				virtual void onVerified(Nfc *nfc,NFC_StatusTypeDef status,
						PasswordType_t,const uint8_t*){
					if(status!=NFC_SUCCESS)
						return onFinishCommand(nfc,status);

					if(mEnable){
						nfc->DisablePermanentState(mType==WRITE? WritePwd : ReadPwd);
					}else
						nfc->EnablePermanentState(mType==WRITE? WritePwd : ReadPwd);

				}

				virtual void onDisablePermanentState(Nfc *nfc, NFC_StatusTypeDef status,
						PasswordType_t type ){
					if(status!=NFC_SUCCESS)
						return onFinishCommand(nfc,status);

					nfc->DisableVerificationRequirement(type);
				}

				virtual void onDisableVerificationRequirement(Nfc *nfc , NFC_StatusTypeDef status,
						PasswordType_t ){
					onFinishCommand(nfc,status);
				}

				virtual void onEnablePermanentState(Nfc *nfc ,NFC_StatusTypeDef status,PasswordType_t ){
					onFinishCommand(nfc,status);
				}


			private:

				/**
				 * Remove the private callback and call the user callback.
				 * @param nfc Object triggering the command.
				 * @param status Command status.
				 */
				void onFinishCommand(Nfc *nfc,NFC_StatusTypeDef status){
					nfc->mComponentCallback=NULL;
					if(mEnable){
						if(mType==READ){
							//enable read = disable write only
							nfc->getCallback()->onDisableWriteOnly(nfc,status);
						}else
							//enable write = disable read only
							nfc->getCallback()->onDisableReadOnly(nfc,status);
					}else{
						if(mType==WRITE){
							//disable write = enable read only
							nfc->getCallback()->onEnableReadOnly(nfc,status);
						}else{
							//
							nfc->getCallback()->onEnableWriteOnly(nfc,status);
						}
					}//if-else enable
				}//onFinish

				AccessType_t mType;
				bool mEnable;

		};


		/**
		 * Object containing the callback chain needed to change the access state
		 */
		ChangeAccessStateCallback mChangeAccessStateCallback;
		friend class ChangeAccessStateCallback;

};

#endif /* __NFC_CLASS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
