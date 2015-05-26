//
// Created by benjamin barnes on 5/25/15.
//

#include <pthread.h>
#include "packetdescriptor.h"
#include "destination.h"
#include "pid.h"
#include "freepacketdescriptorstore.h"
#include "networkdevice.h"
#include "networkdriver.h"
#include "BoundedBuffer.h"

FreePacketDescriptorStore* fpds;
NetworkDevice* networkDevice;

BoundedBuffer* incomingBuffers[MAX_PID + 1];
BoundedBuffer* outgoingBuffer;

pthread_t incomingThread;
pthread_t outgoingThread;


/* These are the calls to be implemented by the students */


/* These calls hand in a PacketDescriptor for dispatching */
/* The nonblocking call must return promptly, indicating whether or */
/* not the indicated packet has been accepted by your code          */
/* (it might not be if your internal buffer is full) 1=OK, 0=not OK */
/* The blocking call will usually return promptly, but there may be */
/* a delay while it waits for space in your buffers.                */
/* Neither call should delay until the packet is actually sent      */
void blocking_send_packet(PacketDescriptor *pd){
    blockingWriteBB(outgoingBuffer, (void *) pd);
    return;
}

int  nonblocking_send_packet(PacketDescriptor *pd){
    return nonblockingWriteBB(outgoingBuffer, (void *) pd);
}



/* These represent requests for packets by the application threads */
/* The nonblocking call must return promptly, with the result 1 if */
/* a packet was found (and the first argument set accordingly) or  */
/* 0 if no packet was waiting.                                     */
/* The blocking call only returns when a packet has been received  */
/* for the indicated process, and the first arg points at it.      */
/* Both calls indicate their process number and should only be     */
/* given appropriate packets. You may use a small bounded buffer   */
/* to hold packets that haven't yet been collected by a process,   */
/* but are also allowed to discard extra packets if at least one   */
/* is waiting uncollected for the same PID. i.e. applications must */
/* collect their packets reasonably promptly, or risk packet loss. */
void blocking_get_packet(PacketDescriptor **pd, PID pid){
    *pd = blockingReadBB(incomingBuffers[pid]);
    return;
}
int  nonblocking_get_packet(PacketDescriptor **pd, PID pid){
    return nonblockingReadBB(incomingBuffers[pid],(void *) *pd);
}

// TODO: DOC STRING
void process_incoming(void* unused){
    PacketDescriptor *pd;
    //blocking_get_pd(fpds, )
    while(1){

    }
}

// TODO: DOC STRING
void process_outgoing(void* unused){
    int i;
    PacketDescriptor *pd;
    while(1){
        pd = (PacketDescriptor *) blockingReadBB(outgoingBuffer);
        for(i = 0; i < 5; i++){
            if((send_packet(networkDevice, pd)) == 1) {
                break;
            }
        }
        blocking_put_pd(fpds, pd);
    }
}

/* Called before any other methods, to allow you to initialise */
/* data structures and start any internal threads.             */
/* Arguments:                                                  */
/*   nd: the NetworkDevice that you must drive,                */
/*   mem_start, mem_length: some memory for PacketDescriptors  */
/*   fpds_ptr: You hand back a FreePacketDescriptorStore into  */
/*             which you have put the divided up memory        */

/* Hint: just divide the memory up into pieces of the right size */
/*       passing in pointers to each of them                     */
void init_network_driver(NetworkDevice *nd, void *mem_start, unsigned long mem_length, FreePacketDescriptorStore **fpds_ptr){
    int num_created, i;

    networkDevice = nd;

    *(fpds_ptr) = create_fpds();
    num_created = create_free_packet_descriptors(*(fpds_ptr), mem_start, mem_length);
    fpds = *(fpds_ptr);

    // TODO: calculate buffer sizes more generally
    outgoingBuffer = createBB(14);

    for(i = 0; i < MAX_PID + 1; i++){
        incomingBuffers[i] = createBB(2);
    }

    // TODO: check if return value is 0
    pthread_create(&incomingThread, NULL, &process_incoming, NULL);
    pthread_create(&outgoingThread, NULL, &process_outgoing, NULL);
}


