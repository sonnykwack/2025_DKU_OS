/*
*	DKU Operating System Lab
*	    Lab2 (Concurrent Data Structure : Skip List)
*	    Student id : 32210214   
*	    Student name : 곽태훈
*	    Date : 2025-05-12
*/

#include "skiplist_impl.h"
#include <cstdlib>
#include <ctime>

// DefaultSkipList 생성자
DefaultSkipList::DefaultSkipList(int max_level, float prob) {
    this->max_level_ = max_level;
    this->prob_ = prob;
    
    header_ = new Node();
    header_->key = -1;  
    header_->value = -1;
    header_->upd_cnt = 0;
    header_->level = max_level_;
    
    header_->forward = new Node*[max_level_ + 1];
    for (int i = 0; i <= max_level_; i++) {
        header_->forward[i] = nullptr;
    }
    
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
            level < max_level_) {
        level++;
    }
    return level;
}

// SkipList 생성자
SkipList::SkipList(int max_level, float prob) : DefaultSkipList(max_level, prob) {}

// SkipList 소멸자
SkipList::~SkipList() {}

void SkipList::insert(int key, int value) {
    //TODO
    Node* update[max_level_ + 1];
    Node* current = header_;

    // 상위 레벨부터 내려오며 삽입 위치 찾기
    for (int i = max_level_; i >= 0; --i) {
        while (current->forward[i] != nullptr && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    // 이미 존재하는 키: 값 누적 + 업데이트 횟수 증가
    if (current != nullptr && current->key == key) {
        current->value += value;
        current->upd_cnt += 1;
    } else {
        int new_level = random_level();

        Node* new_node = new Node();
        new_node->key = key;
        new_node->value = value;
        new_node->upd_cnt = 0;
        new_node->level = new_level;
        new_node->forward = new Node*[new_level + 1];

        for (int i = 0; i <= new_level; ++i) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
    }
}

int SkipList::lookup(int key) {
    // TODO
    Node* current = header_;

    // 상위 레벨부터 내려오며 키 탐색
    for (int i = max_level_; i >= 0; --i) {
        while (current->forward[i] != nullptr && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current != nullptr && current->key == key) {
        return current->value;
    } else {
        return 0;
    }
}

void SkipList::remove(int key) {
    // TODO
     Node* update[max_level_ + 1];
    Node* current = header_;

    // 삭제할 위치 탐색
    for (int i = max_level_; i >= 0; --i) {
        while (current->forward[i] != nullptr && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current != nullptr && current->key == key) {
        for (int i = 0; i <= current->level; ++i) {
            if (update[i]->forward[i] != current) break;
            update[i]->forward[i] = current->forward[i];
        }

        delete[] current->forward;
        delete current;
    }
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
    // TODO
     pthread_mutex_lock(&mutex_lock);

    Node* update[max_level_ + 1];
    Node* current = header_;

    for (int i = max_level_; i >= 0; --i) {
        while (current->forward[i] != nullptr && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current != nullptr && current->key == key) {
        current->value += value;
        current->upd_cnt += 1;
    } else {
        int new_level = random_level();

        Node* new_node = new Node();
        new_node->key = key;
        new_node->value = value;
        new_node->upd_cnt = 0;
        new_node->level = new_level;
        new_node->forward = new Node*[new_level + 1];

        for (int i = 0; i <= new_level; ++i) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
    }

    pthread_mutex_unlock(&mutex_lock);
}

int CoarseSkipList::lookup(int key) {
    // TODO
    pthread_mutex_lock(&mutex_lock);

    Node* current = header_;

    for (int i = max_level_; i >= 0; --i) {
        while (current->forward[i] != nullptr && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    int result = (current != nullptr && current->key == key) ? current->value : 0;

    pthread_mutex_unlock(&mutex_lock);
    return result;
}

void CoarseSkipList::remove(int key) {
    // TODO
    pthread_mutex_lock(&mutex_lock);

    Node* update[max_level_ + 1];
    Node* current = header_;

    for (int i = max_level_; i >= 0; --i) {
        while (current->forward[i] != nullptr && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current != nullptr && current->key == key) {
        for (int i = 0; i <= current->level; ++i) {
            if (update[i]->forward[i] != current) break;
            update[i]->forward[i] = current->forward[i];
        }
        delete[] current->forward;
        delete current;
    }

    pthread_mutex_unlock(&mutex_lock);
}

// FineSkipList 생성자
FineSkipList::FineSkipList(int max_level, float prob) : DefaultSkipList(max_level, prob) {
    delete header_;
    header_ = new Node();
    header_->key = -1;
    header_->value = -1;
    header_->upd_cnt = 0;
    header_->level = max_level_;

    header_->forward = new Node*[max_level_ + 1];
    for (int level = 0; level <= max_level_; level++) {
        header_->forward[level] = nullptr;
    }

    pthread_mutex_init(&header_->lock, nullptr);
}

// FineSkipList 소멸자
FineSkipList::~FineSkipList() {
    Node* current = header_;
    Node* temp;

    while (current != nullptr) {
        temp = current->forward[0];
        pthread_mutex_destroy(&current->lock);
        delete[] current->forward;
        delete current;
        current = temp;
    }

    header_ = nullptr;
}

// FineSkipList::insert
void FineSkipList::insert(int key, int value) {
    Node* update[max_level_ + 1];
    Node* current = header_;

    pthread_mutex_lock(&current->lock);

    for (int i = max_level_; i >= 0; --i) {
        Node* next = current->forward[i];
        while (next && next->key < key) {
            pthread_mutex_lock(&next->lock);
            pthread_mutex_unlock(&current->lock);
            current = next;
            next = current->forward[i];
        }
        update[i] = current;
    }

    Node* target = current->forward[0];
    if (target) pthread_mutex_lock(&target->lock);

    if (target && target->key == key) {
        target->value += value;
        target->upd_cnt += 1;
        pthread_mutex_unlock(&target->lock);
    } else {
        int new_level = random_level();
        Node* new_node = new Node();
        new_node->key = key;
        new_node->value = value;
        new_node->upd_cnt = 0;
        new_node->level = new_level;
        new_node->forward = new Node*[new_level + 1];

        pthread_mutex_init(&new_node->lock, nullptr);

        for (int i = 0; i <= new_level; ++i) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
    }

    if (target) pthread_mutex_unlock(&target->lock);
    for (int i = 0; i <= max_level_; ++i)
        pthread_mutex_unlock(&update[i]->lock);

}

// FineSkipList::lookup
int FineSkipList::lookup(int key) {
        Node* current = header_;
    pthread_mutex_lock(&current->lock);

    for (int i = max_level_; i >= 0; --i) {
        Node* next = current->forward[i];
        while (next && next->key < key) {
            pthread_mutex_lock(&next->lock);
            pthread_mutex_unlock(&current->lock);
            current = next;
            next = current->forward[i];
        }
    }

    Node* target = current->forward[0];
    int result = 0;

    if (target) {
        pthread_mutex_lock(&target->lock);
        if (target->key == key) result = target->value;
        pthread_mutex_unlock(&target->lock);
    }

    pthread_mutex_unlock(&current->lock);
    return result;
}

// FineSkipList::remove
void FineSkipList::remove(int key) {
    Node* update[max_level_ + 1];
    Node* current = header_;
    pthread_mutex_lock(&current->lock);

    for (int i = max_level_; i >= 0; --i) {
        Node* next = current->forward[i];
        while (next && next->key < key) {
            pthread_mutex_lock(&next->lock);
            pthread_mutex_unlock(&current->lock);
            current = next;
            next = current->forward[i];
        }
        update[i] = current;
    }

    Node* target = current->forward[0];
    if (target) pthread_mutex_lock(&target->lock);

    if (target && target->key == key) {
        for (int i = 0; i <= target->level; ++i) {
            if (update[i]->forward[i] == target)
                update[i]->forward[i] = target->forward[i];
        }

        pthread_mutex_unlock(&target->lock);
        pthread_mutex_destroy(&target->lock);
        delete[] target->forward;
        delete target;
    } else {
        if (target) pthread_mutex_unlock(&target->lock);
    }

    for (int i = 0; i <= max_level_; ++i)
        pthread_mutex_unlock(&update[i]->lock);
}
