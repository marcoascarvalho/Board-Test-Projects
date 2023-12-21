/*************************************************************************
 *
 *    TestApp.h
 *
 *    (c) Copyright Dixtal 2010
 *
 *    File name   : TestApp.h
 *    Description : Define a test Core header for LPC2136
 *
 *    History :
 *    1. Data        : Feb 2, 2010
 *       Author      : Marco Carvalho
 *       Description : Create
 *
 *    $Revision: 1.0$
**************************************************************************/
#include "includes.h"

//#ifndef TESTAPP_H
//#define TESTAPP_H

//extern VOID TestAppMainLoop();
VOID ProcessTestCommand(CmdProtVirtual_t aCommand, USHORT aSizeMessage, UCHAR *aMsgCounter);

//#endif