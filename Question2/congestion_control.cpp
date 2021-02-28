#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <queue>
#include <algorithm>

using namespace std;
typedef pair<int, int> pi;

int arrived =0;
int dropped =0;
int transmitted = 0;
int myValue = 0;
void transmitMinSlack(priority_queue<pi, vector<pi>, greater<pi> >&queue){
     if(!queue.empty()){
            pair<int,int> minSlack = queue.top();
            queue.pop();
            myValue += minSlack.second;
            transmitted++;
        }
}
void myClear(priority_queue<pi, vector<pi>, greater<pi> >&queue){
while(!queue.empty()){
    queue.pop();
    }
}
void updateSlack(priority_queue<pi, vector<pi>, greater<pi> > &queue){
            priority_queue<pi, vector<pi>, greater<pi> > helpQueue;
        while(!queue.empty()){
            pair<int,int> p= queue.top();
            queue.pop();
            p.first = p.first-1;
            //if slack after update>0 put in queue
            if(p.first>0){ 
                helpQueue.push(p);
            }
            //if slack=0 drop the packet
            else dropped++; 
        }
        queue=helpQueue;
        myClear(helpQueue);
}

void EDF(priority_queue<pi, vector<pi>, greater<pi> > queue, int size, string file){
    //read from file
    ifstream readFromFile(file);
    string line;
    //real 1 line at a time from given file and put it in string called line 
    //the getline works untill end of file
    while (getline (readFromFile, line)) {
        istringstream iss(line);
        string tuple;
        //get a tuple at a time
        while (iss>> tuple){
            //remove '(' ')' and split every number in the tuple
            string sub_tuple = tuple.substr(1,tuple.size()-2);
             //replace ',' in ' ' (space) in sub_tuple
            replace(sub_tuple.begin(), sub_tuple.end(), ',', ' ');
            stringstream split(sub_tuple);
            int amount; //first number
            int slack; //second number
            int value; // third number

            //put the parameters in amount slack value (at this order)
            split >> amount >> slack >> value;
            
            //update total arrived packets
            arrived += amount;
            //if slack and value are good
            if(slack > 0 && value > 0){
                //if there is space in the queue - add packets
                while (queue.size() < size && amount > 0){
                    pair<int,int> p(slack, value);
                    queue.push(p);
                    amount--;
                }  
             
                pair<int, int> check = queue.top();
                //if the new packets have a small salck then the smallest in the queue - drop the new packets
                while(check.first >= slack && amount >0) {
                    dropped++; 
                    amount--;
                }
                //if there are more packets 
                // replace with the packet with minimun slack in the queue 
                while(check.first< slack && amount>0){   
                    //remove the packet with the minimum slack and push the new packet
                    queue.pop();
                    pair<int,int> newPacket(slack, value);
                    queue.push(newPacket);
                    amount--;
                    check = queue.top();
                    dropped++;
                }  
                //if we have more packet tht can't add to queue drop them
                while(amount >0 ){
                    dropped++;
                    amount--;
                }
            }
                        //if the packets have wrong input dropped them
            else{
                while (amount >0){
                    dropped++;
                    amount--;
                }
            }
        }
                //update the slack of every packet to be slack-1 
        updateSlack(queue);
                //processing phase//
        //transmit the packet with the minimum slack
        transmitMinSlack(queue);
    }

     // countinue the processing until the queue is empty
    while(!queue.empty()){
        //update the slack of every packet to be slack-1 
        updateSlack(queue);
        //transmit the packet with the minimum slack
        transmitMinSlack(queue);
    }
    printf("Total arrived packets %d, total dropped packets %d, total transmitted packets %d, total transmitted value %d\n", arrived, dropped, transmitted, myValue);
}
///end of edf///

void bdUpdateSlack(priority_queue<pi, vector<pi>, greater<pi> > &queue){
            priority_queue<pi, vector<pi>, greater<pi> > helpQueue;
        while(!queue.empty()){
            pair<int,int> p= queue.top();
            queue.pop();
            p.second = p.second-1;
            //if slack after update>0 put in queue
            if(p.second>0){ 
                helpQueue.push(p);
            }
            //if slack=0 drop the packet
            else dropped++; 
        }
        queue=helpQueue;
        myClear(helpQueue);
}
void transmitMaxVal(priority_queue<pi, vector<pi>, greater<pi> > &queue){
    priority_queue<pi, vector<pi>, greater<pi> > help_queue2;
  //put in queue all variabeles except of the last one in the queue
        while(queue.size() > 1){
            help_queue2.push(queue.top());
            queue.pop();
        }
        if(!queue.empty()){
            pair<int,int> maxVal = queue.top();
            queue.pop();
            myValue += maxVal.first;
            transmitted++;
            queue = help_queue2;
         //   myClear(help_queue2);
         }
         myClear(help_queue2);
      }

    void bounded_Delay(priority_queue<pi, vector<pi>, greater<pi> > queue , int size, string file ){
    string line;
    ifstream readFromFile(file);
    //real 1 line at a time from given file and put it in string called line 
    //the getline works untill end of file
    while (getline (readFromFile, line)) {
        //copy  line to iss
        istringstream iss(line);
        string tuple;
        //get one tuple at a time
        while (iss>> tuple){
            //remove '(' ')' and split every number in the tuple
            string sub_tuple = tuple.substr(1,tuple.size()-2);
            //replace ',' in ' ' (space) in sub_tuple
            replace(sub_tuple.begin(), sub_tuple.end(), ',', ' ');
           //split sub_tuple by ' ' (spaces) 
            stringstream split(sub_tuple);
            int amount; //first number
            int slack; //second number
            int value; // third number
        
           //put the parameters in amount slack value (at this order)
            split >> amount >> slack >> value;
            
            //update  total arrived packets
            arrived += amount;
            //if slack and value are good
            if(slack > 0 && value >0){
                //if there is space in the queue - add packets
                while (queue.size() < size && amount > 0){
                    pair<int,int> packet(value  , slack);
                    queue.push(packet);
                    amount--;
                }

                pair<int, int> check = queue.top();
                //if the new packets have a small value then thee smallest in the queue - drop the new packets
                while(check.first >= value && amount >0) {
                    dropped++; 
                    amount--;
                }
                /*if there is stil more packets to push to the queue
                so replace with the packet with minimun value*/  
                while(check.first< value && amount>0){   
                    //remove the packet with the minimum value and push the new packet
                    queue.pop();
                    pair<int,int> packet(value, slack);
                    queue.push(packet);
                    amount--;
                    check = queue.top();
                    dropped++;
                }  
                //if we have more packet we didn't  add to the queue - drop them
                while(amount >0 ){
                    dropped++;
                    amount--;
                }
            }
            //if the packets have wrong input dropped them
            else{
                while (amount >0){
                    dropped++;
                    amount--;
                }
            }
        }

         //update the slack of every packet to be slack-1 by using help queue
        bdUpdateSlack(queue);
                     //processing phase//
        //transmit the packet with the maximum value
        transmitMaxVal(queue);
    
 }
    // countinue the processing until the queue is empty
    while(!queue.empty()){
         //update the slack of every packet to be slack-1 by using help queue
       bdUpdateSlack(queue);
                         //processing phase//
        //transmit the packet with the maximum value
       transmitMaxVal(queue);
    }  
    printf("Total arrived packets %d, total dropped packets %d, total transmitted packets %d, total transmitted value %d\n", arrived, dropped, transmitted, myValue);

    }
       
int main(int argc, char** argv){
     // By default a min heap(represented as a priority queue) is created ordered 
    // by first element of pair.  
    priority_queue<pi, vector<pi>, greater<pi> > queue;
    //read the policy from the terminal
    string policy = argv[1]; 
    //read the size of queue from the terminal converted to int
    int size = atoi(argv[2]);
    //read which file read from - get it from the terminal
    string file = argv[3];
    
    if(policy == "bd") bounded_Delay(queue, size, file);
    if(policy == "edf") EDF(queue, size, file);
}
