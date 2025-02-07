// CMSC 341 - Fall 2024 - Project 4
#include "filesys.h"
FileSys::FileSys(int size, hash_fn hash, prob_t probing = DEFPOLCY){
    // Check that size is prime and within range
    if (size >= MINPRIME && size <= MAXPRIME && isPrime(size)){
        m_currentCap = size;
    }
    else{
        m_currentCap = findNextPrime(size);
    }
    // Allocate memory and initialize to null
    m_currentTable = new File*[m_currentCap];
    for (int i = 0; i < m_currentCap; i++){
        m_currentTable[i] = nullptr;
    }

    // Set rest of member variables to default values
    m_hash = hash;
    m_currProbing = probing;
    m_newPolicy = probing;
    m_currentSize = 0;
    m_currNumDeleted = 0;
    m_oldTable = nullptr;
    m_oldCap = 0;
    m_oldSize = 0;
    m_transferIndex = 0;
}

FileSys::~FileSys(){
    // Deallocate memory from current table if exists
    if (m_currentTable != nullptr){
        for (int i = 0; i < m_currentCap; i++){
            if (m_currentTable[i] != nullptr){
                delete m_currentTable[i];
                m_currentTable[i] = nullptr;
            }
        }
        delete [] m_currentTable;
        m_currentTable = nullptr;
    }

    // Deallocate memory from old table if exists
    if (m_oldTable != nullptr){
        for (int i = 0; i < m_oldCap; i++){
            if (m_oldTable[i] != nullptr){
                delete m_oldTable[i];
                m_oldTable[i] = nullptr;
            }
        }
        delete [] m_oldTable;
        m_oldTable = nullptr;
    }
}

void FileSys::changeProbPolicy(prob_t policy){
    m_newPolicy = policy;
}

bool FileSys::insert(File file){
    hash_fn hash = m_hash;
    int index = hash(file.m_name) % m_currentCap;
    int i = 0;

    // Find a unique, unoccuppied or unused index
    while (m_currentTable[index] != nullptr && !(*m_currentTable[index] == file) &&
            m_currentTable[index]->m_used != false){
        if (m_currProbing == LINEAR){
            index = (index + 1) % m_currentCap;
        }
        else if (m_currProbing == QUADRATIC){
            index = ((hash(file.m_name) % m_currentCap) + (i*i)) % m_currentCap;
            i++;
        }
        else{
            index = ((hash(file.m_name) % m_currentCap) + i * (11-(hash(file.m_name) % 11))) % m_currentCap;
            i++;
        }
    }

    // Check that there's not a duplicate
    if (m_currentTable[index] == nullptr || m_currentTable[index]->m_used == false){
        // If data exists but is unused, deallocate data before inserting
        if (m_currentTable[index] != nullptr){
            delete m_currentTable[index];
        }
        m_currentTable[index] = new File(file.m_name, file.m_diskBlock, true);
        m_currentSize++;
        // If load factor is greater than 50% or rehash process already started, rehash table
        if (lambda() > 0.5 || m_transferIndex != 0){
            reHash();
        }
        return true;
    }
    else{
        return false;
    }
}

void FileSys::insertHash(File file){
    hash_fn hash = m_hash;
    int index = hash(file.m_name) % m_currentCap;
    int i = 0;

    while (m_currentTable[index] != nullptr && !(*m_currentTable[index] == file) &&
            m_currentTable[index]->m_used != false){
        if (m_currProbing == LINEAR){
            index = (index + 1) % m_currentCap;
        }
        else if (m_currProbing == QUADRATIC){
            index = ((hash(file.m_name) % m_currentCap) + (i*i)) % m_currentCap;
            i++;
        }
        else{
            index = ((hash(file.m_name) % m_currentCap) + i * (11-(hash(file.m_name) % 11))) % m_currentCap;
            i++;
        }
    }
    if (m_currentTable[index] == nullptr || m_currentTable[index]->m_used == false){
        m_currentTable[index] = new File(file.m_name, file.m_diskBlock, true);
        m_currentSize++;
    }
}

void FileSys::reHash(){
    // Transfer data from current table to old table
    if (m_oldTable == nullptr){
        m_oldTable = m_currentTable;
        m_oldCap = m_currentCap;
        m_oldSize = m_currentSize;
        m_oldNumDeleted = m_currNumDeleted;
        m_oldProbing = m_currProbing;
        m_currentCap = findNextPrime((m_currentSize - m_currNumDeleted) * 4);
        // Allocate memory for new table of new capacity and initialize to null
        m_currentTable = new File*[m_currentCap];
        for (int i = 0; i < m_currentCap; i++){
            m_currentTable[i] = nullptr;
        }
        m_currentSize = 0;
        m_currNumDeleted = 0;
        m_currProbing = m_newPolicy;
    }
    int twentyFivePercent = m_oldCap / 4;
    int endIndex = m_transferIndex + twentyFivePercent;

    // Scan 25% of old table
    for (int i = m_transferIndex; i < endIndex; i++){
        if (i < m_oldCap){
            // Only transfer used files
            if (m_oldTable[i] != nullptr && m_oldTable[i]->m_used == true){
                insertHash(*m_oldTable[i]);
            }
            // Once all files transfered, delete old table and set transfer index back to 0
            if (i == m_oldCap - 1){
                for (int j = 0; j < m_oldCap; j++){
                    if (m_oldTable[j] != nullptr){
                        delete m_oldTable[j];
                        m_oldTable[j] = nullptr;
                    }
                }
                delete [] m_oldTable;
                m_oldTable = nullptr;
                m_transferIndex = 0;
                return;
            }
        }
    }
    // Update transfer index
    m_transferIndex = endIndex;
}

bool FileSys::remove(File file){
    hash_fn hash = m_hash;
    int index = hash(file.m_name) % m_currentCap;
    int i = 0;

    // Check that file exists
    if (!(getFile(file.m_name, file.m_diskBlock) == File())){
        // Find index of file to delete
        while (m_currentTable[index] != nullptr && !(*m_currentTable[index] == file)){
            if (m_currProbing == LINEAR){
                index = (index + 1) % m_currentCap;
            }
            else if (m_currProbing == QUADRATIC){
                index = ((hash(file.m_name) % m_currentCap) + (i*i)) % m_currentCap;
                i++;
            }
            else{
                index = ((hash(file.m_name) % m_currentCap) + i * (11-(hash(file.m_name) % 11))) % m_currentCap;
                i++;
            }
        }

        // If found, set m_used to false
        if (m_currentTable[index] != nullptr && *m_currentTable[index] == file){
            m_currentTable[index]->m_used = false;
            m_currNumDeleted++;
            // If deleted ratio is greater than 80% or rehash process already started, rehash again
            if (deletedRatio() > 0.8 || m_transferIndex != 0){
                reHash();
            }
            return true;
        }

        // If node to delete not in current table, check old table
        if (m_oldTable != nullptr){
            index = hash(file.m_name) % m_oldCap;
            i = 0;
            while (m_oldTable[index] != nullptr && !(*m_oldTable[index] == file) &&
                    m_oldTable[index]->m_used != false){
                if (m_oldProbing == LINEAR){
                    index = (index + 1) % m_oldCap;
                }
                else if (m_oldProbing == QUADRATIC){
                    index = ((hash(file.m_name) % m_oldCap) + (i*i)) % m_oldCap;
                    i++;
                }
                else{
                    index = ((hash(file.m_name) % m_oldCap) + i * (11-(hash(file.m_name) % 11))) % m_oldCap;
                    i++;
                }
            }

            // Set m_used to false and rehash
            if (*m_oldTable[index] == file){
                m_oldTable[index]->m_used = false;
                m_oldNumDeleted++;
                reHash();
                return true;
            }
        }
    }
    return false;
}

const File FileSys::getFile(string name, int block) const{
    hash_fn hash = m_hash;
    int index = hash(name) % m_currentCap;
    int i = 0;

    // Check current table for file
    while (m_currentTable[index] != nullptr){
        if (m_currentTable[index]->m_name == name &&
            m_currentTable[index]->m_diskBlock == block &&
            m_currentTable[index]->m_used == true){
            return *m_currentTable[index];
        }
        if (m_currProbing == LINEAR){
            index = (index + 1) % m_currentCap;
        }
        else if (m_currProbing == QUADRATIC){
            index = ((hash(name) % m_currentCap) + (i*i)) % m_currentCap;
            i++;
        }
        else{
            index = ((hash(name) % m_currentCap) + i * (11-(hash(name) % 11))) % m_currentCap;
            i++;
        }
    }

    // If not in current table, check old table
    if (m_oldTable != nullptr){
        index = hash(name) % m_oldCap;
        i = 0;
        while (m_oldTable[index] != nullptr){
            if (m_oldTable[index]->m_name == name &&
                m_oldTable[index]->m_diskBlock == block &&
                m_oldTable[index]->m_used == true){
                return *m_oldTable[index];
            }
            if (m_oldProbing == LINEAR){
                index = (index + 1) % m_oldCap;
            }
            else if (m_oldProbing == QUADRATIC){
                index = ((hash(name) % m_oldCap) + (i*i)) % m_oldCap;
                i++;
            }
            else{
                index = ((hash(name) % m_oldCap) + i * (11-(hash(name) % 11))) % m_oldCap;
                i++;
            }
        }
    }
    return File();
}

bool FileSys::updateDiskBlock(File file, int block){
    hash_fn hash = m_hash;
    int index = hash(file.m_name) % m_currentCap;
    int i = 0;

    // Check that file exists
    if (!(getFile(file.m_name, file.m_diskBlock) == File())){
        // Check in current table
        while (m_currentTable[index] != nullptr && m_currentTable[index]->m_used != false){
            // If found, update disk block
            if (*m_currentTable[index] == file){
                m_currentTable[index]->setDiskBlock(block);
                return true;
            }
            if (m_currProbing == LINEAR){
                index = (index + 1) % m_currentCap;
            }
            else if (m_currProbing == QUADRATIC){
                index = ((hash(file.m_name) % m_currentCap) + (i*i)) % m_currentCap;
                i++;
            }
            else{
                index = ((hash(file.m_name) % m_currentCap) + i * (11-(hash(file.m_name) % 11))) % m_currentCap;
                i++;
            }
        }

        // If not in current table, check old table
        if (m_oldTable != nullptr){
            index = hash(file.m_name) % m_oldCap;
            i = 0;
            while (m_oldTable[index] != nullptr){
                // Once found, update disk block
                if (*m_oldTable[index] == file){
                    m_oldTable[index]->setDiskBlock(block);
                    return true;
                }
                if (m_oldProbing == LINEAR){
                    index = (index + 1) % m_oldCap;
                }
                else if (m_oldProbing == QUADRATIC){
                    index = ((hash(file.m_name) % m_oldCap) + (i*i)) % m_oldCap;
                    i++;
                }
                else{
                    index = ((hash(file.m_name) % m_oldCap) + i * (11-(hash(file.m_name) % 11))) % m_oldCap;
                    i++;
                }
            }
        }
    }
    return false;
}

float FileSys::lambda() const {
    return (float)m_currentSize / (float)m_currentCap;
}

float FileSys::deletedRatio() const {
    return (float)m_currNumDeleted / (float)m_currentSize;
}

void FileSys::dump() const {
    cout << "Dump for the current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for the old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

bool FileSys::isPrime(int number){
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int FileSys::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}
