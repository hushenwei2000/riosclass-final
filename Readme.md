# Steps
1. build tests  
```
cd test-new
make build
```

2. 更改要测试的文件 `rtl/makefile` 
```c
// line 18
HEX_FILE := "\""$(HEX_DIR)/add.hex"\"" // add -> 要测的指令
```

3. Run RTL  
```
cd rtl
make cosim
```
查看以 cosim- 开头的性能指标结果

4. Run Model
```
cd model
make model
```
查看以 cosim- 开头的性能指标结果