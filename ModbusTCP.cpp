/*
    ModbusTCP.cpp - Source for Modbus IP Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include "ModbusTCP.h"

extern uint8_t *buffer;
extern Serial pc;
uint16_t ret;

ModbusTCP::ModbusTCP(NetworkInterface* _net):network(_net)
{
    _server.open(network);
}

void ModbusTCP::server_open(uint16_t port)
{
    
    _server.bind(network->get_ip_address(),port);
    _server.listen(5);
}

void ModbusTCP::server_stop()
{
    _socket.close();
}

void ModbusTCP::server_run()
{
    _server.accept(&_socket, &_address);
    ret = _socket.recv(&buffer,sizeof(buffer)-1);
    buffer[ret] = '\0';

    pc.printf("%d\r\r\n",buffer[1]);
    // remaining = 12;
    // rcount = 0;
    // p = buffer;
    // while (0 < (r = _socket.recv(p, remaining))) {
    //     p += r;
    //     rcount += r;
    //     remaining -= r;
    // }

//     if (_server.accept(&_socket, &_address) == NSAPI_ERROR_OK) {
//         if (client.connected())
//         {
//             int i = 0;
//             while (client.available()){
//                 _MBAP[i] = client.read();
//                 i++;
//                 if (i==7) break;  //MBAP length has 7 bytes size
//             }
//             _len = _MBAP[4] << 8 | _MBAP[5];
//             _len--;  // Do not count with last byte from MBAP

//             if (_MBAP[2] !=0 || _MBAP[3] !=0) return;   //Not a MODBUSIP packet
//             if (_len > MODBUSIP_MAXFRAME) return;      //Length is over MODBUSIP_MAXFRAME

//             _frame = (byte*) malloc(_len);
//             i = 0;
//             while (client.available()){
//                 _frame[i] = client.read();
//                 i++;
//                 if (i==_len) break;
//             }

//             this->receivePDU(_frame);
//             if (_reply != MB_REPLY_OFF) {
//                 //MBAP
//                 _MBAP[4] = (_len+1) >> 8;     //_len+1 for last byte from MBAP
//                 _MBAP[5] = (_len+1) & 0x00FF;

//                 byte sendbuffer[7 + _len];

//                 for (i = 0 ; i < 7 ; i++) {
//                     sendbuffer[i] = _MBAP[i];
//                 }
//                 //PDU Frame
//                 for (i = 0 ; i < _len ; i++) {
//                     sendbuffer[i+7] = _frame[i];
//                 }
//                 client.write(sendbuffer, _len + 7);
//             }

// #ifndef TCP_KEEP_ALIVE
//             client.stop();
// #endif
//             free(_frame);
//             _len = 0;
//         }
//     }
}
