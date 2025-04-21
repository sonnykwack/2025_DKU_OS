/*
*	DKU Operating System Lab
*	    Lab2 (Concurrent Data Structure : SkipList)
*	    Student id : 
*	    Student name : 
*	    Date : 
*/


#include "skiplist_impl.h"
#include <cstdlib>
#include <ctime>
#include <cstring>

// DefaultSkipList 생성자
DefaultSkipList::DefaultSkipList(int max_level, float prob) {
    this->max_level_ = max_level;
    this->prob_ = prob;
    this->current_level_ = 0;
    
    // 헤더 노드 생성
    header_ = new Node();
    header_->key = -1;  // 가장 작은 키 값
    header_->value = -1;
    header_->upd_cnt = 0;
    header_->level = max_level_;
    
    // 헤더 노드의 forward 배열 초기화
    header_->forward = new Node*[max_level_ + 1];
    for (int i = 0; i <= max_level_; i++) {
        header_->forward[i] = nullptr;
    }
    
    // 난수 생성을 위한 시드 설정
    srand(time(nullptr));
}

// DefaultSkipList 소멸자
DefaultSkipList::~DefaultSkipList() {
    Node* current = header_;
    Node* temp;
    
    while(current) {
        temp = current->forward[0];
        delete[] current->forward;
        delete current;
        current = temp;
    }
}

// 랜덤 레벨 생성 함수
int DefaultSkipList::random_level() {
    int level = 0;
    while (((double)rand() / RAND_MAX) < prob_ && 
            level < max_level_ && 
            level <= current_level_ + 1) {  // 현재 최대 레벨보다 최대 1단계만 높게
        level++;
    }
    return level;
}

// SkipList 생성자
SkipList::SkipList(int max_level, float prob) : DefaultSkipList(max_level, prob) {}

// SkipList 소멸자
SkipList::~SkipList() {}

void SkipList::insert(int key, int value) {
    // Todo
}

int SkipList::lookup(int key) {
    // Todo
}

void SkipList::remove(int key) {
    // Todo
}

// CoarseSkipList 생성자
CoarseSkipList::CoarseSkipList(int max_level, float prob) : DefaultSkipList(max_level, prob) {
    pthread_mutex_init(&mutex_lock, nullptr);
}

// CoarseSkipList 소멸자
CoarseSkipList::~CoarseSkipList() {
    pthread_mutex_destroy(&mutex_lock);
}

void CoarseSkipList::insert(int key, int value) {
    // Todo
}

int CoarseSkipList::lookup(int key) {
    // Todo
}

void CoarseSkipList::remove(int key) {
    // Todo
}

// FineSkipList 생성자
FineSkipList::FineSkipList(int max_level, float prob) : DefaultSkipList(max_level, prob) {
    // 헤더 노드를 FineNode로 대체
    delete header_;
    header_ = new FineNode();
    header_->key = -1;
    header_->value = -1;
    header_->upd_cnt = 0;
    header_->level = max_level_;
    
    header_->forward = new Node*[max_level_ + 1];
    for (int i = 0; i <= max_level_; i++) {
        header_->forward[i] = nullptr;
    }
    
    pthread_mutex_init(&((FineNode*)header_)->lock, nullptr);
}

// FineSkipList 소멸자
FineSkipList::~FineSkipList() {
    Node* current = header_;
    Node* temp;
    
    while (current) {
        temp = current->forward[0];
        pthread_mutex_destroy(&((FineNode*)current)->lock);
        delete[] current->forward;
        delete current;
        current = temp;
    }
}

void FineSkipList::insert(int key, int value) {
    // Todo
}

int FineSkipList::lookup(int key) {
    // Todo
}

void FineSkipList::remove(int key) {
    // Todo
}
