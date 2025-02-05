/************************************************************************//**
 * @file
 * @version Preview
 * @copyright Copyright (c) Microsoft Corporation. All rights reserved.
 * SPDX-License-Identifier: MIT
 * @brief The Embedded Wireless Framework STM32 UART Host Interface API.
 ****************************************************************************/

#ifndef __ewf_interface_stm32_uart__h__included__
#define __ewf_interface_stm32_uart__h__included__

#include "ewf_interface.h"

#if (USE_HAL_UART_REGISTER_CALLBACKS != 1)
#error UART register callbacks must be enabled for this interface
#endif

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************//**
 * @defgroup group_interface_stm32_uart STM32 UART host interface
 * @ingroup group_interface
 * @brief STM32 UART host interface
 * @{
 ****************************************************************************/

/**
 * @defgroup group_interface_stm32_uart_config STM32 UART configuration
 * @{
 */

#ifndef EWF_INTERFACE_STM32_UART_CONFIG_UART_RX_QUEUE_SIZE
#define EWF_INTERFACE_STM32_UART_CONFIG_UART_RX_QUEUE_SIZE      (32)
#endif

#ifndef EWF_INTERFACE_STM32_UART_CONFIG_RX_THREAD_STACK_SIZE
/** This is the stack size for the reception thread, the default is conservative, you can reduce this in your application */
#define EWF_INTERFACE_STM32_UART_CONFIG_RX_THREAD_STACK_SIZE    (1024 * 2)
#endif

#ifndef EWF_INTERFACE_STM32_UART_CONFIG_RESPONSE_QUEUE_SIZE
#define EWF_INTERFACE_STM32_UART_CONFIG_RESPONSE_QUEUE_SIZE     (8)
#endif

#ifndef EWF_INTERFACE_STM32_UART_CONFIG_URC_QUEUE_SIZE
#define EWF_INTERFACE_STM32_UART_CONFIG_URC_QUEUE_SIZE          (8)
#endif

/** @} *** group_interface_stm32_uart_config */

/**
 * @brief The STM32 UART host interface implementation data structure type
 */
typedef struct _ewf_interface_stm32_uart
{
    UART_HandleTypeDef* uart_handle_ptr;
    ewf_platform_queue* rx_queue_ptr;
    uint8_t rx_buffer[sizeof(uint8_t)];

} ewf_interface_stm32_uart;

/**
 * @brief Start the STM32 UART hardware interface
 */
ewf_result ewf_interface_stm32_uart_hardware_start(ewf_interface* interface_ptr);

/**
 * @brief Stop the STM32 UART hardware interface
 */
ewf_result ewf_interface_stm32_uart_hardware_stop(ewf_interface* interface_ptr);

/**
 * @brief Send a buffer using the STM32 UART hardware interface
 */
ewf_result ewf_interface_stm32_uart_hardware_send(ewf_interface* interface_ptr, const uint8_t* buffer_ptr, uint32_t buffer_length);

/**
 * @brief Receive a buffer using the STM32 UART hardware interface
 */
ewf_result ewf_interface_stm32_uart_hardware_receive(ewf_interface* interface_ptr, uint8_t* buffer_ptr, uint32_t* buffer_length_ptr, bool wait);

#ifdef EWF_PARAMETER_CHECKING
#define EWF_INTERFACE_STM32_UART_INITIALIZE_HEADER(interface_ptr)                                                                                   \
do {                                                                                                                                                \
(interface_ptr)->struct_magic = EWF_INTERFACE_STRUCT_MAGIC;                                                                                         \
(interface_ptr)->struct_size = EWF_INTERFACE_STRUCT_SIZE;                                                                                           \
(interface_ptr)->struct_version = EWF_INTERFACE_VERSION;                                                                                            \
(interface_ptr)->struct_type = EWF_INTERFACE_TYPE_STM32_UART;                                                                                       \
} while(0)
#else
#define EWF_INTERFACE_STM32_UART_INITIALIZE_HEADER(interface_ptr)
#endif /* EWF_PARAMETER_CHECKING */

#ifdef EWF_PLATFORM_SUPPORTS_THREADING
#define EWF_INTERFACE_STM32_UART_INITIALIZE_RX_THREAD(thread_ptr, thread_name_symb, interface_ptr)                                                  \
do {                                                                                                                                                \
EWF_PLATFORM_THREAD_STATIC_DECLARE(                                                                                                                 \
    thread_ptr,                                                                                                                                     \
    thread_name_symb,                                                                                                                               \
    ewf_interface_receive_thread,                                                                                                                   \
    interface_ptr,                                                                                                                                  \
    EWF_INTERFACE_STM32_UART_CONFIG_RX_THREAD_STACK_SIZE,                                                                                           \
    EWF_PLATFORM_THREAD_PRIORITY_HIGH);                                                                                                             \
} while(0)
#else
#define EWF_INTERFACE_STM32_UART_INITIALIZE_RX_THREAD(thread_ptr, thread_name_symb, interface_ptr)
#endif /* EWF_ADAPTER_QUECTEL_BG95_MQTT_BASIC_ENABLED */

/**
 * @brief Declare a STM32 UART host interface
 * @param[in,out] interface_ptr a pointer to an interface that will be initialized to point to the one statically declared by the macro
 * @param[in] interface_name_symb a unique symbol name that will be used as a name suffix for the statically declared data
 */
#define EWF_INTERFACE_STM32_UART_STATIC_DECLARE(interface_ptr, interface_name_symb, uart_handle_ptr_param)                                          \
do {                                                                                                                                                \
static ewf_interface_stm32_uart ewf_interface_stm32_uart__implementation__##interface_name_symb = {0};                                              \
static ewf_interface ewf_interface_stm32_uart__interface__##interface_name_symb = {0};                                                              \
ewf_interface_stm32_uart__implementation__##interface_name_symb.uart_handle_ptr = uart_handle_ptr_param;                                            \
EWF_PLATFORM_QUEUE_STATIC_DECLARE(                                                                                                                  \
    ewf_interface_stm32_uart__implementation__##interface_name_symb.rx_queue_ptr,                                                                   \
    ewf_interface_stm32_uart__uart_rx_queue__##interface_name_symb,                                                                                 \
    uint8_t,                                                                                                                                        \
    EWF_INTERFACE_STM32_UART_CONFIG_UART_RX_QUEUE_SIZE);                                                                                            \
EWF_INTERFACE_STM32_UART_INITIALIZE_RX_THREAD(                                                                                                      \
    ewf_interface_stm32_uart__interface__##interface_name_symb.receive_thread_ptr,                                                                  \
    ewf_interface_stm32_uart__receive_thread__##interface_name_symb,                                                                                \
    &(ewf_interface_stm32_uart__interface__##interface_name_symb));                                                                                 \
EWF_PLATFORM_QUEUE_STATIC_DECLARE(                                                                                                                  \
    ewf_interface_stm32_uart__interface__##interface_name_symb.response_queue_ptr,                                                                  \
    ewf_interface_stm32_uart__response_queue__##interface_name_symb,                                                                                \
    ewf_interface_message,                                                                                                                          \
    EWF_INTERFACE_STM32_UART_CONFIG_RESPONSE_QUEUE_SIZE);                                                                                           \
EWF_PLATFORM_QUEUE_STATIC_DECLARE(                                                                                                                  \
    ewf_interface_stm32_uart__interface__##interface_name_symb.urc_queue_ptr,                                                                       \
    ewf_interface_stm32_uart__urc_queue__##interface_name_symb,                                                                                     \
    ewf_interface_message,                                                                                                                          \
    EWF_INTERFACE_STM32_UART_CONFIG_URC_QUEUE_SIZE);                                                                                                \
ewf_interface_stm32_uart__interface__##interface_name_symb.hardware_start = ewf_interface_stm32_uart_hardware_start;                                \
ewf_interface_stm32_uart__interface__##interface_name_symb.hardware_stop = ewf_interface_stm32_uart_hardware_stop;                                  \
ewf_interface_stm32_uart__interface__##interface_name_symb.hardware_send = ewf_interface_stm32_uart_hardware_send;                                  \
ewf_interface_stm32_uart__interface__##interface_name_symb.hardware_receive = ewf_interface_stm32_uart_hardware_receive;                            \
ewf_interface_stm32_uart__interface__##interface_name_symb.implementation_ptr = &(ewf_interface_stm32_uart__implementation__##interface_name_symb); \
interface_ptr = &(ewf_interface_stm32_uart__interface__##interface_name_symb);                                                                      \
EWF_INTERFACE_STM32_UART_INITIALIZE_HEADER(interface_ptr);                                                                                          \
} while(0)

/************************************************************************//**
 * @} *** group_interface_stm32_uart
 ****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __ewf_interface_stm32_uart__h__included__ */
