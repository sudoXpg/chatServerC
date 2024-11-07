all:
	@gcc chat.c -o chat
	@echo connect from other terminals on port 9034
	@./chat
	@echo

clean:
	rm chat