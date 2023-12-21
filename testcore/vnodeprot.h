#ifndef VNODEPROT_H
#define VNODEPROT_H

#define VNODE_ASCII_SOH         (0x55)

#define VNODE_POS_SOH           (0)
#define VNODE_POS_COMMAND       (1)
#define VNODE_POS_SIZE          (2)

#define VNODE_HEADER_BYTES      (3)
#define VNODE_TRAILER_BYTES     (1)
#define VNODE_CKS_BYTES         (1)
#define VNODE_EXTRA_BYTES       (VNODE_HEADER_BYTES + VNODE_TRAILER_BYTES)
#define VNODE_MSG_SZ_MIN        (VNODE_EXTRA_BYTES)
#define MSG_MIN_SZ              (VNODE_HEADER_BYTES + VNODE_TRAILER_BYTES + VNODE_CKS_BYTES)

/** Command structure :
    header + cmd + num_bytes + data_bytes[] + cks = 4 bytes + data_bytes[]

        num bytes = number of data bytes (CHECKSUM NOT INCLUDE))

        data_bytes[num_bytes] = message sequence id (or MsgCounter)

        msg minimum size = 5 (header + cmd + num_bytes + data_bytes[0] + cks) **/

typedef enum
{
    VNP_BLOCK_GET               = 0x00,
    VNP_BLOCK_SET               = 0x01,
    VNP_ADDRESS_SET             = 0x02,
    VNP_PORT_DIRECTION          = 0x03,
    VNP_PORT_GET                = 0x04,
    VNP_PORT_SET                = 0x05,
    VNP_PARAMETER_GET           = 0x06,
    VNP_PARAMETER_SET           = 0x07,
    VNP_TEST_PERFORM            = 0x08,
    VNP_TEST_STOP               = 0x09,
    VNP_DEBUG_STRING            = 0x0A,
    VNP_BLOCK_POLL              = 0x0B,
    VNP_NVMD_GET                = 0x0C,
    VNP_NVMD_SET                = 0x0D,
    VNP_NVMD_ERASE              = 0x0E,
    VNP_ANALOG_GET              = 0x0F,
    VNP_ANALOG_TRIGGER          = 0x10,
    VNP_ACQ_GET                 = 0x11,
    VNP_ACQ_SET                 = 0x12,
    VNP_PING                    = 0x13,
    VNP_CONNECTED               = 0x14,
    VNP_NVMA_GET                = 0x15,
    VNP_NVMA_SET                = 0x16,
    VNP_NVMA_ERASE              = 0x17,
    VNP_BUS_WRITE               = 0x18,
    VNP_RESET                   = 0x19,
    VNP_FASTSET_1               = 0x1A,
    VNP_FASTSET_2               = 0x1B,
    VNP_FASTSET_3               = 0x1C,
    VNP_FASTSET_4               = 0x1D,
    VNP_FASTGET_1               = 0x1E,
    VNP_FASTGET_2               = 0x1F,
    VNP_FASTGET_3               = 0x20,
    VNP_ANALOG_SINGLE_GET_ASK   = 0x21,
    VNP_nRST                    = 0x22,
    VNP_STEP_GPIO_SET           = 0x23,
    VNP_SYNC                    = 0x24,
    VNP_NACK                    = 0x25,
    VNP_GO_AHEAD                = 0x26,
    VNP_PLS_WAIT                = 0x27,
    VNP_IDLE                    = 0x28,
    VNP_ABORT                   = 0x29,
    VNP_WAIT_STEP_A             = 0x2A,
    VNP_WAIT_STEP_B             = 0x2B,
    VNP_WAIT_PEN_UP             = 0x2C,
    VNP_WAIT_PEN_DN             = 0x2D,
    VNP_WAIT_REP                = 0x2E,
    VNP_MOVE_TO                 = 0x2F,
    VNP_POLY_TO                 = 0x30,
    VNP_SET_X                   = 0x31,
    VNP_SET_Y                   = 0x32,
    VNP_STEP_X                  = 0x33,
    VNP_STEP_Y                  = 0x34,
    VNP_REP_X                   = 0x35,
    VNP_REP_Y                   = 0x36,
    VNP_END_PROC                = 0x37,
    VNP_INFO                    = 0x38,
    VNP_NEEDS_RESET             = 0x39
} CmdProtVirtual_t;

typedef enum
{
    GPIO_STEP_nSLP,
    GPIO_STEP_ENBL,
    GPIO_STEP_nSR_A,
    GPIO_STEP_nSR_B,
    GPIO_STEP_DIR_A,
    GPIO_STEP_DIR_B,
    GPIO_STEP_USM_A,
    GPIO_STEP_USM_B,
    GPIO_STEP_SENSOR_PAPER,
    GPIO_STEP_SENSOR_EOL,
    GPIO_STEP_LED_PAPER,
    GPIO_STEP_LED_EOL,
    GPIO_STEP_PEN,
    GPIO_STEP_PEN_HIGH_ENERGY,
    GPIO_STEP_PEN_LOW_ENERGY,
    GPIO_MAXSTEPPORTS  /* please keep this at the end */
}  gpioStepPort_t;

typedef enum
{
    ePLOTTER_DIR_PLUS,
    ePLOTTER_DIR_MINUS,
    ePLOTTER_MAX_DIR
} PLOTTER_dir_t;

typedef enum
{
    ePLOTTER_PEN_UP,
    ePLOTTER_PEN_DOWN
} PLOTTER_penPos_t;

typedef enum
{
    ePLOTTER_FULL_STEP,
    ePLOTTER_HALF_STEP,
    ePLOTTER_QUARTER_STEP,
    ePLOTTER_EIGHTH_STEP,
    ePLOTTER_SIXTEENTH_STEP,
    ePLOTTER_MAX_STEP_MODE
} PLOTTER_uStep_t;

typedef enum
{
    ePLOTTER_DETECT_SUCCESS = 0,
    ePLOTTER_DETECT_PAPER_STUCK,
    ePLOTTER_DETECT_WHITE_PAPER,
    ePLOTTER_DETECT_NON_STANDARD_PAPER,
    ePLOTTER_DETECT_PLOTTER_ERROR,
    ePLOTTER_DETECT_NO_PAPER,
    ePLOTTER_DETECT_END_OF_PAPER,
    ePLOTTER_BUSY_PRINTING,
    ePLOTTER_UNKNOWN_STATE
} PLOTTER_DetectStatus_t;

#endif //  VNODEPROT_H

