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
  if (node == nullptr) {
      return;
  }
  printf("Insert %d Start\n", node->key_);
  Node* prev = head;
  Node* current = head->next_;
  while (current != nullptr) {
      prev->mutex_.lock();
      current->mutex_.lock();
      if (node->key_ < current->key_ && node->key_ != prev->key_){
          prev->next_ = node;
          node->next_ = current;
          printf("Insert %d End\n", node->key_);
          current->mutex_.unlock();
          prev->mutex_.unlock();
          return;
      } else if(node->key_ == prev->key_){
          current->mutex_.unlock();
          prev->mutex_.unlock();
          printf("duplicated key%d!\n",node->key_);
          return;
      }else{
          prev->mutex_.unlock();
          current->mutex_.unlock();
          prev = current;
          current = current->next_;
      };
  }
  prev->mutex_.lock();
  printf("Insert %d End\n", node->key_);
  //prev->mutex_.unlock();
  prev->next_ = node;
  prev->mutex_.unlock();

}

//find a key
void Contains(Node* head,int ckey){
  Node* prev = head;
  Node* current = head->next_;
  while(current != nullptr && current->key_ <= ckey ){
    prev->mutex_.lock();
    current->mutex_.lock();
    if(current->key_ < ckey){
      current->mutex_.unlock();
      prev->mutex_.unlock();
      prev = current;
      current = current->next_;
    }
    else{
      printf("%d:%d\n",current->key_,current->value_);
      current->mutex_.unlock();
      prev->mutex_.unlock();
      return;
    }
  }
  printf("NOT FOUND!\n");
  return;

}

//remove a node
void Remove (Node* head, int dkey){
  Node* prev = head;
  Node* current = head->next_;
  printf("Remove %d starts\n", dkey);
  while(current != nullptr && current->key_ <= dkey ){
    prev->mutex_.lock();
    current->mutex_.lock();
    if(current->key_ < dkey){
      current->mutex_.unlock();
      prev->mutex_.unlock();
      prev = current;
      current = current->next_;
    }
    else{
      //find the dkey:dvalue
      current->mutex_.unlock();
      prev->mutex_.unlock();
      prev->next_ = current->next_;
      current->next_ = current->next_->next_;
      printf("Remove %d ends\n", dkey);
      return;
    }
  }
  current->mutex_.unlock();
  prev->mutex_.unlock();
  printf("NOT FOUND THIS %d!\n",dkey);
  printf("Remove %d ends\n", dkey);
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

  //test for RemoveThreads
  vector<thread> RemoveThreads;
  for(int i = 0; i < 1000; i = i + 2){
    auto node1 = new Node(i, i + 101);
    RemoveThreads.emplace_back(Add, head, node1);
  }
  for (auto& thread : RemoveThreads) {
    thread.join();
  }

//  //test for Add + Remove Thread
//  vector<thread> ARThreads;
//  for(int i = 1000; i < 2000; i = i ++){
//    auto node1 = new Node(i+1,i+100);
//    ARThreads.emplace_back(ARTest,head,node1,i);
//  }
//  for (auto& thread : ARThreads) {
//    thread.join();
//  }

  //test for Remove + Add Thread
//  vector<thread> RAThreads;
//  for(int i = 1000; i < 2000; i = i + 2){
//    auto node2 = new Node(i+1,i+100);
//    RAThreads.emplace_back(RATest,head,node2,i);
//  }
//  for (auto& thread2 : RAThreads) {
//    thread2.join();
//  }
  Print(head);
  return 0;
}
