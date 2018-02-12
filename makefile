all: 

rnn_test: obj/ring_buffer.o obj/matrix_task.o obj/thread_pool.o obj/rnn_test.o obj/RnnLoop.o obj/RnnBranch.o obj/Graph.o obj/VirtualGraph.o obj/Optimizer.o obj/Adadelta.o obj/BranchNode.o obj/LoopNode.o obj/ComputeGraph.o obj/Node.o obj/VirtualNode.o obj/OperatorNode.o obj/Dropout.o obj/Input.o obj/Add.o obj/AbsSum.o obj/Bias.o obj/Mult.o obj/Minus.o obj/SquareSum.o obj/Sigmoid.o obj/Parameter.o obj/RnnInputX.o obj/RnnInputY.o obj/Tensor.o
	g++ -std=c++11 -pthread obj/ring_buffer.o obj/matrix_task.o obj/thread_pool.o obj/rnn_test.o obj/RnnLoop.o obj/RnnBranch.o obj/Graph.o obj/VirtualGraph.o obj/Optimizer.o obj/Adadelta.o obj/BranchNode.o obj/LoopNode.o obj/ComputeGraph.o obj/Node.o obj/VirtualNode.o obj/OperatorNode.o obj/Dropout.o obj/Input.o obj/Add.o obj/AbsSum.o obj/Bias.o obj/Mult.o obj/Minus.o obj/SquareSum.o obj/Sigmoid.o obj/Parameter.o obj/RnnInputX.o obj/RnnInputY.o obj/Tensor.o -o rnn_test

xor_test: obj/ring_buffer.o obj/matrix_task.o obj/thread_pool.o obj/xor_test.o obj/Graph.o obj/VirtualGraph.o obj/ComputeGraph.o obj/Optimizer.o obj/Adadelta.o obj/Node.o obj/VirtualNode.o obj/OperatorNode.o obj/Dropout.o obj/Input.o obj/Add.o obj/Bias.o obj/Mult.o obj/Minus.o obj/SquareSum.o obj/Sigmoid.o obj/AbsSum.o obj/Parameter.o obj/RnnInputX.o obj/RnnInputY.o obj/Tensor.o
	g++ -std=c++11 -pthread obj/ring_buffer.o obj/matrix_task.o obj/thread_pool.o obj/xor_test.o obj/Graph.o obj/VirtualGraph.o obj/ComputeGraph.o obj/Optimizer.o obj/Adadelta.o obj/Node.o obj/VirtualNode.o obj/OperatorNode.o obj/Dropout.o obj/Input.o obj/Add.o obj/Bias.o obj/Mult.o obj/Minus.o obj/SquareSum.o obj/Sigmoid.o obj/AbsSum.o obj/Parameter.o obj/RnnInputX.o obj/RnnInputY.o obj/Tensor.o -o xor_test

graph_test: obj/graph_test.o obj/Graph.o obj/Node.o
	g++ -std=c++11 obj/graph_test.o obj/Graph.o obj/Node.o -o graph_test

tensor_test: obj/tensor_test.o obj/Tensor.o obj/ring_buffer.o obj/matrix_task.o obj/thread_pool.o
	g++ -std=c++11 -pthread obj/tensor_test.o obj/Tensor.o obj/ring_buffer.o obj/matrix_task.o obj/thread_pool.o -o tensor_test

operatorNode_test: obj/operatorNode_test.o obj/ComputeGraph.o obj/Optimizer.o obj/Graph.o obj/Dropout.o obj/Sigmoid.o obj/AbsSum.o obj/SquareSum.o obj/Add.o obj/Bias.o obj/Mult.o obj/Minus.o obj/Parameter.o obj/OperatorNode.o obj/Node.o obj/Tensor.o
	g++ -std=c++11 obj/operatorNode_test.o obj/ComputeGraph.o obj/Optimizer.o obj/Graph.o obj/Dropout.o obj/Sigmoid.o obj/AbsSum.o obj/SquareSum.o obj/Add.o obj/Bias.o obj/Mult.o obj/Minus.o obj/Parameter.o obj/OperatorNode.o obj/Node.o obj/Tensor.o -o operatorNode_test

obj/rnn_test.o: unit_test/rnn/rnn_test.cpp
	g++ -std=c++11 -c unit_test/rnn/rnn_test.cpp -o obj/rnn_test.o
obj/xor_test.o: unit_test/xor_test.cpp
	g++ -std=c++11 -c unit_test/xor_test.cpp -o obj/xor_test.o
obj/operatorNode_test.o: unit_test/operatorNode_test.cpp
	g++ -std=c++11 -c unit_test/operatorNode_test.cpp -o obj/operatorNode_test.o
obj/graph_test.o: unit_test/graph_test.cpp
	g++ -std=c++11 -c unit_test/graph_test.cpp -o obj/graph_test.o
obj/tensor_test.o: unit_test/tensor_test.cpp
	g++ -std=c++11 -c unit_test/tensor_test.cpp -o obj/tensor_test.o

obj/RnnLoop.o: unit_test/rnn/RnnLoop.cpp
	g++ -std=c++11 -c unit_test/rnn/RnnLoop.cpp -o obj/RnnLoop.o
obj/RnnBranch.o: unit_test/rnn/RnnBranch.cpp
	g++ -std=c++11 -c unit_test/rnn/RnnBranch.cpp -o obj/RnnBranch.o

obj/SquareSum.o: src/op_node/SquareSum.cpp
	g++ -std=c++11 -c src/op_node/SquareSum.cpp -o obj/SquareSum.o
obj/Sigmoid.o: src/op_node/Sigmoid.cpp
	g++ -std=c++11 -c src/op_node/Sigmoid.cpp -o obj/Sigmoid.o
obj/Mult.o: src/op_node/Mult.cpp
	g++ -std=c++11 -c src/op_node/Mult.cpp -o obj/Mult.o
obj/Minus.o: src/op_node/Minus.cpp
	g++ -std=c++11 -c src/op_node/Minus.cpp -o obj/Minus.o
obj/Add.o: src/op_node/Add.cpp
	g++ -std=c++11 -c src/op_node/Add.cpp -o obj/Add.o
obj/Bias.o: src/op_node/Bias.cpp
	g++ -std=c++11 -c src/op_node/Bias.cpp -o obj/Bias.o
obj/Input.o: src/op_node/Input.cpp
	g++ -std=c++11 -c src/op_node/Input.cpp -o obj/Input.o
obj/Parameter.o: src/op_node/Parameter.cpp
	g++ -std=c++11 -c src/op_node/Parameter.cpp -o obj/Parameter.o
obj/RnnInputX.o: src/op_node/RnnInputX.cpp
	g++ -std=c++11 -c src/op_node/RnnInputX.cpp -o obj/RnnInputX.o
obj/RnnInputY.o: src/op_node/RnnInputY.cpp
	g++ -std=c++11 -c src/op_node/RnnInputY.cpp -o obj/RnnInputY.o
obj/AbsSum.o: src/op_node/AbsSum.cpp
	g++ -std=c++11 -c src/op_node/AbsSum.cpp -o obj/AbsSum.o
obj/Dropout.o: src/op_node/Dropout.cpp
	g++ -std=c++11 -c src/op_node/Dropout.cpp -o obj/Dropout.o

obj/ComputeGraph.o: src/ComputeGraph.cpp
	g++ -std=c++11 -c src/ComputeGraph.cpp -o obj/ComputeGraph.o
obj/VirtualGraph.o: src/VirtualGraph.cpp
	g++ -std=c++11 -c src/VirtualGraph.cpp -o obj/VirtualGraph.o
obj/Graph.o: src/Graph.cpp
	g++ -std=c++11 -c src/Graph.cpp -o obj/Graph.o

obj/OperatorNode.o: src/OperatorNode.cpp 
	g++ -std=c++11 -c src/OperatorNode.cpp -o obj/OperatorNode.o
obj/VirtualNode.o: src/VirtualNode.cpp 
	g++ -std=c++11 -c src/VirtualNode.cpp -o obj/VirtualNode.o
obj/LoopNode.o: src/LoopNode.cpp
	g++ -std=c++11 -c src/LoopNode.cpp -o obj/LoopNode.o
obj/BranchNode.o: src/BranchNode.cpp
	g++ -std=c++11 -c src/BranchNode.cpp -o obj/BranchNode.o
obj/Node.o: src/Node.cpp
	g++ -std=c++11 -c src/Node.cpp -o obj/Node.o

obj/Optimizer.o: src/Optimizer.cpp
	g++ -std=c++11 -c src/Optimizer.cpp -o obj/Optimizer.o
obj/Adadelta.o: src/optimizer/Adadelta.cpp
	g++ -std=c++11 -c src/optimizer/Adadelta.cpp -o obj/Adadelta.o

obj/Tensor.o: src/Tensor.cpp
	g++ -std=c++11 -c src/Tensor.cpp -o obj/Tensor.o

obj/ring_buffer.o: src/multi_thread/ring_buffer.cpp
	g++ -std=c++11 -c src/multi_thread/ring_buffer.cpp -o obj/ring_buffer.o
obj/thread_pool.o: src/multi_thread/thread_pool.cpp
	g++ -std=c++11 -c src/multi_thread/thread_pool.cpp -o obj/thread_pool.o
obj/matrix_task.o: src/multi_thread/matrix_task.cpp
	g++ -std=c++11 -c src/multi_thread/matrix_task.cpp -o obj/matrix_task.o

clean:
	rm obj/*o 
