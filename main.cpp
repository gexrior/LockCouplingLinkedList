#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

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
      if (node->key_ < current->key_) {
          prev->next_ = node;
          node->next_ = current;
          printf("Insert %d End\n", node->key_);
          current->mutex_.unlock();
          prev->mutex_.unlock();
          return;
      } else {
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

Node* Remove (Node* head, int dkey){
  Node* prev = head;
  Node* current = head->next_;
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
      return head;
    }
  }
  printf("NOT FOUND THIS dkey!\n");
  return head;
}

void Print(Node* head) {
  head->mutex_.lock();
  cout << "----------PRINT----------" << endl;
  for (Node* current = head->next_; current != nullptr; current = current->next_) {
      cout << current->key_ << ":" << current->value_<<endl;
  }
  head->mutex_.unlock();
  cout << endl;
}

int main(){
  auto head = new Node(0, 0);
  vector<thread> threads;
  for (int i = 0; i < 5; ++i) {
      auto node = new Node(i, i + 100);
      threads.emplace_back(Add, head, node);
  }
  for (auto& thread : threads) {
      thread.join();
  }
  //Contains(head,15);
  Print(head);
  //Remove(head,3);
  //Print(head);
  return 0;
}