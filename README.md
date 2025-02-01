# cross-miner
Duco cross platform and cross compile miner.

## Cross Compile on Linux
#### Local (Static):
```bash
apt install make gcc git perl
```
```bash
wget https://www.openssl.org/source/openssl-1.1.1k.tar.gz
./Configure -static 
make -j$(nproc)
```
```bash
git clone https://github.com/Naster17/cross-miner/
cd cross-miner/
make OPENSSL=/path/to/openssl-1.1.1k CC=gcc
```

#### ARM (Static):
```bash
apt install gcc-arm-linux-gnueabi make gcc git perl
```
```bash
wget https://www.openssl.org/source/openssl-1.1.1k.tar.gz
./Configure linux-armv4 -static 
make -j$(nproc)
```
```bash
git clone https://github.com/Naster17/cross-miner/
cd cross-miner/
make OPENSSL=/path/to/openssl-1.1.1k CC=arm-linux-gnueabi-gcc
```
#### ARM64 (Static):
```bash
apt install gcc-aarch64-linux-gnu make gcc git perl
```
```bash
wget https://www.openssl.org/source/openssl-1.1.1k.tar.gz
./Configure linux-aarch64 -static 
make -j$(nproc)
```
```bash
git clone https://github.com/Naster17/cross-miner/
cd cross-miner/
make OPENSSL=/path/to/openssl-1.1.1k CC=aarch64-linux-gnu-gcc
```
#### Mine
```bash
./cpuminer username key
./cpuminer qwerty 1234
```

### Tested 
- DiskStation DS211j - arm v5l (DSM 6.2.4-25556)
- Raspberry Pi 4B - arm64 (kali linux 2024.4) 
- Ryzen 5 8645HS - x86 (debian 12)
- Poco F6 - arm64 (Termux)

### Screenshots
![ARM64](https://github.com/user-attachments/assets/82e975da-5b8e-4419-a3c4-846967faebf5)
![ARM](https://github.com/user-attachments/assets/43373bd4-b642-4a90-864d-2e67c709da1d)
