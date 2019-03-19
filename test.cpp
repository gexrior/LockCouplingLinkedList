#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <fstream>


using namespace std;


class Node {
public:
  Node(int key, int value) {
    key_ = key;
    value_ = value;
    next_ = nullptr;
  }

  ~Node() {
    delete(next_);
  }

  int key_;
  int value_;
  Node* next_;
  std::mutex mutex_;
};

//add a node
void Add(Node* head, Node* node) {

}

//find a key
void Contains(Node* head,int ckey){

}

//remove a node
Node* Remove (Node* head, int dkey){

}

//print this LinkedList
void Print(Node* head) {
  head->mutex_.lock();
  ofstream fout("result.txt");
  fout << "----------PRINT----------" << endl;
  for (Node* current = head->next_; current != nullptr; current = current->next_) {
    fout << current->key_ << ":" << current->value_<<endl;
  }
  head->mutex_.unlock();
  fout << endl;
}

//test for add + remove thread
void ARTest(Node* head,Node* node,int dlkey){
  Add(head,node);
  Remove(head,dlkey);
}

//test for Remove + Add Thread
void RATest(Node* head,Node* node,int dlkey){
  Remove(head,dlkey);
  Add(head,node);
}

int main(){
  auto head = new Node(0, 0);

  //test for AddThreads
  vector<thread> AddThreads;
  for (int i = 0; i < 1000; ++i) {
    auto node = new Node(i, i + 100);
    AddThreads.emplace_back(Add, head, node);
  }
  for (auto& thread1 : AddThreads) {
    thread1.join();
  }

//  //test for RemoveThreads
//  vector<thread> RemoveThreads;
//  for(int i = 0; i < 1000; i = i + 2){
//    RemoveThreads.emplace_back(Remove, head, i);
//  }
//  for (auto& thread : RemoveThreads) {
//    thread.join();
//  }

//  //test for Add + Remove Thread
//  vector<thread> ARThreads;
//  for(int i = 0; i < 1000; i = i + 2){
//    auto node1 = new Node(i+1,i+100);
//    ARThreads.emplace_back(ARTest,head,node1,i);
//  }
//  for (auto& thread : ARThreads) {
//    thread.join();
//  }

  //test for Remove + Add Thread
  vector<thread> RAThreads;
  for(int i = 0; i < 1000; i = i + 2){
    auto node2 = new Node(i+1,i+100);
    RAThreads.emplace_back(RATest,head,node2,i+1);
  }
  for (auto& thread2 : RAThreads) {
    thread2.join();
  }
  Print(head);
  return 0;
}