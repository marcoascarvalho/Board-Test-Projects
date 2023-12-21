/*************************************************************************
 *
 *    TestCore
 *
 *    (c) Copyright Dixtal 2010
 *
 *    File name   : TestCore.c
 *    Description : Define a test Core for LPC2136
 *
 *    History :
 *    1. Data        : Jan 31, 2010
 *       Author      : Marco Carvalho
 *       Description : Create
 *
 *
 *    $Revision: 1.0$
**************************************************************************/
#include "includes.h"

#include    "vnodeprot.h"
#include    "TestCore.h"

    UCHAR _ucMsgCounter = 0;
    UCHAR _usProcCounter = 0;

#ifdef PLOTTER_RX_BUFFER
    USHORT _usRxStart = 0;
    USHORT _usRxEnd = 0;
#endif
#ifdef PLOTTER_TX_BUFFER
    BOOL   _bAuthorized = FALSE;
    USHORT _usTxStart = 0;
    USHORT _usTxEnd = 0;
#endif
    BOOL   _bNowReceiving = TRUE ;
    USHORT _usMessageLevel = 0;
    BOOL   _bAuthorized = FALSE;

/*************************************************************************
 * Function Name:  TransmitMessage - TX
 * Parameters: const UCHAR *buffer,
               const USHORT usBytesToTransmit

 * Return: void
 *
 * Description: Send message to Master Controller trough the Test UART
 *		
 *************************************************************************/
VOID TransmitMessage(const UCHAR *buffer, const USHORT usBytesToTransmit)
{
    UART_PutStringSz(TEST_UART_CONTROLLER, (char *) buffer, (size_t) usBytesToTransmit);
}

/*************************************************************************
 * Function Name: GetMessageLength - RX
 * Parameters: void
 * Return: USHORT
 *         Number of Chars in UART RX Buffer
 *
 * Description: Get the Number of Chars in UART RX Buffer that can be a
 *              message including a valid command. This is not the size field
 *              in the virtual node protocol but the RxCount Variable inside
 *              the LPC_Uart_Buffer_t structure.
 *		
 *************************************************************************/
USHORT GetMessageLength()
{
    return UART_GetCharsInBuffer(TEST_UART_CONTROLLER);
}

/*************************************************************************
 * Function Name: GetElement(USHORT aIdx) - RX
 * Parameters: USHORT aIdx (Index)
 * Return: UCHAR - One char from UART
 *
 * Description: Get one char from UART indexed from Index parameter (aIdx).
 *		
 *************************************************************************/
UCHAR GetElement(USHORT aIdx)
{
    ASSERT(GetMessageLength() > aIdx);

    return UART_GetCharAtIndex(TEST_UART_CONTROLLER, aIdx);
}

/*************************************************************************
 * Function Name: IncrCommandStart() - RX
 * Parameters: void
 * Return: UCHAR from RX UART
 *
 * Description: Get one char from UART. The main purpose of this function
 *              is to consume char from UART RX Buffer to free space in it.
 *		
 *************************************************************************/
UCHAR IncrCommandStart()
{
    unsigned char ucRetByte;

    ASSERT(GetMessageLength() > 0);

    ucRetByte = UART_GetChar(TEST_UART_CONTROLLER);

    return ucRetByte;
}

/*************************************************************************
 * Function Name: CommandIsNotEmpty - RX
 * Parameters: void
 * Return: BOOL
 *         0 - Command is empty
 *         1 - Command is not empty
 *
 * Description: Check if the message received is an Empty message or not by
 *              checking how many bytes are in RX UART Buffer.
 *		
 *************************************************************************/
BOOL CommandIsNotEmpty()
{
    return (BOOL) (GetMessageLength() > 0);
}

/*************************************************************************
 * Function Name: CommandIsNotValid() - RX
 * Parameters: void
 * Return: BOOL
 *         0 - Command is valid
 *         1 - Command is not valid
 *
 * Description: Check if the message received is a valid command or not by
 *
 *		
 *************************************************************************/
BOOL CommandIsNotValid()
{
    return (BOOL) (GetElement(VNODE_POS_SOH) != VNODE_ASCII_SOH);
}

/*************************************************************************
 * Function Name: GetCommandSize() - RX
 * Parameters: void
 * Return: USHORT
 *         Command Size (Virtual Node Protocol)
 *
 *
 * Description: Return the Size Field byte from the message received
 *              trough Virtual Node Protocol
 *		
 *************************************************************************/
USHORT GetCommandSize()
{

    ASSERT(GetMessageLength() >= VNODE_HEADER_BYTES);

    return (USHORT) GetElement(VNODE_POS_SIZE);
}

/*************************************************************************
 * Function Name: CheckSum - RX
 * Parameters: void
 * Return: BOOL
 *         0 - Checksum is not valid
 *         1 - Checksum is valid
 *
 * Description: Perform the xor operation with the command received bytes.
 *              If this operation result is zero, so the checksum is valid,
 *              otherwise is invalid.
 *		
 *************************************************************************/
BOOL CheckSum()
{
    USHORT i, sz;
    UCHAR chk;

    sz = GetCommandSize();

    chk = GetElement(0); // it's supposed to be always the Start Header Byte

    for (i = 1; i < sz + VNODE_EXTRA_BYTES; i++)
    {
        chk ^= GetElement(i);
    }

    return (BOOL) (chk == 0);
}

/*************************************************************************
 * Function Name: ManageOccupation - RX
 * Parameters: void
 * Return: void
 *
 * Description: general buffer management to pace down master
 * transmission (to stop and resume comm)
 *		
 *************************************************************************/
VOID ManageOccupation()
{
    /* Get How many Bytes are received in the USART RXBuff and are
       waiting to be processed */
    USHORT usOccupation = GetMessageLength();

    if (_bNowReceiving != FALSE)
    {
        /* if the Buffer is bigger than limit */
        /* why 128 bytes?? --> USART RX Buff deep is 16 bytes */
        /* will this condition occur?? */
        if (usOccupation >= PARSE_UPPER_BUFFER_LIMIT)
        {
            EncodeCommand(VNP_PLS_WAIT);
            _bNowReceiving = FALSE;
            _usMessageLevel = 1;
        }
    }
    else
    {
        /* if buffer is under the limit */
        if (usOccupation < PARSE_LOWER_BUFFER_LIMIT)
        {
            EncodeCommand(VNP_GO_AHEAD);
            _bNowReceiving = TRUE;
            _usMessageLevel = 0;
        }
        else
        {
            /* will this condition occur?? */
            if (usOccupation >= PARSE_UPPER_BUFFER_LIMIT + (PARSE_WARNG_BUFFER_LIMIT * _usMessageLevel))
            {
                EncodeCommand(VNP_PLS_WAIT);
                _usMessageLevel++;
            }

        }
    }
}

/*************************************************************************
 * Function Name: MessageIsComplete() - RX
 * Parameters: void
 * Return: BOOL
 *         TRUE - The message received is complete
 *         FALSE - The message received is not complete

 * Description: Get and check some informations from the message received to
 *              verify if it is complete. Informations checked includes Start
 *              Header field and command size field. Command size field is
 *              compared to the number of bytes received in UART.
 *		        why also not check the checksum here??
 *************************************************************************/
BOOL MessageIsComplete()
{
    USHORT usCommandSize, usMessageLength;

    if ((GetMessageLength() < VNODE_EXTRA_BYTES) ||(GetElement(VNODE_POS_SOH) != VNODE_ASCII_SOH))
    {
        return (BOOL) FALSE;
    }
    else
    {
        ASSERT(CommandIsNotValid() == FALSE);

        usCommandSize = GetCommandSize();

        /* Get How many Bytes we have in TEST UART RXBuff*/
        usMessageLength = GetMessageLength();

        if (usMessageLength >= usCommandSize + VNODE_EXTRA_BYTES)
        {
            return (BOOL) TRUE;
        }
        else
        {
            return (BOOL) FALSE;
        }
    }
}

/*************************************************************************
 * Function Name: ParseFlowControlCommands - RX
 * Parameters: CmdProtVirtual_t command,
 *             USHORT sizeMessage,
 *             UCHAR *msgCounter
 * Return: BOOL
 *         1 - Command consumed from UART (not Treated)
 *         0 - Command is not Flow Control and not consumed
 *
 * Description: Just parse Error/Flow Control Commands. In Fact not
 *              implemented in the Slave TestCore Yet. These kind of
 *              Commands/Messages just have sense in the TestCore TX side
 *              not in RX side. These functions may be implemented in the
 *              master in order to control the flow between the both sides
 *              of communication.
 *		
 *************************************************************************/
BOOL ParseFlowControlCommands(CmdProtVirtual_t command, USHORT sizeMessage, UCHAR *msgCounter)
{
    BOOL    rc  = TRUE;

    switch (command)
    {
        case VNP_NACK:
        {
            UCHAR ucWrongMessage;

            ASSERT(sizeMessage == 1);

            ucWrongMessage = IncrCommandStart();

            ASSERT((USHORT) ucWrongMessage <= 255); /* just to shut it up */
        }
        break;

        case VNP_GO_AHEAD:
        {
            ASSERT(sizeMessage == 1);
            *msgCounter =IncrCommandStart();
        }
        break;

        case VNP_PLS_WAIT:
        {
            ASSERT(sizeMessage == 1);
            *msgCounter = IncrCommandStart();
        }
        break;

        default:
            rc = FALSE;
        break;
    }

    return rc;
}

/*************************************************************************
 * Function Name: ComputeChkSum - TX
 * Parameters: const UCHAR *aBuffer,
 *             const USHORT aSize
 * Return: UCHAR - Computed Checksum
 *
 * Description: Compute the checksum between the bytes to be transmited
 *              in order to send it at the end of message
 *		
 *************************************************************************/
UCHAR ComputeChkSum(const UCHAR *aBuffer, const USHORT aSize)
{
    UCHAR chk = 0;
    USHORT i;

    for (i = 0; i < aSize; i++)
    {
        chk ^= aBuffer[i];
    }

    return chk;
}

/*************************************************************************
 * Function Name: AppendChkSum - TX
 * Parameters: UCHAR *aBuffer,
 *             const USHORT aSize
 * Return:  void
 *
 * Description: append computed checksum at the end of message to be
 *              transmited
 *		
 *************************************************************************/
VOID AppendChkSum(UCHAR *aBuffer, const USHORT aSize)
{
    aBuffer[aSize - 1] = ComputeChkSum(aBuffer, (USHORT) (aSize - 1));
}

/*************************************************************************
 * Function Name: EncodeInfo -  TX
 * Parameters: const UCHAR *aInfo,
 *             const USHORT aSize
 * Return: void
 *
 * Description: Encode VNP_INFO (0x38) command. This command contain the
 *              TestApp software version to be checked by the master program
 *		
 *************************************************************************/
VOID EncodeInfo(const UCHAR *aInfo, const USHORT aSize)
{
    static UCHAR buffer[VNODE_EXTRA_BYTES + PARSE_VERSION_SIZE + 1];
    USHORT usBytesToTransmit;

    ASSERT(aSize < PARSE_VERSION_SIZE);

    buffer[0] = VNODE_ASCII_SOH;
    buffer[1] = (UCHAR) VNP_INFO;
    buffer[2] = (UCHAR) (aSize + 1);

    memcpy(&buffer[VNODE_HEADER_BYTES], aInfo, aSize);

    buffer[VNODE_HEADER_BYTES + aSize] = _ucMsgCounter;
    _ucMsgCounter++;

    usBytesToTransmit = VNODE_EXTRA_BYTES + aSize + 1;

    AppendChkSum(buffer, usBytesToTransmit);
    TransmitMessage(buffer, usBytesToTransmit);
}

/*************************************************************************
 * Function Name: EncodeNack
 * Parameters: const UCHAR aCounter
 * Return: void
 *
 * Description: Encode VNP_NACK (0x25) command. This command inform the
 *              master or slave that previous command was not understood
 *		
 *************************************************************************/
VOID EncodeNack(const UCHAR aCounter)
{
    static UCHAR ucBuffer[VNODE_EXTRA_BYTES + 1];
    USHORT nBytesToTransmit;

    ucBuffer[0] = VNODE_ASCII_SOH;
    ucBuffer[1] = (UCHAR) VNP_NACK;
    ucBuffer[2] = sizeof(ucBuffer) - VNODE_EXTRA_BYTES;

    ucBuffer[VNODE_HEADER_BYTES] = aCounter;

    nBytesToTransmit = sizeof(ucBuffer);

    AppendChkSum(ucBuffer, nBytesToTransmit);
    TransmitMessage(ucBuffer, nBytesToTransmit);
}

/*************************************************************************
 * Function Name: EncodeCommand
 * Parameters: void
 * Return: void
 *
 * Description: Encode commands to be transmited. These are simple commands
 *              that require only message counter as a content.
 *		
 *************************************************************************/
VOID EncodeCommand(const CmdProtVirtual_t aCommand)
{
    static UCHAR ucBuffer[VNODE_EXTRA_BYTES + 1];
    USHORT nBytesToTransmit;

    ucBuffer[0] = VNODE_ASCII_SOH;
    ucBuffer[1] = (UCHAR) aCommand;
    ucBuffer[2] = sizeof(ucBuffer) - VNODE_EXTRA_BYTES;

    ucBuffer[VNODE_HEADER_BYTES] = _ucMsgCounter;
    _ucMsgCounter++;

    nBytesToTransmit = sizeof(ucBuffer);

    AppendChkSum(ucBuffer, nBytesToTransmit);
    TransmitMessage(ucBuffer, nBytesToTransmit);
}


/*************************************************************************
 * Function Name: AnswerSync()
 * Parameters: void
 * Return: void
 *
 * Description: Check if testcore is receiving something. if not receiving,
 *              send VNP_GO_AHEAD command to inform the controller to continue
 *              send commands otherwise send VNP_PLS_WAIT to put controller
 *              in hold until testcore can be receive more commands.
 *		
 *************************************************************************/
VOID AnswerSync()
{
    if (_bNowReceiving == FALSE)
    {
        EncodeCommand(VNP_PLS_WAIT);
    }
    else
    {
        EncodeCommand(VNP_GO_AHEAD);
    }
}


/*************************************************************************
 * Function Name: DecodeTestCommand()
 * Parameters: void
 * Return: void
 *
 * Description: Decode Test commands received from controller. If the
 *              command is completed and checksumed process it. The test
 *              command processor must be implemented in TestApp.c file
 *		
 *************************************************************************/
VOID DecodeTestCommand()
{

    USHORT usMaxCmds = 2;

    /* buffer occupation management */
    ManageOccupation();

    /* flush any garbage */
    while ((CommandIsNotEmpty() != FALSE) && (CommandIsNotValid() != FALSE))
    {
        (VOID) IncrCommandStart();
    }

    /* Check if receive some Test Command */
    while ((MessageIsComplete() != FALSE) && (usMaxCmds > 0))
    {
        BOOL bMsgIsOk;
        CmdProtVirtual_t command;
        UCHAR   ucMsgCounter = 0;
        USHORT  usCommandSize;
        UCHAR   ucStx;

        usMaxCmds--;

        /* hey, there is a complete message received ! But not yet checked ("checksumed") */

        if (CheckSum() != FALSE)
        {
            bMsgIsOk = TRUE; // now, the complete command was received correctly
        }
        else
        {
            bMsgIsOk = FALSE;
        }

        /* consumes STX and command */

        ucStx         = IncrCommandStart();
        command     = (CmdProtVirtual_t) IncrCommandStart();

        /* compute size */

        usCommandSize = IncrCommandStart();

        ASSERT(ucStx == VNODE_ASCII_SOH);

        /* act based on command */

        if ((ParseFlowControlCommands(command, usCommandSize, &ucMsgCounter) == FALSE) && (bMsgIsOk != FALSE))
        {
            ProcessTestCommand(command, usCommandSize, &ucMsgCounter);
        }

        /* asks for retransmission if msg was in error */

        if (bMsgIsOk == FALSE)
        {
            EncodeNack(ucMsgCounter);
        }

         /* skip check sum */

        (VOID) IncrCommandStart();

        /* flush any garbage */

        while ((CommandIsNotEmpty() != FALSE) && (CommandIsNotValid() != FALSE))
        {
            (VOID) IncrCommandStart();
        }
    }
}

