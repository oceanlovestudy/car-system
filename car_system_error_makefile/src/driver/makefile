#项目路径
PROJECT_PATH	 = ./
PROJECT_INC_PATH = ../inc
#最终可执行文件
EXECUTE_FILE = driver_main
#编译参数
CXXFLAGS += -g -Wall -Wextra -pthread -lm
CPPFLAGS +=

all : $(EXECUTE_FILE)
clean : 
	rm -f *.o $(EXECUTE_FILE)

$(EXECUTE_FILE) : $(SRC_O_FILES) $(INC_H_FILES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $(SRC_O_FILES) $^ -o $@

#.c .cpp等文件（包含完整的文件路径）
SRC_C_FILES	:= $(foreach src_path, $(PROJECT_PATH), $(shell find $(src_path) -name '*.c'))
SRC_CPP_FILES	:= $(foreach src_path, $(PROJECT_PATH), $(shell find $(src_path) -name '*.cpp'))
#全部的后缀为.c .cpp的源文件
SRC_FILES := $(SRC_C_FILES)
SRC_FILES += $(SRC_CPP_FILES)
#.h等头文件（包含完整的文件路径）
INC_H_FILES := $(foreach inc_path, $(PROJECT_INC_PATH), $(shell find $(inc_path) -name '*.h'))
#指定头文件的路径 参考shell终端命令：gcc -I ../inc/ test.c -o test
INC_I_FILES := $(addprefix -I, $(sort $(dir $(INC_H_FILES))))
#.o文件
SRC_O_FILES := $(addsuffix .o, $(basename $(notdir $(SRC_FILES))))

#执行shell终端命令
$(foreach src_cpp_file,$(SRC_CPP_FILES),$(shell $(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC_I_FILES) -c $(src_cpp_file) -o $(addsuffix .o,$(basename $(notdir $(src_cpp_file))))))
$(foreach src_c_file,$(SRC_C_FILES),$(shell $(CC) $(CPPFLAGS) $(CXXFLAGS) $(INC_I_FILES) -c $(src_c_file) -o $(addsuffix .o,$(basename $(notdir $(src_c_file))))))


