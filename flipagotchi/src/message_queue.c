#include "../include/message_queue.h"

MessageQueue* message_queue_alloc() {
    MessageQueue* queue = malloc(sizeof(MessageQueue));

    queue->queueSize = PWNAGOTCHI_PROTOCOL_QUEUE_SIZE;

    queue->messageQueue = malloc(sizeof(uint8_t) * queue->queueSize);

    // Set everything to 0
    memset(queue->messageQueue, 0, queue->queueSize);

    queue->writePtr = queue->messageQueue;
    queue->readPtr = queue->messageQueue;

    return queue;
}

void message_queue_free(MessageQueue* queue) {
    free(queue->messageQueue);
    free(queue);

    queue = NULL;
}

bool message_queue_has_message(MessageQueue* queue) {
    // [patched] RESYNC — the real cause of the "screen freezes, needs restart" bug.
    // The original returned false forever whenever readPtr was not sitting exactly on
    // a START byte (e.g. after a dropped/partial UART byte). readPtr then never
    // advanced, the parser stalled permanently and the display stayed frozen until the
    // app was restarted. We now skip any garbage and re-align readPtr to the next
    // START byte (bounded by writePtr), so a single glitch can't wedge the stream.
    size_t qsize = queue->queueSize;
    size_t readIdx = (size_t)(queue->readPtr - queue->messageQueue);
    size_t writeIdx = (size_t)(queue->writePtr - queue->messageQueue);

    // Nothing buffered
    if(readIdx == writeIdx) {
        return false;
    }

    // Skip forward to the next START byte (drop desync garbage)
    while(readIdx != writeIdx && queue->messageQueue[readIdx] != PWNAGOTCHI_PROTOCOL_START) {
        readIdx = (readIdx + 1) % qsize;
    }
    queue->readPtr = queue->messageQueue + readIdx;

    if(readIdx == writeIdx) {
        // No START available yet; stay put and wait for more bytes
        return false;
    }

    // Look for the matching END, but never past the data we actually have
    size_t cursor = readIdx;
    while(cursor != writeIdx) {
        uint8_t b = queue->messageQueue[cursor];
        if(b == PWNAGOTCHI_PROTOCOL_END) {
            return true;
        }
        cursor = (cursor + 1) % qsize;
    }

    // START seen but message not complete yet
    return false;
}

void message_queue_push_byte(MessageQueue* queue, uint8_t data) {
    *(queue->writePtr) = data;

    // [patched] exact ring advance (keep writePtr/readPtr aligned)
    size_t idx = (size_t)(queue->writePtr - queue->messageQueue);
    queue->writePtr = queue->messageQueue + ((idx + 1) % queue->queueSize);
}

void message_queue_wipe(MessageQueue* queue) {
    // Set everything to 0
    memset(queue->messageQueue, 0, queue->queueSize);

    queue->readPtr = queue->messageQueue;
    queue->writePtr = queue->messageQueue;
}

bool message_queue_validate(MessageQueue* queue) {
    UNUSED(queue);
    /*      AS OF RIGHT NOW VALIDATE IS NOT IMPLEMENTED
    // Loop through and confirm that each message is a valid byte
    for (size_t ii = 0; ii < queue->queueSize - PWNAGOTCHI_PROTOCOL_BYTE_LEN; ii += PWNAGOTCHI_PROTOCOL_BYTE_LEN) {
        if (queue->messageQueue[ii] == 0 && queue->messageQueue[ii + PWNAGOTCHI_PROTOCOL_BYTE_LEN - 1] == 0) {
            continue;
        }

        if (!(queue->messageQueue[ii] == PWNAGOTCHI_PROTOCOL_START &&
            queue->messageQueue[ii + PWNAGOTCHI_PROTOCOL_BYTE_LEN - 1] == PWNAGOTCHI_PROTOCOL_END)) {
                // This means it failed so we should wipe and breka
                message_queue_wipe(queue);
                return false;
        }
    }
    */

    return true;
}

bool message_queue_pop_message(MessageQueue* queue, PwnCommand* dest) {
    if (!message_queue_has_message(queue)) {
        return false;
    }

    // Otherwise let's grab the message! Currently readPtr is pointing at STX
    dest->parameterCode = *(queue->readPtr + 1);

    // Wipe current arguments to hold 0's
    memset(dest->arguments, 0, PWNAGOTCHI_PROTOCOL_ARGS_MAX);

    // Everything folowing is arguments until we hit the end
    // We'll count up the number of bytes in the transmission to know how many to wipe
    int argCount;
    for (argCount = 0; argCount < PWNAGOTCHI_PROTOCOL_ARGS_MAX; argCount++) {
        uint8_t* bytePtr = queue->readPtr + 2 + argCount;

        // Wrap around if needed
        if (bytePtr >= queue->messageQueue + queue->queueSize) {
            bytePtr = queue->messageQueue;
        }

        if (*bytePtr == PWNAGOTCHI_PROTOCOL_END) {
            break;
        }

        dest->arguments[argCount] = *bytePtr;
    }

    //                  STX + CODE + args + ETX
    size_t messageSize = 1 + 1 + argCount + 1;

    // [patched] wrap-safe reset — the message may straddle the end of the ring.
    // The old code called memset(readPtr, 0, messageSize), which wrote past the
    // buffer whenever readPtr was near the end -> heap corruption -> crash/NULL-deref
    // after a couple of minutes (upstream issue #6 / #19).
    size_t startIdx = (size_t)(queue->readPtr - queue->messageQueue);
    for(size_t k = 0; k < messageSize; k++) {
        queue->messageQueue[(startIdx + k) % queue->queueSize] = 0;
    }

    // Now advance readPtr (exact ring wrap)
    queue->readPtr = queue->messageQueue + ((startIdx + messageSize) % queue->queueSize);

    return true;
}
