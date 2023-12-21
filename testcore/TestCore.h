/*************************************************************************
 *
 *    TestCore.h
 *
 *    (c) Copyright Dixtal 2010
 *
 *    File name   : TestCore.h
 *    Description : Define a test Core header for LPC2136
 *
 *    History :
 *    1. Data        : Feb 1, 2010
 *       Author      : Marco Carvalho
 *       Description : Create
 *
 *    $Revision: 1.0$
**************************************************************************/


#ifndef TESTCORE_H
#define TESTCORE_H

// Command structure
// header + cmd + num_bytes + data_bytes[] + cks = 4 bytes + data_bytes[]
// num bytes = number of data bytes (CHECKSUM NOT INCLUDE)
// data_bytes[num_bytes] = message sequence id (or MsgCounter)
// msg minimum size = 5

#define TEST_UART_CONTROLLER UART0

#define PARSE_UPPER_BUFFER_LIMIT    ((PARSE_RX_BUFFER_SZ) / 4)
#define PARSE_LOWER_BUFFER_LIMIT    ((PARSE_RX_BUFFER_SZ) / 8)
#define PARSE_WARNG_BUFFER_LIMIT    ((PARSE_RX_BUFFER_SZ) / 8)
#define PARSE_TX_CHUNK              (100)
#define PARSE_VERSION_SIZE          (32)
#define PARSE_ASK_RESET_FREQUENCY   (10000)

#define PARSE_RX_BUFFER_SZ          (512)

//#ifdef WIN32
//#define PARSE_IDLE_FREQUENCY        (300)
//#else
//#define PARSE_IDLE_FREQUENCY        (6000)
//#endif
//#include <system/System.h>

    /**
    * perform command analysis and acts properly
    */
    //VOID    ParseCommands       (UCHAR *aBuffer, LONG aSz);
    /**
    * general buffer management to pace down master transmission (to stop and resume comm)
    */
    VOID    ManageOccupation    ();
    /**
    * general reset management: to guarantee that after a reset, master will realign with us
    */
    VOID    ManageReset         ();
    /**
    * simple command encoding (no arguments other than command itself)
    */
    VOID    EncodeCommand       (const CmdProtVirtual_t aCommand);
    /**
    * special encoding because it does not respect busy plotter state
    */
    VOID    EncodeImmediate     (const CmdProtVirtual_t aCommand);
    /**
    * immediate abort encoding (VNP_ABORT and the sequence "ABORT"
    */
    VOID    EncodeImmediateAbort();
    /**
    * encodes a move to x, y
    */
    //VOID    EncodeMoveTo        (const USHORT asX, const USHORT asY);
    /**
    * encodes a pconsecutive vector move to x, y
    */
    //VOID    EncodePolyTo        (const USHORT *aXPoints, const USHORT *aYPOINTS, const USHORT anPoints);
    /**
    * encodes a gpio state
    */
    //VOID    EncodeGPIO          (const gpioStepPort_t aPort, const UCHAR aState);
    /**
    * encodes waiting times for many actions (pen motion, motors)
    */
    VOID    EncodeWait          (const CmdProtVirtual_t cmd, const USHORT wait);
    /**
    * encodes new absolute position of motors
    */
    // VOID    EncodeSet           (const CmdProtVirtual_t cmd, const USHORT aPos);
    /**
    * communication error. Encodes nack telling which message was badly received
    */
    VOID    EncodeNack          (const UCHAR aCounter);
    /**
    * procedure to step a motor multiple times, at a maximum of aCounter times, until aCondition is satisfied
    */
    //VOID    EncodeRep           (const CmdProtVirtual_t cmd, const USHORT aCounter, const BOOL aCondition);
    /**
    * gets multiple boolean conditions
    */
    //PLOTTER_STATUS GetStatus()  { return m_Status; }
    /**
    * sets multiple boolean conditions
    */
    //VOID    SetStatus           (const PLOTTER_STATUS aStatus ) { m_Status = aStatus; };
    /**
    * gets result of an EncodeRep command
    */
    //USHORT  GetProcCounter()    { return m_usProcCounter; }
    /**
    * Sets driver version
    */
    //VOID    SetVersion          (const UCHAR *aVersion);
    /**
    * Gets driver version
    */
    //const UCHAR *GetVersion     (VOID) const;
    /**
    * basically returns busy or not, and also sensors
    */
    VOID    AnswerSync          ();
    /*
    * this is the answer to an EncodeRep command
    */
    //VOID    EncodeEndProc       (const USHORT aCounter);
    /*
    * this is the way to any information to the other party, such as version
    */
    // VOID    EncodeInfo          (const UCHAR *aInfo, const USHORT aSize);
    //VOID    EncodeDebug         (const UCHAR *string, const UCHAR len);

    //VOID    SendMessage      (const UCHAR *buffer, USHORT usBytesToTransmit);
    VOID    TransmitMessage     (const UCHAR *aBuffer, const USHORT aSize);
    BOOL    ParseFlowControlCommands    (CmdProtVirtual_t command, USHORT sizeMessage, UCHAR *msgCounter);
    VOID    ProcessTestCommand  (CmdProtVirtual_t aCommand, USHORT aSizeMessage, UCHAR *aMsgCounter);
    UCHAR   ComputeChkSum       (const UCHAR *aBuffer, const USHORT aSize);
    VOID    AppendChkSum        (UCHAR *aBuffer, const USHORT aSize);
    VOID    EncodeMsg           (VOID  *aBuffer, USHORT aSize);

    USHORT GetMessageLength();
    BOOL CommandIsNotEmpty();
    BOOL CommandIsNotValid();
    USHORT GetCommandSize();
    UCHAR GetElement(USHORT aIdx);
    VOID DecodeTestCommand();
    BOOL MessageIsComplete();
    VOID EncodeInfo(const UCHAR *aInfo, const USHORT aSize);
    UCHAR IncrCommandStart();

    //VOID    SerialWrite         (const UCHAR *aBuffer, const LONG aSz);
    //USHORT  SerialRead          (UCHAR *aBuffer, LONG aSz);

    //UCHAR   m_ucMsgCounter;
    //LONG    m_lPlotterDriver;

    //PLOTTER_STATUS m_Status;
    //USHORT  m_usProcCounter;
    //UCHAR   m_ucVersion[PARSE_VERSION_SIZE];


#endif
