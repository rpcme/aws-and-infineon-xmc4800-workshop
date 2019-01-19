/**
 * @file Buttons.c
 * @date 2017-06-28
 *
 * @cond
 *********************************************************************************************************************
 * Buttons interface for XMC4300 Relax Kit v1.02
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************/

#include "xmc_gpio.h"
#include "xmc_scu.h"
#include "xmc_eru.h"
#include "Board_Buttons.h"

#define BUTTON1 P3_4
#define BUTTON_COUNT 2

/**
  \fn          int32_t Buttons_Initialize (void)
  \brief       Initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Initialize (void) {
  XMC_ERU_ETL_CONFIG_t eru_etl_config =
  {
    .input_a = ERU0_ETL1_INPUTA_SCU_HIB_SR0,
    .source = XMC_ERU_ETL_SOURCE_A,
    .status_flag_mode = XMC_ERU_ETL_STATUS_FLAG_MODE_HWCTRL,
    .edge_detection = XMC_ERU_ETL_EDGE_DETECTION_FALLING
  };

  // Initialize BUTTON1 (HIB_IO_0)
  XMC_SCU_HIB_EnableHibernateDomain();

  XMC_SCU_HIB_SetPinMode(XMC_SCU_HIB_IO_1, XMC_SCU_HIB_PIN_MODE_INPUT_PULL_NONE);
  XMC_SCU_HIB_SetSR0Input(XMC_SCU_HIB_SR0_INPUT_HIB_IO_1);
  while (XMC_SCU_GetMirrorStatus() != 0)
  {
    /* Wait until update of registers in HIB is done */
  }

  XMC_ERU_ETL_Init(XMC_ERU0, 1, &eru_etl_config);

  XMC_GPIO_SetMode(BUTTON1, XMC_GPIO_MODE_INPUT_TRISTATE);

  return 0;
}

/**
  \fn          int32_t Buttons_Uninitialize (void)
  \brief       De-initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Uninitialize (void) {

  return 0;
}

/**
  \fn          uint32_t Buttons_GetState (void)
  \brief       Get buttons state
  \returns     Buttons state
*/
uint32_t Buttons_GetState (void) {
  uint32_t val = 0;

  val = ((uint32_t)~XMC_GPIO_GetInput(BUTTON1) & 0x1U);

  if (XMC_ERU_ETL_GetStatusFlag(XMC_ERU0, 1) != 0)
  {
    val |= 2;
  }
  

  return val;
}

/**
  \fn          uint32_t Buttons_GetCount (void)
  \brief       Get number of available buttons
  \return      Number of available buttons
*/
uint32_t Buttons_GetCount (void) {
  return BUTTON_COUNT;
}
