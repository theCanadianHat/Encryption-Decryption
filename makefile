all:
	gcc -Wall encryption.c -o Encryption
	gcc -Wall decryption.c -o Decryption

clean:
	rm Encryption
	rm Decryption
