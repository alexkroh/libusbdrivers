/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(NICTA_BSD)
 */

/*
 * These functions need to be implemented by platform
 * specific code.
 */

#ifndef __USB_USB_HOST_H_
#define __USB_USB_HOST_H_

#include <dma/dma.h>
#include <platsupport/io.h>
#include <usb/plat/usb.h>

enum usb_speed {
/// 1.5Mbps connection
    USBSPEED_LOW  = 0,
/// 12Mbps connection
    USBSPEED_FULL = 1,
/// 480Mbps connection
    USBSPEED_HIGH = 2
};

enum usb_xact_type {
/// Input PID
    PID_IN,
/// Output PID
    PID_OUT,
/// Setup PID
    PID_SETUP,
/// Interrupt PID
    PID_INT
};

enum usb_xact_status {
/// The transaction completed successfully
    XACTSTAT_SUCCESS,
/// The transaction has not been processed
    XACTSTAT_PENDING,
/// The transaction was cancelled due to disconnect, etc
    XACTSTAT_CANCELLED,
/// There was an error in processing the transaction
    XACTSTAT_ERROR,
/// The host exibited a failure during the transaction.
    XACTSTAT_HOSTERROR
};

struct xact {
/// Transfer type
    enum usb_xact_type type;
/// DMA buffer to exchange
    dma_mem_t buf;
/// The length of @ref{buf}
    int len;
};

/** Callback type for asynchronous USB transactions
 * @param[in] token  An unmodified opaque token as passed to
 *                   the associated transacton request.
 * @param[in] stat   The status of the transaction.
 * @return           1 if the transaction should be rescheduled,
 *                   otherwise, 0.
 */
typedef int (*usb_cb_t)(void* token, enum usb_xact_status stat);


struct usb_host;
typedef struct usb_host usb_host_t;

struct usb_host {
    /// Device ID
    enum usb_host_id id;

    /// DMA allocator
    struct dma_allocator* dalloc;

    /// Submit a transaction for transfer.
    int (*schedule_xact)(usb_host_t* hdev, uint8_t addr, uint8_t hub_addr, uint8_t hub_port,
                         enum usb_speed speed, int ep, int max_pkt, int rate_ms, struct xact* xact, int nxact,
                         usb_cb_t cb, void* t);
    /// Cancel all transactions for a given device address
    int (*cancel_xact)(usb_host_t* hdev, uint8_t usb_addr);
    /// Handle an IRQ
    void (*handle_irq)(usb_host_t* hdev);

    /// IRQ numbers tied to this device
    const int* irqs;
    /// Host private data
    struct usb_hc_data* pdata;
};

/**
 * Initialise USB host controller.
 * This function should only be called if you wish to use a raw API for the usb host controller, otherwise,
 * this function will be called by usb_init and the appropriate book keeping for device management
 * will be created and maintained.
 * @param[in]  id     The id of the host controller to initialise
 * @param[in]  ioops  a list of io operation functions.
 *                    of the initialised host controller
 * @param[in]  dalloc dma allocator for the host crontroller to use
 * @param[out] hdev   A host structure to populate. This must
 *                    already be filled with a DMA allocator.
 *                    and the device ID.
 * @return            0 on success
 */
int usb_host_init(enum usb_host_id id, ps_io_ops_t* ioops, struct dma_allocator* dalloc, usb_host_t* hdev);

/** Return a list of IRQ numbers handled by the provided host
 * @param[in]  host   A handle to the USB host device in question
 * @param[out] nirqs  The number of IRQs handled by this host.
 * @return            A NULL terminated list of IRQs
 */
const int* usb_host_irqs(usb_host_t* host, int* nirqs);


#endif /* __USB_USB_HOST_H_ */
