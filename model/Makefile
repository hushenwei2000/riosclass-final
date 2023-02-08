Utils: Utils.hpp Utils.cpp
	g++ -c Utils.cpp -I "./"
PRegFile: PRegFile.hpp PRegFile.cpp
	g++ -c PRegFile.cpp -I "./"
Memory: Memory.hpp Memory.cpp
	g++ -c Memory.cpp -I "./"
Decode: Decode.hpp Decode.cpp
	g++ -c Decode.cpp -I "./"
Rename: Rename.hpp Rename.cpp
	g++ -c Rename.cpp -I "./"
GShare: GShare.hpp GShare.cpp
	g++ -c GShare.cpp -I "./"
BTB: BTB.hpp BTB.cpp GShare.o
	g++ -c BTB.cpp -I "./"
ROB: ROB.hpp ROB.cpp Utils.hpp
	g++ -c ROB.cpp -I "./"
InsBuffer: InsBuffer.hpp InsBuffer.cpp
	g++ -c InsBuffer.cpp -I "./"
Cache: Cache.hpp Cache.cpp
	g++ -c Cache.cpp -I "./"
ICache: Cache.hpp Cache.cpp ICache.hpp ICache.cpp
	g++ -c ICache.cpp -I "./"
Issue: Issue.hpp Issue.cpp PRegFile.hpp Rename.hpp Utils.hpp ROB.hpp ALU.hpp LSU.hpp CSR.hpp
	g++ -c Issue.cpp -I "./"
ALU: ALU.hpp ALU.cpp PRegFile.hpp Rename.hpp Utils.hpp
	g++ -c ALU.cpp -I "./"
LSU: LSU.hpp LSU.cpp PRegFile.hpp Rename.hpp Utils.hpp Memory.hpp
	g++ -c LSU.cpp -I "./"
CSR: CSR.hpp CSR.cpp PRegFile.hpp Rename.hpp Utils.hpp
	g++ -c CSR.cpp -I "./"
main: Utils PRegFile Memory Cache ICache Decode Rename BTB GShare ROB InsBuffer Issue ALU LSU CSR
	g++ -c main.cpp Utils.cpp PRegFile.cpp Memory.cpp Cache.cpp ICache.cpp Decode.cpp BTB.cpp GShare.cpp ROB.cpp InsBuffer.cpp Issue.cpp ALU.cpp LSU.cpp CSR.cpp  -I "./"
model: main Utils PRegFile Memory Cache ICache Decode Rename BTB GShare ROB InsBuffer Issue ALU LSU CSR
	g++ Utils.o PRegFile.o Memory.o Cache.o ICache.o Decode.o Rename.o BTB.o GShare.o ROB.o InsBuffer.o Issue.o ALU.o LSU.o CSR.o main.o  -I "./" -o model.o
	./model.o > log
clean: 
	rm -rf *.o *.out