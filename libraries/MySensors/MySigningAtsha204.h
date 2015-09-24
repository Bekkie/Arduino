/*
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 */
/**
 * @ingroup MySigninggrp
 * @{
 * @file MySigningAtsha204.h
 * @brief ATSHA204 signing backend.
 *
 * The Atmel ATSHA204 offers true random number generation and
 * HMAC-SHA256 authentication with a readout-protected key.
 *
 * REVISION HISTORY
 *  - See git log
 */

#ifndef MySigningAtsha204_h
#define MySigningAtsha204_h

#include "MyConfig.h"
#include "MySigning.h"
#include "utility/ATSHA204.h"
#include <stdint.h>

#ifdef MY_SECURE_NODE_WHITELISTING
/**
 * @brief A single whitelist entry
 */
typedef struct {
	uint8_t nodeId;                   /**< @brief The ID of the node */
	uint8_t serial[SHA204_SERIAL_SZ]; /**< @brief Node specific serial number */
} whitelist_entry_t;
#endif

// The ATSHA204 is capable of generating proper random numbers for nonce
// and can calculate HMAC-SHA256 signatures. This is enterprise-
// level of security and ought to implement the signing needs for anybody.
class MySigningAtsha204 : public MySigning
{ 
public:
	/// @brief Construct the hardware implemented signing backend.
        ///
	/// With @ref MY_SECURE_NODE_WHITELISTING enabled, additional whitelist arguments can be provided
	/// To initialize the backend with a whitelist to filter accepted senders.
	/// @param requestSigs Set this to @c true if you want destination node to sign all messages sent to this node. Default is not to require signing.
	/// @param nof_whitelist_entries The number of entries in the provided whitelist.
	/// @param the_whitelist A list of whitelist entries.
	/// @param atshaPin A digital IO pin used to communicate with ATSHA204A device. Defaults to @ref MY_ATSHA204_PIN.
	MySigningAtsha204(bool requestSigs=true,
#ifdef MY_SECURE_NODE_WHITELISTING
		uint8_t nof_whitelist_entries=0, const whitelist_entry_t* the_whitelist=NULL,
#endif
		uint8_t atshaPin=MY_ATSHA204_PIN);
	bool getNonce(MyMessage &msg);
	bool checkTimer(void);
	bool putNonce(MyMessage &msg);
	bool signMsg(MyMessage &msg);
	bool verifyMsg(MyMessage &msg);
private:
	ATSHA204Class atsha204;
	unsigned long timestamp;
	bool verification_ongoing;
	uint8_t current_nonce[NONCE_NUMIN_SIZE_PASSTHROUGH+SHA204_SERIAL_SZ+1];
	uint8_t temp_message[SHA_MSG_SIZE];
	uint8_t rx_buffer[SHA204_RSP_SIZE_MAX];
	uint8_t tx_buffer[SHA204_CMD_SIZE_MAX];
	void calculateSignature(MyMessage &msg);
	uint8_t* sha256(const uint8_t* data, size_t sz);
#ifdef MY_SECURE_NODE_WHITELISTING
	uint8_t whitlist_sz;
	const whitelist_entry_t* whitelist;
#endif
};

#endif
/** @}*/