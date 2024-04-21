#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <random>
#include <sstream>
#include <thread>

struct Packet {
  Packet(size_t off, size_t len, uint8_t* data) : offset(off), length(len), data(data) {}
  size_t offset;
  size_t length;
  uint8_t* data;
};

class TCP {
 public:
  TCP() {
    nextOffset_ = 0;
    finished_ = false;
  }

  size_t read(void* buf, size_t count) {
    std::unique_lock<std::mutex> lock(mu_);

    size_t leftBytes = count;
    while (leftBytes > 0) {
      if (!packets_.empty()) {
        size_t offset = packets_.begin()->first;
        auto* p = packets_.begin()->second;
        if (offset == nextOffset_) {
          // fetch the packet
          packets_.erase(offset);

          // copy to the user buf
          size_t len = std::min(p->length, leftBytes);
          std::memcpy(buf, p->data, len);
          leftBytes -= len;
          nextOffset_ += len;

          // put back the left data
          p->length -= len;
          if (p->length > 0) {
            p->data += len;
            p->offset += len;
            packets_[p->offset] = p;
          }

          return count - leftBytes;
        }
      } else if (finished_) {
        break;
      }

      cv_.wait(lock);
    }

    // finished
    return 0;
  }

  void receive(Packet* p) {
    std::unique_lock<std::mutex> lock(mu_);
    packets_[p->offset] = p;
    cv_.notify_one();
  }

  void finish() {
    std::unique_lock<std::mutex> lock(mu_);
    finished_ = true;
    cv_.notify_one();
  }

 private:
  std::mutex mu_;
  std::condition_variable cv_;
  size_t nextOffset_;
  bool finished_;
  std::map<uint64_t, Packet*> packets_;
};

TCP tcp;

void producer(uint8_t* data) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(50, 99);

  std::cout << "construct data..." << std::endl;
  std::vector<Packet*> packets(100);
  size_t offset = 0;
  for (size_t i = 0; i < 100; ++i) {
    size_t randLen = dis(gen);
    packets[i] = new Packet(offset, randLen, data);
    data += randLen;
    offset += randLen;
  }

  std::cout << "total " << offset << " bytes" << std::endl;
  std::cout << "make the data unordered..." << std::endl;
  std::shuffle(packets.begin(), packets.end(), gen);

  std::cout << "give it to tcp..." << std::endl;
  for (size_t i = 0; i < 100; ++i) {
    tcp.receive(packets[i]);
    std::cout << "receive data [" << packets[i]->offset << " ,"
              << packets[i]->offset + packets[i]->length << "): " << packets[i]->length << " bytes"
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  tcp.finish();
}

void consumer(uint8_t* data) {
  size_t nBytes = 0;
  uint8_t buf[100];

  size_t offset = 0;
  while ((nBytes = tcp.read(buf, 50)) > 0) {
    auto diff = std::memcmp(data + offset, buf, nBytes);
    std::stringstream ss;
    ss << "read data [" << offset << " ," << offset + nBytes << "): " << nBytes << " bytes";
    if (!diff) {
      ss << "; verify ok";
    } else {
      ss << "; verify bad";
    }
    std::cout << ss.str() << std::endl;

    offset += nBytes;
  }
  std::cout << "read finish" << std::endl;
}

int main() {
  uint8_t buffer[10000];
  for (size_t i = 0; i < 10000; ++i) {
    buffer[i] = i & 0xff;
  }
  std::thread c(consumer, buffer);
  std::thread p(producer, buffer);

  p.join();
  c.join();
}
