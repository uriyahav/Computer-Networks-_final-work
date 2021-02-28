how to run the code:
Open the terminal in the folder where the source files are located.
In the terminal use the command 'make'/'make all'- this  command will compile the code files to an object file called , congestion_control.
Run this file with the algorithm you want (edf/bd) , size of queue you want and the txt file you want to run.
Running example:
./congestion edf 10 input_file.txt        (we run edf algorithm  with queue size 10 read input from the file input_file.txt).
output example:
Total arrived packets 1000, total dropped packets 200, total transmitted packets 800, total transmitted
value 2400.

note: this code is running with text file with ' '(space)  between every packet.
**there is already sample input file for you , you can add more as much as you want
