compile:
	gcc P1FIFO.c -o P1FIFO
	gcc P2FIFO.c -o P2FIFO
	gcc P1socket.c -o P1socket
	gcc P2socket.c -o P2socket
	gcc P1MessageQueue.c -o P1MessageQueue
	gcc P2MessageQueue.c -o P2MessageQueue


clean: 
	rm P1FIFO P2FIFO P1socket P2socket P1MessageQueue P2MessageQueue 
