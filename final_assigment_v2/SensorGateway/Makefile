# Biến để đơn giản hóa việc sử dụng các thư mục
INCLUDE_DIR = -IInc
LIB_DIR = -LLib
CFLAGS = -Wall -ggdb3 $(INCLUDE_DIR)
LDFLAGS = -pthread -lmysqlcppconn

# Target build tất cả
all: ch pr

# Build chương trình childproc
ch:
	g++ Src/childproc.cpp $(CFLAGS) $(LIB_DIR) -lLogProcess -o ch

# Build chương trình parentproc
pr:
	g++ Src/parentproc.cpp $(CFLAGS) $(LIB_DIR) -lMyLibrary $(LDFLAGS) -o pr

# Target để dọn dẹp file tạm
clean:
	rm -rf ch pr myfifo
