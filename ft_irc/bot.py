import socket
import requests


LISTEN_ADDR = ('0.0.0.0', 33333)
BUFFSIZE = 4096
DELIM = '\r\n'

def doCALC(expr: str):
	expr = expr.replace("^", "**")
	try:
		res = eval(expr)
	except Exception as e:
		return "ERROR " + str(e)
	return str(res)

def doQUOTE():
	req = requests.get("https://goquotes-api.herokuapp.com/api/v1/random?count=1").json()
	return req["quotes"][0]["text"]

if __name__ == '__main__':
	
	s = socket.socket()
	s.bind(LISTEN_ADDR)
	s.listen(5)
	print("Script listening for bot on", LISTEN_ADDR)

	c = None
	try:
		while True:
			c, addr = s.accept()
			print("Got one from", addr)
			
			# Receiving command
			cmd = ""
			while True:
				r = c.recv(BUFFSIZE)
				cmd += r.decode()
				if not r or cmd.endswith(DELIM):
					break
			if not cmd.endswith(DELIM):
				c.close()
				c = None
				continue
		
			if cmd.startswith("CALC "):
				resp = doCALC(cmd[len("CALC "):])
			elif cmd.startswith("QUOTE"):
				resp = doQUOTE()
			else:
				resp = "ERROR Command not found"

			# Send back a response
			c.send((resp + DELIM).encode())

			c.close()
			c = None
	except:
		if c is not None:
			c.shutdown(socket.SHUT_RDWR)
		s.shutdown(socket.SHUT_RDWR)
