/**
  ******************************************************************************
  * @file       X_NUCLEO_NFC01A1.h
  * @author  	ST Central Labs
  * @date       05 Nov 2015
  * @brief      Singleton class that controls all the electronics inside the 
  * 			X_NUCLEO_NFC01A1 expansion board
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
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

#include <X_NUCLEO_NFC01A1.h>

const uint8_t  X_NUCLEO_NFC01A1::M24SR_ADDR=0xAC;
const PinName  X_NUCLEO_NFC01A1::DEFAULT_SDA_PIN=D14;
const PinName  X_NUCLEO_NFC01A1::DEFAULT_SDL_PIN=D15;


const PinName X_NUCLEO_NFC01A1::DEFAULT_GPO_PIN=D12;
const PinName X_NUCLEO_NFC01A1::DEFAULT_RF_DISABLE_PIN=D11;
const PinName X_NUCLEO_NFC01A1::DEFAULT_LED1_PIN=D5;
const PinName X_NUCLEO_NFC01A1::DEFAULT_LED2_PIN=D4;
const PinName X_NUCLEO_NFC01A1::DEFAULT_LED3_PIN=D2;

X_NUCLEO_NFC01A1 *X_NUCLEO_NFC01A1::mInstance = NULL;

X_NUCLEO_NFC01A1* X_NUCLEO_NFC01A1::Instance(I2C &devI2C,
		Nfc::gpoEventCallback gpoEventCallback,
		const PinName &gpoName,
		const PinName &RFDisableName, const PinName &led1Name,
		const PinName &led2Name, const PinName &led3Name) {
	if (mInstance == NULL) { // the first time
		mInstance = new X_NUCLEO_NFC01A1(devI2C,gpoEventCallback,gpoName,
				RFDisableName,led1Name,led2Name,led3Name);
		if (mInstance != NULL) { //allocation ok
			const int status = mInstance->mM24SR.Init(NULL);
			if (status != NFC_SUCCESS) { //initialization failed
				delete mInstance;
				error(
						"Failed to init X_NUCLEO_NFC01A1 expansion board!\r\nError:0x%X\r\n",
						status);
			} //if init
		} //if instance !=NULL
	} //if instance
	return mInstance;
}
