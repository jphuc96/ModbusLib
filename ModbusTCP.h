
#include <Modbus.h>
#include "NetworkInterface.h"

#ifndef __bswap_16
 #define __bswap_16(num) ((uint16_t)num>>8) | ((uint16_t)num<<8)
#endif

#define MODBUSTCP_PORT 	  502
#define MODBUSTCP_MAXFRAME 200
#define MODBUSTCP_TIMEOUT 1000
#define MODBUSTCP_UNIT	  255
#define MODBUSTCP_MAX_TRANSACIONS 16
#define MODBUSTCP_MAX_CLIENTS	  4

// Callback function Type
typedef bool (*cbModbusConnect)(SocketAddress ip);

typedef struct TTransaction TTransaction;

typedef bool (*cbTransaction)(Modbus::ResultCode event, TTransaction* t);

typedef struct TTransaction {
	uint16_t	transactionId;
	uint32_t	timestamp;
	cbTransaction cb = nullptr;
	uint8_t*	_frame;
    bool operator ==(const TTransaction &obj) const
	    {
		    return transactionId == obj.transactionId;
	    }
};

class ModbusTCP : public Modbus {
	protected:
	typedef union MBAP_t {
		struct {
			uint16_t transactionId;
			uint16_t protocolId;
			uint16_t length;
			uint8_t	 unitId;
		};
		uint8_t  raw[7];
	};
    MBAP_t _MBAP;
	cbModbusConnect cbConnect = nullptr;
	cbModbusConnect cbDisconnect = nullptr;
	TCPServer* server;
	TCPSocket* client;
	std::vector<TTransaction> _trans;
	int16_t		transactionId = 0;  // Last started transaction. Increments on unsuccessful transaction start too.
	int8_t n = -1;

	TTransaction* searchTransaction(uint16_t id);
	void cleanup(); 	// Free clients if not connected and remove timedout transactions
	int8_t getFreeClient();    // Returns free slot position
	int8_t getSlave(SocketAddress ip);
	bool send(SocketAddress ip, cbTransaction cb);

	public:
	ModbusTCP(NetworkInterface* _net) : network(_net);
	uint16_t lastTransaction();
	bool isTransaction(uint16_t id);
	bool isConnected(SocketAddress ip);
	bool connect(SocketAddress ip);
	bool disconnect(SocketAddress addr) {}  // Not implemented yet
	void slave();
	void master();
	void task();
	void begin(); 	// Depricated

    void onConnect(cbModbusConnect cb = nullptr);
	void onDisconnect(cbModbusConnect cb = nullptr);
    SocketAddress eventSource();

    bool writeCoil(SocketAddress ip, uint16_t offset, bool value, cbTransaction cb = nullptr);
	bool writeHreg(SocketAddress ip, uint16_t offset, uint16_t value, cbTransaction cb = nullptr);
	bool pushCoil(SocketAddress ip, uint16_t offset, uint16_t numregs = 1, cbTransaction cb = nullptr);
	bool pullCoil(SocketAddress ip, uint16_t offset, uint16_t numregs = 1, cbTransaction cb = nullptr);
	bool pullIsts(SocketAddress ip, uint16_t offset, uint16_t numregs = 1, cbTransaction cb = nullptr);
	bool pushHreg(SocketAddress ip, uint16_t offset, uint16_t numregs = 1, cbTransaction cb = nullptr);
	bool pullHreg(SocketAddress ip, uint16_t offset, uint16_t numregs = 1, cbTransaction cb = nullptr);
	bool pullIreg(SocketAddress ip, uint16_t offset, uint16_t numregs = 1, cbTransaction cb = nullptr);
};