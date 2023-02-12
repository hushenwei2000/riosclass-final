import os
ins = [
'add.hex',
'addi.hex',
'beq.hex',
'bge.hex',
'blt.hex',
'sll.hex',
'sra.hex'
]
def orig_version():
  for i in ins:
    print(("./model/model.o /work/stu/dyzhang/projects/riosclass-final/test/hex/rv64ui/%s > log1"%(i)))
    os.system(("./model/model.o /work/stu/dyzhang/projects/riosclass-final/test/hex/rv64ui/%s > log1"%(i)))

orig_version()