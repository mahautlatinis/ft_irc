import socket
import threading
from time import sleep

IN_ADDR = ('0.0.0.0', 3333)
OUT_ADDR = ('127.0.0.1', 6667)
BUFF_SIZE = 512
DELIM = "\r\n"

COLORS = [
	'\033[91m',
	'\033[92m',
	'\033[93m',
	'\033[94m',
	'\033[95m',
	'\033[96m',
]
ENDC = '\033[0m'

clients = []

class Client:
	currentColor = 0

	def __init__(self, sock: socket.socket, addr):
		self.color = COLORS[Client.currentColor]
		Client.currentColor = (Client.currentColor + 1) % len(COLORS)

		self.inSock = sock
		self.inAddr = addr
		self.inThread = threading.Thread(target=self.inWorker)

		self.outSock = socket.socket()
		self.outSock.connect(OUT_ADDR)
		self.outThread = threading.Thread(target=self.outWorker)

		self.print("New client " + str(addr))
		self.running = True
		self.ended = False
		self.inThread.start()
		self.outThread.start()

	def terminate(self):
		sleep(0.5)
		if self.ended:
			return
		self.ended = True
		clients.remove(self)
		try:
			self.inSock.shutdown(socket.SHUT_RDWR)
			self.inSock.close()
			self.print("INSOCK CLOSED")
		except Exception as e:
			self.print("Exception close inSock: " + str(e))
		try:
			self.outSock.shutdown(socket.SHUT_RDWR)
			self.outSock.close()
			self.print("OUTSOCK CLOSED")
		except Exception as e:
			self.print("Exception close outSock: " + str(e))

	def inWorker(self):
		"""Forward message from client to server"""
		try:
			while self.running:
				raw = self.inSock.recv(BUFF_SIZE)
				if not raw:
					self.running = False
					break
				msg = raw.decode().replace(DELIM, "\\r\\n\n     ")
				self.outSock.sendall(raw)
				self.print(">>>> " + msg)
		except Exception as e:
			self.print("Exception inWorker: " + str(e))
		self.terminate()

	def outWorker(self):
		"""Forward message from server to client"""
		try:
			while self.running:
				raw = self.outSock.recv(BUFF_SIZE)
				if not raw:
					self.running = False
					break
				msg = raw.decode().replace(DELIM, "\\r\\n\n     ")
				self.inSock.sendall(raw)
				self.print("  << " + msg)
		except Exception as e:
			self.print("Exception outWorker: " + str(e))
		self.terminate()

	def print(self, text: str):
		"""Print to the screen"""
		if "PING" not in text and "PONG" not in text:
			print(f"{self.color}{text}{ENDC}")


if __name__ == '__main__':
	server = socket.socket()
	server.bind(IN_ADDR)
	server.listen(42)
	print("Listening on", IN_ADDR)

	try:
		while True:
			client, addr = server.accept()
			clients.append(Client(client, addr))
	except KeyboardInterrupt:
		for c in clients:
			c.terminate()
		server.shutdown(socket.SHUT_RDWR)
