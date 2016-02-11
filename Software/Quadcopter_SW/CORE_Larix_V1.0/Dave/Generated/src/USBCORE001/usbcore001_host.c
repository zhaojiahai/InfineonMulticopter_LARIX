/*******************************************************************************
**  DAVE App Name : USBCORE001       App Version: 1.0.18               
**  This file is generated by DAVE, User modification to this file will be    **
**  overwritten at the next code generation.                                  **
*******************************************************************************/


/* CODE_BLOCK_BEGIN[USBCORE001_host.c] */

/*******************************************************************************
 Copyright (c) 2011, Infineon Technologies AG                                 **
 All rights reserved.                                                         **
                                                                              **
 Redistribution and use in source and binary forms, with or without           **
 modification,are permitted provided that the following conditions are met:   **
                                                                              **
 *Redistributions of source code must retain the above copyright notice,      **
 this list of conditions and the following disclaimer.                        **
 *Redistributions in binary form must reproduce the above copyright notice,   **
 this list of conditions and the following disclaimer in the documentation    **
 and/or other materials provided with the distribution.                       **
 *Neither the name of the copyright holders nor the names of its contributors **
 may be used to endorse or promote products derived from this software without** 
 specific prior written permission.                                           **
                                                                              **
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  **
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    **
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   **
 ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   **
 LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         **
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         **
 SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    **
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      **
 CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       **
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   **
 POSSIBILITY OF SUCH DAMAGE.                                                  **
                                                                              **
 To improve the quality of the software, users are encouraged to share        **
 modifications, enhancements or bug fixes with Infineon Technologies AG       **
 dave@infineon.com).                                                          **
                                                                              **
********************************************************************************
**                                                                            **
**                                                                            **
** PLATFORM : Infineon XMC4000 Series                                         **
**                                                                            **
** COMPILER : Compiler Independent                                            **
**                                                                            **
** AUTHOR : App Developer                                                     **
**                                                                            **
** MAY BE CHANGED BY USER [yes/no]: Yes                                       **
**                                                                            **
** MODIFICATION DATE : Dec 4, 2012                                            **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                       Author(s) Identity                                   **
********************************************************************************
**                                                                            **
** Initials    Name                                                           **
** ---------------------------------------------------------------------------**
** CM          App Developer                                                  **
*******************************************************************************/

/* ==========================================================================
 *
 * Synopsys HS OTG Linux Software Driver and documentation (hereinafter,
 * "Software") is an Unsupported proprietary work of Synopsys, Inc. unless
 * otherwise expressly agreed to in writing between Synopsys and you.
 *
 * The Software IS NOT an item of Licensed Software or Licensed Product under
 * any End User Software License Agreement or Agreement for Licensed Product
 * with Synopsys or any supplement thereto. You are permitted to use and
 * redistribute this Software in source and binary forms, with or without
 * modification, provided that redistributions of source code must retain this
 * notice. You may not view, use, disclose, copy or distribute this file or
 * any information contained herein except pursuant to this license grant from
 * Synopsys. If you do not agree with this notice, including the disclaimer
 * below, then you are not authorized to use the Software.
 *
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS" BASIS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE HEREBY DISCLAIMED. IN NO EVENT SHALL SYNOPSYS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * ========================================================================== */

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/**
 * @file  usbcore001_host.c
 *
 * @brief This file contains implementations of all Public and Private functions
 *        of USB host glue logic.
 *
 */

/*******************************************************************************
**                      Include Files                                         **
*******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "../../src/USBLD001/Type_CM.h"
#include "../../src/USBLD001/dwc_common_port/usb.h"
#include "../../inc/USBCore001/usbcore001_host.h"
#include "../../src/USBLD001/driver/dwc_otg_hcd.h"
#include "../../src/USBLD001/driver/dwc_otg_hcd_if.h"

/**
 * @cond INTERNAL_DOCS
 */
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
/**
 * @ingroup USBCORE001_host_privateparam
 * @{
 */

#ifndef DWC_DEVICE_ONLY




typedef struct USBCORE001_HostEndpoint
{
  usb_endpoint_descriptor_t Desc;
  void *HcPriv;
}USBCORE001_HostEndpoint;

USBCORE001_HostEndpoint USBCORE001_HostEndpointpIn[USBCORE001_HOST_MAX_PIPE];

/**
 * This contains the information of the Host Controller Driver.
 */  
typedef struct USBCORE001_HostControllerDriver
{
  USBCORE001_HostEndpoint *EndPoint[USBCORE001_HOST_MAX_PIPE];
  USBCORE001_HostEvents *USB_HostEvents;
  USBCORE001_HostConnectEventsCallBack* USB_ConEvent;
  uint8_t CurPipeNum;
  dwc_otg_hcd_t *GHCD;
}USBCORE001_HostControllerDriver;

USBCORE001_HostControllerDriver USBCORE001_HostHCD;

struct dwc_otg_hcd_pipe_info USBCORE001_HostPipe[USBCORE001_HOST_MAX_PIPE] ;

/* URB instance to queue all transfers */
dwc_otg_hcd_urb_t USBCORE001_HostDwcOtgUrb;

/* Instance of host controller driver */
dwc_otg_hcd_t USBCORE001_HostDwcOtgHcd;

/* Period for interrupt transfer */ 
uint8_t USBCORE001_HostInterruptInterval = 0;

/* This flag indicates the completion of queued URB */
volatile int USBCORE001_HostURBFlag;

/* Data buffer to receive data */
uint8_t USBCORE001_DataBuffer[USBCORE001_HOST_BULK_XFER];

/**
 * @}
 */
 
 
/**
 * @ingroup USBCORE001_privatefunc
 * @{
 */

/*******************************************************************************
**                 Private Function declarations                              **
*******************************************************************************/
/**
 * @brief This callback will notify device disconnected from synopsys driver and 
 * will notify same to the LUFA.
 * @param[in]   hcd Pointer to dwc_otg_hcd_t structure
 * @return int
 */
static int USBCORE001_HostHCDDisconnect(dwc_otg_hcd_t * HCDPtr);

/**
 * @brief Dummy callback to make it compatible with synopsys driver.
 * @param[in]   hcd Pointer to dwc_otg_hcd_t structure
 * @return int
 */
static int USBCORE001_HostHCDStart(dwc_otg_hcd_t * HCDPtr);

/**
 * @brief Dummy callback to make it compatible with synopsys driver.
 * @param[in]   hcd Pointer to dwc_otg_hcd_t structure
 * @return int
 */
static int USBCORE001_HostHCDHubInfo(dwc_otg_hcd_t * HCDPtr,
      void *URBHandlePtr, uint32_t * HubAddrPtr,uint32_t * PortAddrPtr);

/**
 * @brief Dummy callback to make it compatible with synopsys driver.
 * @param[in]   hcd Pointer to dwc_otg_hcd_t structure
 * @return int
 */
static int USBCORE001_HostHCDSpeed(dwc_otg_hcd_t * HCDPtr, void *URBHandlePtr);

/**
 * @brief Dummy callback to make it compatible with synopsys driver.
 * @param[in]   hcd Pointer to dwc_otg_hcd_t structure
 * @return int
 */
static int USBCORE001_HostHCDGetBHNPEnable(dwc_otg_hcd_t * HCDPtr);

/**
 * @brief This call back will be called on completion of queued URB.
 * @param[in]   hcd Pointer to dwc_otg_hcd_t structure
 * @urb_handle[in]  pointer to dummy urb handle
 * @dwc_otg_urb[in] pointer to dwc_otg_hcd_urb_t structure
 * @status[in]    Indicates the completion status of URB
 * @return int
 */
static int USBCORE001_HostHCDComplete(dwc_otg_hcd_t * HCDPtr,
    void *URBHandlePtr, dwc_otg_hcd_urb_t * DwcOtgUrbPtr, int32_t Status);

/**
 * @brief This callback will notify device connected from synopsys driver and 
 * will notify same to the LUFA.
 * @param[in]   
 * @return int
 */
static int USBCORE001_HostHCDConnect(void);

/**
 * @}
 */

/** Host controller driver operation callback function structure */ 
static struct dwc_otg_hcd_function_ops USBCORE001_HostHCDfops = {
  USBCORE001_HostHCDStart,
  USBCORE001_HostHCDDisconnect,
  USBCORE001_HostHCDHubInfo,
  USBCORE001_HostHCDSpeed,
  USBCORE001_HostHCDComplete,
  USBCORE001_HostHCDGetBHNPEnable,
  USBCORE001_HostHCDConnect
};


/*******************************************************************************
**                     Private Function Definitions                           **
*******************************************************************************/
static int USBCORE001_HostHCDDisconnect(dwc_otg_hcd_t * HCDPtr)
{
  uint8_t PIPECnt;
  
  for(PIPECnt = 0; PIPECnt < USBCORE001_HOST_MAX_PIPE; PIPECnt++ )
  {
    if(USBCORE001_HostHCD.EndPoint[PIPECnt] != NULL)
      if(USBCORE001_HostHCD.EndPoint[PIPECnt]->HcPriv)
        USBCORE001_HostHCD.EndPoint[PIPECnt]->HcPriv = NULL;
  }
    
  if(NULL != USBCORE001_HostHCD.USB_ConEvent->USBCORE001_HostDisconnect)
  {
    USBCORE001_HostHCD.USB_ConEvent->USBCORE001_HostDisconnect();
  }  

  return 0;
}


static int USBCORE001_HostHCDStart(dwc_otg_hcd_t * HCDPtr)
{
  return 0;
}


static int USBCORE001_HostHCDHubInfo(dwc_otg_hcd_t * HCDPtr,
      void *URBHandlePtr, uint32_t * HubAddrPtr,uint32_t * PortAddrPtr)
{
  *HubAddrPtr = 0;
  *PortAddrPtr = 0;

  return 0;
}


static int USBCORE001_HostHCDSpeed(dwc_otg_hcd_t * HCDPtr, void *URBHandlePtr)
{
  hprt0_data_t * HPrtPtr;

  HPrtPtr = (hprt0_data_t *)HCDPtr->core_if->host_if->hprt0;

  if(HPrtPtr->b.prtspd & 1)
    return USB_SPEED_FULL;
  else
    return USB_SPEED_LOW;
}


static int USBCORE001_HostHCDGetBHNPEnable(dwc_otg_hcd_t * HCDPtr)
{
  return 0;
}


static int USBCORE001_HostHCDComplete(dwc_otg_hcd_t * HCDPtr,
    void *URBHandlePtr, dwc_otg_hcd_urb_t * DwcOtgUrbPtr, int32_t Status)
{
  
  if(DwcOtgUrbPtr->status == 0)
  { 
    USBCORE001_HostURBFlag = 1;
  }
  else
  {
    USBCORE001_HostURBFlag = DwcOtgUrbPtr->status;
  }

  return 0;
}


static int USBCORE001_HostHCDConnect(void)
{
  if(NULL != USBCORE001_HostHCD.USB_ConEvent->USBCORE001_HostConnect)
  {
    USBCORE001_HostHCD.USB_ConEvent->USBCORE001_HostConnect();
  }  

  return 0;
}

/**
 * @endcond
 */

/*******************************************************************************
**                     Public Function Definitions                            **
*******************************************************************************/
/* This function registers the call back for USB host status events  */
void USBCORE001_HostCallBackRegister(USBCORE001_HostEvents *USBH_Events)
{
  if(NULL != USBH_Events)
  {
    USBCORE001_HostHCD.USB_HostEvents = USBH_Events;
  }
}

/* This function registers call back for USB connect and disconnect events*/
void USBCORE001_ConnectEventRegister
                            (USBCORE001_HostConnectEventsCallBack *USB_ConEvent)
{
  if(NULL != USB_ConEvent)
  {	
    USBCORE001_HostHCD.USB_ConEvent = USB_ConEvent;
  }
}


/* This function initialises the synopsys host controller driver. */
int USBCORE001_HostStart(USBCORE001_OtgDevice *OtgDevPtr)
{
  int RetVal = 0;

  if(dwc_otg_hcd_init(&USBCORE001_HostDwcOtgHcd, OtgDevPtr->CoreIfPtr))
  {
    RetVal = -DWC_E_NO_MEMORY;
    goto end;
  }

  OtgDevPtr->HCDPtr = &USBCORE001_HostDwcOtgHcd;

  if (dwc_otg_hcd_start(&USBCORE001_HostDwcOtgHcd, &USBCORE001_HostHCDfops)) {
    return 0;
  }

end:
  return RetVal;
}


/* This function resets the USB host port.*/
void Reset_Host(void)
{
  reset_tasklet_func(&USBCORE001_HostDwcOtgHcd);
}

/* This function gets the status of USB host port.*/
uint8_t Get_HostStat(void)
{
  uint8_t HostStat = USBCORE001_HOST_STATE_WaitForDevice;
  if(NULL != USBCORE001_HostHCD.USB_HostEvents->USBCORE001_HostStatGet)
  {
    USBCORE001_HostHCD.USB_HostEvents->USBCORE001_HostStatGet(&HostStat);
  }
  return(HostStat);
}

/* This function selects through which communication will happen.
 * This is the LUFA compatible API.
 */
void Pipe_SelectPipe(const uint8_t PipeNumber)
{
  USBCORE001_HostRequestHeader Request;
  uint8_t HostStat = USBCORE001_HOST_STATE_WaitForDevice;

  if(NULL != USBCORE001_HostHCD.USB_HostEvents->USBCORE001_HostStatGet)
  {
    USBCORE001_HostHCD.USB_HostEvents->USBCORE001_HostStatGet(&HostStat);
  }

  if(PipeNumber == PIPE_CONTROLPIPE)
  {
    if(HostStat >= USBCORE001_HOST_STATE_Addressed)
    {
      USBCORE001_HostPipe[PIPE_CONTROLPIPE].dev_addr = USB_HOST_DEVICEADDRESS;
    }
    else
    {
      USBCORE001_HostPipe[PIPE_CONTROLPIPE].dev_addr = 0;    
    }

    if(NULL != USBCORE001_HostHCD.USB_HostEvents->USBCORE001_HostCtrlRequestGet)
    {
      USBCORE001_HostHCD.USB_HostEvents->USBCORE001_HostCtrlRequestGet(&Request);
    }
    USBCORE001_HostPipe[PIPE_CONTROLPIPE].mps = 8;
    USBCORE001_HostPipe[PIPE_CONTROLPIPE].pipe_dir 
                                  = Request.bmRequestType & 0xFE;
  }

  memcpy(&USBCORE001_HostDwcOtgUrb.pipe_info, &USBCORE001_HostPipe[PipeNumber],
                                        sizeof(struct dwc_otg_hcd_pipe_info));
  USBCORE001_HostHCD.CurPipeNum = PipeNumber;
}

/* This function sends control request to the selected pipe.
 * This is the LUFA compatible API.
 */
uint8_t USB_Host_SendControlRequest(void* const BufferPtr)
{
  uint8_t Status;
  USBCORE001_HostRequestHeader Request;
  
  if(NULL != USBCORE001_HostHCD.USB_HostEvents->USBCORE001_HostCtrlRequestGet)
  {
    USBCORE001_HostHCD.USB_HostEvents->USBCORE001_HostCtrlRequestGet(&Request);
  }  

  dwc_otg_hcd_urb_set_params(&USBCORE001_HostDwcOtgUrb, NULL, BufferPtr,
   (int)BufferPtr, Request.wLength, &Request, (int)&Request, 1, 0);

  USBCORE001_HostHCD.EndPoint[USBCORE001_HostHCD.CurPipeNum] = 
                     &USBCORE001_HostEndpointpIn[USBCORE001_HostHCD.CurPipeNum];

  USBCORE001_HostURBFlag = 0;
  if(dwc_otg_hcd_urb_enqueue(&USBCORE001_HostDwcOtgHcd, 
                             &USBCORE001_HostDwcOtgUrb, 
  &USBCORE001_HostHCD.EndPoint[USBCORE001_HostHCD.CurPipeNum]->HcPriv, 1) != 0)
  {
    return  HOST_SENDCONTROL_DeviceDisconnected;
  }  

  while(USBCORE001_HostURBFlag == 0);

  memset(&USBCORE001_HostDwcOtgUrb, 0, sizeof(USBCORE001_HostDwcOtgUrb));

  if(Get_HostStat() == USBCORE001_HOST_STATE_WaitForDevice)
  {
    Status = HOST_SENDCONTROL_DeviceDisconnected;
  }
  else if(USBCORE001_HostURBFlag != 1)
  {
    Status = HOST_SENDCONTROL_PipeError;
  }
  else
  {
    Status = HOST_SENDCONTROL_Successful;
  }

  return(Status);
}

/* This function configures pipes as needed by LUFA.
 * This is the LUFA compatible API.
 */
bool Pipe_ConfigurePipe(const uint8_t Number,
                        const uint8_t Type,
                        const uint8_t Token,
                        const uint8_t EndpointNumber,
                        const uint16_t Size,
                        const uint8_t Banks)
{
  if(Number >= USBCORE001_HOST_MAX_PIPE)
  {
    return false;
  }

  USBCORE001_HostPipe[Number].dev_addr = USB_HOST_DEVICEADDRESS;
  USBCORE001_HostPipe[Number].ep_num = EndpointNumber & 0x0F;
  USBCORE001_HostPipe[Number].pipe_type = Type;
  USBCORE001_HostPipe[Number].pipe_dir = EndpointNumber & 0xF0;
  USBCORE001_HostPipe[Number].mps = Size;

  return(true);
}


/* This function will prepare the USB request and will queue it to the
 * Synopsys driver.
 * This is the LUFA compatible API.
 */
uint8_t Pipe_Write_Stream_LE(const void* Buffer,
                             uint16_t Length,
                             uint16_t* const BytesProcessed)
{
  uint8_t Status;

  if(USBCORE001_HostPipe[USBCORE001_HostHCD.CurPipeNum].pipe_type
                                                          == EP_TYPE_INTERRUPT)
  {
    dwc_otg_hcd_urb_set_params(&USBCORE001_HostDwcOtgUrb, NULL, 
                              (void *)Buffer, (int)Buffer, Length, NULL,
                                0, 1, USBCORE001_HostInterruptInterval);
  }
  else
  {
    dwc_otg_hcd_urb_set_params(&USBCORE001_HostDwcOtgUrb, NULL,
                              (void *)Buffer, (int)Buffer, Length, NULL,
                               0, 1, 0);
  }

  USBCORE001_HostHCD.EndPoint[USBCORE001_HostHCD.CurPipeNum] = 
                    &USBCORE001_HostEndpointpIn[USBCORE001_HostHCD.CurPipeNum];
  USBCORE001_HostURBFlag = 0;
  if(dwc_otg_hcd_urb_enqueue(&USBCORE001_HostDwcOtgHcd, 
                             &USBCORE001_HostDwcOtgUrb, 
  &USBCORE001_HostHCD.EndPoint[USBCORE001_HostHCD.CurPipeNum]->HcPriv, 1) != 0 )
  {
    return  HOST_SENDCONTROL_DeviceDisconnected;    
  }

  while(USBCORE001_HostURBFlag == 0);

  memset(&USBCORE001_HostDwcOtgUrb, 0, sizeof(USBCORE001_HostDwcOtgUrb));

  if(Get_HostStat() == USBCORE001_HOST_STATE_WaitForDevice)
  {
    Status = HOST_SENDCONTROL_DeviceDisconnected;
  }
  else if(USBCORE001_HostURBFlag != 1)
  {
    Status = HOST_SENDCONTROL_PipeError;
  }
  else
  {
    Status = PIPE_RWSTREAM_NoError;
  }

  return(Status);
}

/* This function will prepare the USB request and will queue it 
 * to the synopsys driver. This is the LUFA compatible API.
 */
uint8_t Pipe_Read_Stream_LE(void* Buffer,
                            uint16_t Length,
                            uint16_t* const BytesProcessed)
{
  uint8_t Status;

  if(USBCORE001_HostPipe[USBCORE001_HostHCD.CurPipeNum].pipe_type 
                                                          == EP_TYPE_INTERRUPT)
  {
    dwc_otg_hcd_urb_set_params(&USBCORE001_HostDwcOtgUrb, NULL, Buffer,
                               (int)Buffer, Length, NULL, 0, 1,
                                USBCORE001_HostInterruptInterval);
  }
  else
  {
    dwc_otg_hcd_urb_set_params(&USBCORE001_HostDwcOtgUrb, NULL, Buffer, 
                               (int)Buffer, Length, NULL, 0, 1, 0);
  }

  USBCORE001_HostHCD.EndPoint[USBCORE001_HostHCD.CurPipeNum] = 
                    &USBCORE001_HostEndpointpIn[USBCORE001_HostHCD.CurPipeNum];

  USBCORE001_HostURBFlag = 0;
  if(dwc_otg_hcd_urb_enqueue(&USBCORE001_HostDwcOtgHcd, 
                             &USBCORE001_HostDwcOtgUrb,
   &USBCORE001_HostHCD.EndPoint[USBCORE001_HostHCD.CurPipeNum]->HcPriv, 1) != 0)
  {
    return  HOST_SENDCONTROL_DeviceDisconnected;
  }

  while(USBCORE001_HostURBFlag == 0);

  memset(&USBCORE001_HostDwcOtgUrb, 0, sizeof(USBCORE001_HostDwcOtgUrb));

  if(Get_HostStat() == USBCORE001_HOST_STATE_WaitForDevice)
  {
    Status = HOST_SENDCONTROL_DeviceDisconnected;
  }
  else if(USBCORE001_HostURBFlag == 1)
  {
    Status = PIPE_RWSTREAM_NoError;
  }
  else
  {
    Status = HOST_SENDCONTROL_PipeError;
  }

  return(Status);
}

/* This function will read one byte of data from device. 
 * This is the LUFA compatible API.
 */
int16_t Pipe_Read_8(void)
{
  static uint32_t RxDataLen = 0;
  static uint32_t Indx = 0;
  int16_t RxDataByte;

  if(RxDataLen < 1)
  {
    Indx = 0;
    RxDataLen = 0;
    memset(USBCORE001_DataBuffer, 0, 64 * sizeof(uint8_t));
    if(USBCORE001_HostPipe[USBCORE001_HostHCD.CurPipeNum].pipe_type
                              == EP_TYPE_INTERRUPT)
    {
      dwc_otg_hcd_urb_set_params(&USBCORE001_HostDwcOtgUrb, NULL, (int *)&USBCORE001_DataBuffer,
                    (int)NULL, USBCORE001_HOST_ONE_BYTE, NULL, 0, 1,
                    USBCORE001_HostInterruptInterval);
    }
    else
    {
      dwc_otg_hcd_urb_set_params(&USBCORE001_HostDwcOtgUrb, NULL, (int *)&USBCORE001_DataBuffer,
                    (int)NULL, USBCORE001_HOST_ONE_BYTE,
                    NULL, 0, 1, 0);
    }

    USBCORE001_HostHCD.EndPoint[USBCORE001_HostHCD.CurPipeNum] =
             &USBCORE001_HostEndpointpIn[USBCORE001_HostHCD.CurPipeNum];

    USBCORE001_HostURBFlag = 0;
    if(dwc_otg_hcd_urb_enqueue(&USBCORE001_HostDwcOtgHcd,
                              &USBCORE001_HostDwcOtgUrb,
     &USBCORE001_HostHCD.EndPoint[USBCORE001_HostHCD.CurPipeNum]->HcPriv, 1) != 0)
    {
      return  HOST_SENDCONTROL_DeviceDisconnected;
    }

    while(USBCORE001_HostURBFlag == 0);
    RxDataLen = USBCORE001_HostDwcOtgUrb.actual_length;
    memset(&USBCORE001_HostDwcOtgUrb, 0, sizeof(USBCORE001_HostDwcOtgUrb));

    if(RxDataLen != 0)
    {
      RxDataByte = USBCORE001_DataBuffer[Indx];
      RxDataLen--;
    }
    else
    {
      RxDataByte = -1;
    }
  }
  else
  {
    RxDataLen--;
    Indx++;
    RxDataByte = USBCORE001_DataBuffer[Indx];
  }

  return RxDataByte;
}


/* This function will prepare the USB request and will queue it
 * to the synopsys driver. This is the LUFA compatible API.
 */
void Pipe_Write_8(uint8_t Data)
{
  if(USBCORE001_HostPipe[USBCORE001_HostHCD.CurPipeNum].pipe_type 
                                                       == EP_TYPE_INTERRUPT)
  {
    dwc_otg_hcd_urb_set_params(&USBCORE001_HostDwcOtgUrb, NULL, &Data, 
                              (int)NULL, USBCORE001_HOST_ONE_BYTE, NULL, 0, 1,
                              USBCORE001_HostInterruptInterval);
  }
  else
  {
    dwc_otg_hcd_urb_set_params(&USBCORE001_HostDwcOtgUrb, NULL, &Data,
                               (int)NULL, USBCORE001_HOST_ONE_BYTE, NULL,
                               0, 1, 0);
  }              

  USBCORE001_HostHCD.EndPoint[USBCORE001_HostHCD.CurPipeNum] = 
                     &USBCORE001_HostEndpointpIn[USBCORE001_HostHCD.CurPipeNum];

  USBCORE001_HostURBFlag = 0;
  if(dwc_otg_hcd_urb_enqueue(&USBCORE001_HostDwcOtgHcd,
                             &USBCORE001_HostDwcOtgUrb,
  &USBCORE001_HostHCD.EndPoint[USBCORE001_HostHCD.CurPipeNum]->HcPriv, 1) != 0);  
  {
    //return  HOST_SENDCONTROL_DeviceDisconnected;
  }

  while(USBCORE001_HostURBFlag == 0);

  memset(&USBCORE001_HostDwcOtgUrb, 0, sizeof(USBCORE001_HostDwcOtgUrb));
}

/* Dummy function to make it compatible with LUFA stack.*/
void Pipe_Unfreeze(void)
{

}

/* Dummy function to make it compatible with LUFA stack.*/
void Pipe_Freeze(void)
{

}

/* Dummy function to make it compatible with LUFA stack.*/
void Pipe_ClearIN(void)
{

}

/* Dummy function to make it compatible with LUFA stack.*/
void Pipe_ClearOUT(void)
{

}


/* This function sets the periodicity of interrupt transfer.*/
void Pipe_SetInterruptPeriod(const uint8_t Milliseconds)
{
  USBCORE001_HostInterruptInterval = Milliseconds;
}

/* Dummy function to make it compatible with LUFA stack.*/
bool Pipe_IsEndpointBound(const uint8_t EndpointAddress)
{
  return 0;  
}
 
/* Dummy function to make it compatible with LUFA stack.*/
uint16_t Pipe_BytesInPipe(void)
{
  return 0;
}

/* Dummy function to make it compatible with LUFA stack.*/
bool Pipe_IsINReceived(void)
{
  return 1;
}

/* Dummy function to make it compatible with LUFA stack.*/
uint8_t Pipe_WaitUntilReady(void)
{
  return 0;
}

/* Dummy function to make it compatible with LUFA stack.*/
uint16_t USB_Host_GetFrameNumber(void)
{
  return 0;
}

/* Dummy function to make it compatible with LUFA stack.*/
bool Pipe_IsStalled(void)
{
  return 0;
}

/* Dummy function to make it compatible with LUFA stack.*/
bool Pipe_IsOUTReady(void)
{
  return 1;
}

/* Dummy function to make it compatible with LUFA stack.*/
bool Pipe_IsReadWriteAllowed(void)
{
  return 1;
}

/* This function swaps data endianness.*/
void SwapEndian_n(void* const Data, uint8_t Length)
{
  uint8_t* CurrDataPos = (uint8_t*)Data;

  while (Length > 1)
  {
    uint8_t Temp = *CurrDataPos;
    *CurrDataPos = *(CurrDataPos + Length - 1);
    *(CurrDataPos + Length - 1) = Temp;

    CurrDataPos++;
    Length -= 2;
  }
}
#endif /* DWC_DEVICE_ONLY */

/*CODE_BLOCK_END*/

